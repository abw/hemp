#check_PROGRAMS  =   type

check_PROGRAMS  =   action      \
                    context     \
                    dialect     \
                    element     \
                    elements    \
                    error       \
                    factory     \
                    filesystem  \
                    grammar     \
                    memory      \
                    pool        \
                    ptree       \
                    slab        \
                    source      \
                    string      \
                    tag         \
                    tagset      \
                    template    \
                    type        \
                    utils       \
                    value
 
TESTS           = $(check_PROGRAMS)
LDADD           = ../../library/libhemp.la
CFLAGS          = @CFLAGS@ \
                -I"${top_srcdir}/include" \
                -DHEMP_TESTDIR='"$(abs_top_srcdir)/tests"'
