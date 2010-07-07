AC_PREREQ(2.61)
AC_INIT([hemp], [0.01], [make@gnu.org])

AM_INIT_AUTOMAKE([-Wall -Werror foreign])

AC_PROG_CC

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([stdlib.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_CONFIG_HEADERS([include/config.h])
AC_CONFIG_FILES([
    Makefile
    source/Makefile
])

# Checks for library functions.
AC_FUNC_VPRINTF

AC_OUTPUT

