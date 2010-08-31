AC_PREREQ(2.61)
AC_INIT([hemp], [0.01], [hemp@wardley.org])
AC_CONFIG_AUX_DIR(build)
AC_CONFIG_MACRO_DIR([build/m4])

AM_INIT_AUTOMAKE([-Wall -Werror foreign subdir-objects color-tests])

AC_CONFIG_HEADERS([include/hemp/config.h])

AC_CONFIG_FILES([
    Makefile 
    include/Makefile 
    library/Makefile 
    programs/Makefile 
    tests/Makefile
    tests/core/Makefile
    tests/type/Makefile
    tests/language/Makefile
    tests/language/hemp/Makefile
    build/libltdl/Makefile
])

#    modules/Makefile

# Checks for programs
AC_PROG_CC
AM_PROG_CC_C_O
AC_PROG_CPP
AC_PROG_LIBTOOL
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_RANLIB

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([argz.h limits.h stddef.h stdlib.h string.h unistd.h])
AC_CHECK_LIB([readline], [readline], [LIBS="-lreadline $LIBS"])


# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_FUNC_ERROR_AT_LINE
AC_FUNC_VPRINTF
AC_FUNC_STRTOD
AC_CHECK_FUNCS([atexit bcopy getcwd memmove memset strchr strdup strrchr strstr strtol])


CFLAGS="${CFLAGS=}"

dnl this macro is used to get the arguments supplied
dnl to the configure script (./configure --enable-debug)
dnl Check if we have enable debug support.

AC_MSG_CHECKING(whether to enable debugging)
AC_ARG_ENABLE(
    debug, [  --enable-debug=[no/yes/N] turn on debugging],, enable_debug=no)

if test "x$enable_debug" = "xyes"; then
    CFLAGS="$CFLAGS -g -DHEMP_DEBUG"
    AC_MSG_RESULT(yes)
elif test "x$enable_debug" = "xno"; then
    CFLAGS="$CFLAGS -O3"
    AC_MSG_RESULT(no)
else
    CFLAGS="$CFLAGS -g -DHEMP_DEBUG=$enable_debug"
    AC_MSG_RESULT(yes)
fi

# Configure libtool with dlopen support if possible
LT_CONFIG_LTDL_DIR([build/libltdl])
LT_INIT([dlopen])
LTDL_INIT([recursive])

AC_OUTPUT
