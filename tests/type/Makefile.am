check_PROGRAMS  =   list

#check_PROGRAMS  =   code     \
#                    text     \
#                    list     \
#                    hash     \
#                    params   
 
TESTS           = $(check_PROGRAMS)
LDADD           = ../../library/libhemp.la
CFLAGS          = @CFLAGS@ \
                -I"${top_srcdir}/include" \
                -DHEMP_TEST_DIR='"$(abs_top_srcdir)/tests"'
