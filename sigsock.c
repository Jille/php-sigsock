/*
   +----------------------------------------------------------------------+
   | unknown license:                                                      |
   +----------------------------------------------------------------------+
   +----------------------------------------------------------------------+
*/

/* $ Id: $ */ 

#include "php_sigsock.h"

#if HAVE_SIGSOCK

/* {{{ sigsock_functions[] */
function_entry sigsock_functions[] = {
	{ NULL, NULL, NULL }
};
/* }}} */


/* {{{ sigsock_module_entry
 */
zend_module_entry sigsock_module_entry = {
	STANDARD_MODULE_HEADER,
	"sigsock",
	sigsock_functions,
	PHP_MINIT(sigsock),     /* Replace with NULL if there is nothing to do at php startup   */ 
	PHP_MSHUTDOWN(sigsock), /* Replace with NULL if there is nothing to do at php shutdown  */
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


/* {{{ PHP_MINIT_FUNCTION */
PHP_MINIT_FUNCTION(sigsock)
{

	/* add your stuff here */

	return SUCCESS;
}
/* }}} */


/* {{{ PHP_MSHUTDOWN_FUNCTION */
PHP_MSHUTDOWN_FUNCTION(sigsock)
{

	/* add your stuff here */

	return SUCCESS;
}
/* }}} */


/* {{{ PHP_RINIT_FUNCTION */
PHP_RINIT_FUNCTION(sigsock)
{
	/* add your stuff here */

	return SUCCESS;
}
/* }}} */


/* {{{ PHP_RSHUTDOWN_FUNCTION */
PHP_RSHUTDOWN_FUNCTION(sigsock)
{
	/* add your stuff here */

	return SUCCESS;
}
/* }}} */


/* {{{ PHP_MINFO_FUNCTION */
PHP_MINFO_FUNCTION(sigsock)
{
	php_printf("The unknown extension\n");
	php_info_print_table_start();
	php_info_print_table_row(2, "Version",PHP_SIGSOCK_VERSION " (devel)");
	php_info_print_table_row(2, "Released", "2015-03-23");
	php_info_print_table_row(2, "CVS Revision", "$Id: $");
	php_info_print_table_end();
	/* add your stuff here */

}
/* }}} */

#endif /* HAVE_SIGSOCK */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
