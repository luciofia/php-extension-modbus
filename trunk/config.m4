dnl $Id$
dnl config.m4 for extension modbus

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(modbus, for modbus support,
dnl Make sure that the comment is aligned:
dnl [  --with-modbus             Include modbus support])

dnl Otherwise use enable:

PHP_ARG_ENABLE(modbus, whether to enable modbus support,
Make sure that the comment is aligned:
[  --enable-modbus           Enable modbus support])

if test "$PHP_MODBUS" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-modbus -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/modbus.h"  # you most likely want to change this
  dnl if test -r $PHP_MODBUS/$SEARCH_FOR; then # path given as parameter
  dnl   MODBUS_DIR=$PHP_MODBUS
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for modbus files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       MODBUS_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$MODBUS_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the modbus distribution])
  dnl fi

  dnl # --with-modbus -> add include path
  dnl PHP_ADD_INCLUDE($MODBUS_DIR/include)

  dnl # --with-modbus -> check for lib and symbol presence
  dnl LIBNAME=modbus # you may want to change this
  dnl LIBSYMBOL=modbus # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $MODBUS_DIR/lib, MODBUS_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_MODBUSLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong modbus lib version or lib not found])
  dnl ],[
  dnl   -L$MODBUS_DIR/lib -lm -ldl
  dnl ])
  dnl
  dnl PHP_SUBST(MODBUS_SHARED_LIBADD)

  PHP_NEW_EXTENSION(modbus, modbus.c functions.c serial.c, $ext_shared)
fi
