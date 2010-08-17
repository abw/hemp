# check_PROGRAMS  =   
SUBDIRS         = core language .
#TESTS           = $(check_PROGRAMS)
LDADD           = ../library/libhemp.la tap.o
CFLAGS          = @CFLAGS@ \
                -I"${top_srcdir}/include" \
                -DHEMP_TESTDIR='"$(abs_top_srcdir)/tests"'
