check_PROGRAMS  = hemp
TESTS           = $(check_PROGRAMS)
#SUBDIRS         = core type language .
SUBDIRS         = core language
LDADD           = ../library/libhemp.la tap.o
CFLAGS          = @CFLAGS@ \
                -I"${top_srcdir}/include" \
                -DHEMP_TESTDIR='"$(abs_top_srcdir)/tests"'
