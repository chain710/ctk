#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT(ctk, 1.0.0, chain710@gmail.com)
AC_CONFIG_SRCDIR([dummy.tag.c])
AC_CONFIG_HEADER([config.h])

AM_INIT_AUTOMAKE(ctk, 1.0.0)

AC_PREFIX_DEFAULT(/usr/local/ctk/)

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
#AC_PROG_LN_S
#AC_PROG_MAKE_SET
#AC_PROG_RANLIB
#AC_PROG_INSTALL
AC_PROG_LIBTOOL

# Checks for libraries.

AC_CHECK_LIB([fcgi], [FCGX_Accept], [have_fcgi=yes])

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

if test "x${have_fcgi}" = xno; then
AC_MSG_ERROR([
------------------------------------------
Unable to find libfcgi!!!
------------------------------------------
])
fi

AC_OUTPUT([
    Makefile
    helper/Makefile
    http/Makefile
    cgi_wrapper/Makefile
])
