check_PROGRAMS  =    hash

#check_PROGRAMS  =   assign      \
#                    boolops     \
#                    comments    \
#                    functions   \
#                    hash        \
#                    lists       \
#                    numbers     \
#                    numops      \
#                    parens      \
#                    quotes      \
#                    textops     \
#                    words

TESTS           = $(check_PROGRAMS)
LDADD           = ../../../library/libhemp.la
CFLAGS          = @CFLAGS@ \
                  -I"$(top_srcdir)/include" \
                  -DHEMP_TESTDIR='"$(abs_top_srcdir)/tests"' 
