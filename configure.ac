AC_PREREQ(2.61)
AC_INIT([hemp], [0.01], [make@gnu.org])
AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE([-Wall -Werror foreign])

AC_PROG_CC
AC_PROG_LIBTOOL

AC_CONFIG_HEADERS([
    include/config.h
])
AC_CONFIG_FILES([
    Makefile
    source/Makefile
    tests/Makefile
])

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([stdlib.h string.h])


# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST

# Checks for library functions.
AC_FUNC_ERROR_AT_LINE
AC_FUNC_VPRINTF
AC_CHECK_FUNCS([memset strdup])


CFLAGS="${CFLAGS=}"
dnl this macro is used to get the arguments supplied
dnl to the configure script (./configure --enable-debug)
dnl Check if we have enable debug support.

AC_MSG_CHECKING(whether to enable debugging)
AC_ARG_ENABLE(
    debug, [  --enable-debug=[no/yes] turn on debugging],, enable_debug=yes)
if test "x$enable_debug" = "xyes"; then
    CFLAGS="$CFLAGS -g -DDEBUG"
    AC_MSG_RESULT(yes)
else
    CFLAGS="$CFLAGS -O3"
    AC_MSG_RESULT(no)
fi


AC_OUTPUT

