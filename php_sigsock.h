/*
   +----------------------------------------------------------------------+
   | unknown license:                                                      |
   +----------------------------------------------------------------------+
   +----------------------------------------------------------------------+
*/

/* $ Id: $ */ 

#ifndef PHP_SIGSOCK_H
#define PHP_SIGSOCK_H

#ifdef  __cplusplus
extern "C" {
#endif

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <php.h>

#ifdef HAVE_SIGSOCK
#define PHP_SIGSOCK_VERSION "0.1.0"

#define SIGSOCK_MAX_SIGNALS	32

#include <php_ini.h>
#include <SAPI.h>
#include <ext/standard/info.h>
#include <Zend/zend_extensions.h>
#ifdef  __cplusplus
} // extern "C" 
#endif
#ifdef  __cplusplus
extern "C" {
#endif

extern zend_module_entry sigsock_module_entry;
#define phpext_sigsock_ptr &sigsock_module_entry

#ifdef PHP_WIN32
#define PHP_SIGSOCK_API __declspec(dllexport)
#else
#define PHP_SIGSOCK_API
#endif

PHP_MINIT_FUNCTION(sigsock);
PHP_MSHUTDOWN_FUNCTION(sigsock);
PHP_RINIT_FUNCTION(sigsock);
PHP_RSHUTDOWN_FUNCTION(sigsock);
PHP_MINFO_FUNCTION(sigsock);

ZEND_BEGIN_MODULE_GLOBALS(sigsock)
	int socktable[SIGSOCK_MAX_SIGNALS+1];
	zval *streamtable[SIGSOCK_MAX_SIGNALS+1];
ZEND_END_MODULE_GLOBALS(sigsock)

#ifdef ZTS
#define SIGSOCK_G(v) TSRMG(sigsock_globals_id, zend_sigsock_globals *, v)
#else
#define SIGSOCK_G(v)  (sigsock_globals.v)
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

#ifdef  __cplusplus
} // extern "C" 
#endif

#endif /* PHP_HAVE_SIGSOCK */

#endif /* PHP_SIGSOCK_H */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
