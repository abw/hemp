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
                    hash        \
                    list        \
                    memory      \
                    pool        \
                    ptree       \
                    slab        \
                    source      \
                    string      \
                    tag         \
                    tagset      \
                    template    \
                    text        \
                    type        \
                    utils       \
                    value
 
TESTS           = $(check_PROGRAMS)
LDADD           = ../../library/libhemp.la
CFLAGS          = @CFLAGS@ \
                -I"${top_srcdir}/include" \
                -DHEMP_TESTDIR='"$(abs_top_srcdir)/tests"'
