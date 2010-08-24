check_PROGRAMS  =   value

#check_PROGRAMS  =   action      \
#                    context     \
#                    cstr        \
#                    dialect     \
#                    element     \
#                    elements    \
#                    error       \
#                    factory     \
#                    filesystem  \
#                    grammar     \
#                    hash        \
#                    list        \
#                    memory      \
#                    pool        \
#                    ptree       \
#                    slab        \
#                    source      \
#                    tag         \
#                    tagset      \
#                    template    \
#                    text        \
#                    utils       \
#                    value
 
TESTS           = $(check_PROGRAMS)
LDADD           = ../../library/libhemp.la
CFLAGS          = @CFLAGS@ \
                -I"${top_srcdir}/include" \
                -DHEMP_TESTDIR='"$(abs_top_srcdir)/tests"'
