/*
   +----------------------------------------------------------------------+
   | unknown license:                                                      |
   +----------------------------------------------------------------------+
   +----------------------------------------------------------------------+
*/

/* $ Id: $ */ 

#include <fcntl.h>
#include "php_sigsock.h"
#include "php_network.h"
#include "php_signal.c"

#if HAVE_SIGSOCK

#undef ZEND_ASSERT
#define ZEND_ASSERT(x) if (!(x)) { printf("Assertion failed: " #x "\n"); abort(); }

ZEND_DECLARE_MODULE_GLOBALS(sigsock)

ZEND_BEGIN_ARG_INFO_EX(arginfo_sigsock_register, 0, 0, 2)
	ZEND_ARG_INFO(0, stream)
	ZEND_ARG_INFO(0, signo)
	ZEND_ARG_INFO(0, restart_syscalls)
ZEND_END_ARG_INFO()


/* {{{ PHP_RINIT_FUNCTION */
PHP_RINIT_FUNCTION(sigsock)
{
	int i;
	for(i = 0; SIGSOCK_MAX_SIGNALS > i; i++) {
		SIGSOCK_G(socktable)[i] = -1;
		SIGSOCK_G(streamtable)[i] = NULL;
	}

	return SUCCESS;
}
/* }}} */


/* {{{ PHP_RSHUTDOWN_FUNCTION */
PHP_RSHUTDOWN_FUNCTION(sigsock)
{
	int i;
	for(i = 0; SIGSOCK_MAX_SIGNALS > i; i++) {
		if (SIGSOCK_G(socktable)[i] != -1) {
			ZEND_ASSERT(SIGSOCK_G(streamtable)[i] != NULL);
			close(SIGSOCK_G(socktable)[i]);
			zval_ptr_dtor(&SIGSOCK_G(streamtable)[i]);
		} else {
			ZEND_ASSERT(SIGSOCK_G(streamtable)[i] == NULL);
		}
	}

	return SUCCESS;
}
/* }}} */


/* {{{ PHP_MINFO_FUNCTION */
PHP_MINFO_FUNCTION(sigsock)
{
	php_printf("Receive signals via a stream\n");
	php_info_print_table_start();
	php_info_print_table_row(2, "Version", PHP_SIGSOCK_VERSION);
	php_info_print_table_end();

}
/* }}} */

static void sigsock_signal_handler(int signo)
{
	TSRMLS_FETCH();
	int fd = SIGSOCK_G(socktable)[signo];
	ZEND_ASSERT(fd != -1);
	char buf = signo;
	if (write(fd, &buf, 1) == -1) {
		// TODO: Can we do anything reasonable here?
	}
}

static zend_bool sigsock_sock_is_used(int fd) {
	int i;
	for(i = 0; SIGSOCK_MAX_SIGNALS > i; i++) {
		if (SIGSOCK_G(socktable)[fd] == SIGSOCK_G(socktable)[i]) {
			return 1;
		}
	}
	return 0;
}

/* {{{ proto stream sigsock_register(stream s, int signo [, bool restart_syscalls])
   Assigns a system signal handler to a PHP stream */
PHP_FUNCTION(sigsock_register)
{
	zval *z_stream;
	long signo;
	zend_bool restart_syscalls = 1;

	int copyfromsig;
	int old_fd = 1, new_fd = -1;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "r!l|b", &z_stream, &signo, &restart_syscalls) == FAILURE) {
		return;
	}

	if (signo < 1 || signo > SIGSOCK_MAX_SIGNALS) {
		php_error_docref(NULL, E_WARNING, "Invalid signal");
		RETURN_FALSE;
	}

	if (z_stream == NULL) {
		copyfromsig = -1;
	} else {
		int i;
		ZEND_ASSERT(Z_TYPE_P(z_stream) == IS_RESOURCE);
		copyfromsig = -1;
		for(i = 0; SIGSOCK_MAX_SIGNALS > i; i++) {
			if (SIGSOCK_G(streamtable)[i] != NULL) {
				if (SIGSOCK_G(streamtable)[i] == z_stream) {
					copyfromsig = i;
					break;
				}
			}
		}
		if (copyfromsig == -1) {
			php_error_docref(NULL, E_WARNING, "Stream given to sigsock_register() was not created by sigsock");
			RETURN_FALSE;
		}
	}

	if (copyfromsig == -1) {
		int pair[2];
		php_stream *stream;
		// TODO: CLOEXEC
		if (socketpair(AF_UNIX, SOCK_STREAM, 0, pair) == -1) {
			php_error_docref(NULL TSRMLS_CC, E_WARNING, "Unable to create socket pair: %s", strerror(errno));
			return;
		}
		int flags = fcntl(pair[0], F_GETFL);
		if (flags == -1) {
			php_error_docref(NULL TSRMLS_CC, E_WARNING, "Unable to fcntl: %s", strerror(errno));
			close(pair[0]);
			close(pair[1]);
			return;
		}
		flags |= O_NONBLOCK;
		if (fcntl(pair[0], F_SETFL, flags) == -1) {
			php_error_docref(NULL TSRMLS_CC, E_WARNING, "Unable to fcntl: %s", strerror(errno));
			close(pair[0]);
			close(pair[1]);
			return;
		}

		new_fd = pair[0];
		stream = php_stream_sock_open_from_socket(pair[1], 0);
		php_stream_auto_cleanup(stream);
		RETVAL_RESOURCE(php_stream_get_resource_id(stream));
	} else {
		new_fd = SIGSOCK_G(socktable)[copyfromsig];
		RETVAL_ZVAL(SIGSOCK_G(streamtable)[copyfromsig], 1, 0);
	}

	if (php_signal4(signo, sigsock_signal_handler, (int) restart_syscalls, 1) == SIG_ERR) {
		php_error_docref(NULL, E_WARNING, "Error assigning signal");
		zval_ptr_dtor(&return_value);
		if (!sigsock_sock_is_used(new_fd)) {
			close(new_fd);
		}
		RETURN_FALSE;
	}

	Z_ADDREF_P(return_value);

	old_fd = SIGSOCK_G(socktable)[signo];
	SIGSOCK_G(socktable)[signo] = new_fd;

	if (old_fd != -1) {
		if (!sigsock_sock_is_used(old_fd)) {
			close(old_fd);
		}
		ZEND_ASSERT(SIGSOCK_G(streamtable)[signo] != NULL);
		zval_ptr_dtor(&SIGSOCK_G(streamtable)[signo]);
	}

	SIGSOCK_G(streamtable)[signo] = return_value;
}
/* }}} */

/* {{{ sigsock_functions[] */
zend_function_entry sigsock_functions[] = {
	PHP_FE(sigsock_register,		arginfo_sigsock_register)
	{ NULL, NULL, NULL }
};
/* }}} */

/* {{{ sigsock_module_entry
 */
zend_module_entry sigsock_module_entry = {
	STANDARD_MODULE_HEADER,
	"sigsock",
	sigsock_functions,
	NULL,
	NULL,
	PHP_RINIT(sigsock),     /* Replace with NULL if there is nothing to do at request start */
	PHP_RSHUTDOWN(sigsock), /* Replace with NULL if there is nothing to do at request end   */
	PHP_MINFO(sigsock),
	PHP_SIGSOCK_VERSION,
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_SIGSOCK
ZEND_GET_MODULE(sigsock)
#endif


#endif /* HAVE_SIGSOCK */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
