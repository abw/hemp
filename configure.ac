AC_PREREQ(2.61)
AC_INIT([hemp], [0.01], [hemp@wardley.org])
AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE([-Wall -Werror foreign subdir-objects color-tests])

AC_PROG_CC
AM_PROG_CC_C_O
AC_PROG_LIBTOOL

AC_CONFIG_HEADERS([
    include/config.h
])
AC_CONFIG_FILES([
    Makefile 
    include/Makefile 
    library/Makefile 
    programs/Makefile 
    tests/Makefile
    modules/Makefile
    extra/libltdl/Makefile
])

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([stdlib.h string.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST

# Checks for library functions.
AC_FUNC_ERROR_AT_LINE
AC_FUNC_VPRINTF
AC_CHECK_FUNCS([strdup, memset, bcopy, memmove])


CFLAGS="${CFLAGS=}"
dnl this macro is used to get the arguments supplied
dnl to the configure script (./configure --enable-debug)
dnl Check if we have enable debug support.

AC_MSG_CHECKING(whether to enable debugging)
AC_ARG_ENABLE(
    debug, [  --enable-debug=[no/yes/N] turn on debugging],, enable_debug=no)
if test "x$enable_debug" = "xyes"; then
    CFLAGS="$CFLAGS -g -DDEBUG"
    AC_MSG_RESULT(yes)
elif test "x$enable_debug" = "xno"; then
    CFLAGS="$CFLAGS -O3"
    AC_MSG_RESULT(no)
else
    CFLAGS="$CFLAGS -g -DDEBUG=$enable_debug"
    AC_MSG_RESULT(yes)
fi

# Configure libtool with dlopen support if possible
LT_CONFIG_LTDL_DIR([extra/libltdl])
LT_INIT([dlopen])
LTDL_INIT([recursive])

AC_OUTPUT
