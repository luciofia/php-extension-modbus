/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2007 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:                                                              |
  +----------------------------------------------------------------------+
*/

/* $Id: header 226204 2007-01-01 19:32:10Z iliaa $ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_modbus.h"

#include "serial.h"
#include "modbus.h"

/* If you declare any globals in php_modbus.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(modbus)
*/

/* True global resources - no need for thread safety here */
static int le_modbus;

/* {{{ modbus_functions[]
 *
 * Every user visible function must have an entry in modbus_functions[].
 */
zend_function_entry modbus_functions[] = {
	PHP_FE(confirm_modbus_compiled,	NULL)		/* For testing, remove later. */
	PHP_FE(modbus_open, NULL)
	PHP_FE(modbus_close, NULL)
	PHP_FE(modbus_read_holding_registers, NULL)
	PHP_FE(modbus_read_input_registers, NULL)
	PHP_FE(modbus_write, NULL)
	{NULL, NULL, NULL}	/* Must be the last line in modbus_functions[] */
};
/* }}} */

/* {{{ modbus_module_entry
 */
zend_module_entry modbus_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"modbus",
	modbus_functions,
	PHP_MINIT(modbus),
	PHP_MSHUTDOWN(modbus),
	PHP_RINIT(modbus),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(modbus),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(modbus),
#if ZEND_MODULE_API_NO >= 20010901
	"0.1", /* Replace with version number for your extension */
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_MODBUS
ZEND_GET_MODULE(modbus)
#endif

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("modbus.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_modbus_globals, modbus_globals)
    STD_PHP_INI_ENTRY("modbus.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_modbus_globals, modbus_globals)
PHP_INI_END()
*/
/* }}} */

/* {{{ php_modbus_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_modbus_init_globals(zend_modbus_globals *modbus_globals)
{
	modbus_globals->global_value = 0;
	modbus_globals->global_string = NULL;
}
*/
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(modbus)
{
	/* If you have INI entries, uncomment these lines 
	REGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(modbus)
{
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(modbus)
{
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(modbus)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(modbus)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "modbus support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */


/* Remove the following function when you have succesfully modified config.m4
   so that your module can be compiled into PHP, it exists only for testing
   purposes. */

/* Every user-visible function in PHP should document itself in the source */
/* {{{ proto string confirm_modbus_compiled(string arg)
   Return a string to confirm that the module is compiled in */
PHP_FUNCTION(confirm_modbus_compiled)
{
	char *arg = NULL;
	int arg_len, len;
	char *strg;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &arg, &arg_len) == FAILURE) {
		return;
	}

	len = spprintf(&strg, 0, "Congratulations! You have successfully modified ext/%.78s/config.m4. Module %.78s is now compiled into PHP.", "modbus", arg);
	RETURN_STRINGL(strg, len, 0);
}
/* }}} */
/* The previous line is meant for vim and emacs, so it can correctly fold and 
   unfold functions in source code. See the corresponding marks just before 
   function definition, where the functions purpose is also documented. Please 
   follow this convention for the convenience of others editing your code.
*/

PHP_FUNCTION(modbus_open)
{
	int fd;
	char* port;
	int port_len;
	int baudrate;
	char* parity;
	int parity_len;
	int stopbits;
	int timeout;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "slsll", &port, &port_len, &baudrate, &parity, &parity_len, &stopbits, &timeout) == FAILURE)
	{
		RETURN_LONG(-1);
	}

	fd = serial_open(port, baudrate, 8, *parity, stopbits, timeout);

	RETURN_LONG(fd);
}

PHP_FUNCTION(modbus_close)
{
	int port;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &port) == FAILURE)
	{
		RETURN_LONG(-1);
	}

	close(port);
}

PHP_FUNCTION(modbus_read_holding_registers)
{
	int port;
	int dev;
	int add;
	int len;
	int r;
	unsigned int datar[128];
	char datas[5];

	int i;

	array_init(return_value);

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "llll", &port, &dev, &add, &len) == FAILURE)
	{
		add_assoc_long(return_value, "ERROR", -2);
		return;
	}
	r = modbus_read_holding_registers(port, dev, add, len, datar);

	if (r == len)
	{
		for (i=0; i<len; i++)
		{
			sprintf(datas, "%04X", datar[i]);
			add_next_index_string(return_value, datas, 1);
		}
		add_assoc_long(return_value, "ERROR", 0);
	}
	else
	{
		add_assoc_long(return_value, "ERROR", r);
	}
}

PHP_FUNCTION(modbus_read_input_registers)
{
	int port;
	int dev;
	int add;
	int len;
	int r;
	unsigned int datar[128];
	char datas[5];

	int i;

	array_init(return_value);

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "llll", &port, &dev, &add, &len) == FAILURE)
	{
		add_assoc_long(return_value, "ERROR", -2);
		return;
	}

	r = modbus_read_input_registers(port, dev, add, len, datar);

	if (r == len)
	{
		for (i=0; i<len; i++)
		{
			sprintf(datas, "%04X", datar[i]);
			add_next_index_string(return_value, datas, 1);
		}
		add_assoc_long(return_value, "ERROR", 0);
	}
	else
	{
		add_assoc_long(return_value, "ERROR", r);
	}
}

PHP_FUNCTION(modbus_write)
{
	int port;
	int dev;
	int add;
	int len;
	int * dataw;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "llll", &port, &dev, &add, &len) == FAILURE)
	{
		RETURN_LONG(-2);
	}

//	modbus_write(port, dev, add, value);

	RETURN_LONG(0);
}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
