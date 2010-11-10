check_PROGRAMS  =   namespace viewer

#check_PROGRAMS  =   action      \
#                    context     \
#                    dialect     \
#                    element     \
#                    elements    \
#                    error       \
#                    factory     \
#                    filesystem  \
#                    grammar     \
#                    memory      \
#                    namespace   \
#                    pool        \
#                    ptree       \
#                    slab        \
#                    source      \
#                    string      \
#                    tag         \
#                    tagset      \
#                    template    \
#                    type        \
#                    utils       \
#                    value       \
#                    viewer
 
TESTS           = $(check_PROGRAMS)
LDADD           = ../../library/libhemp.la
CFLAGS          = @CFLAGS@ \
                -I"${top_srcdir}/include" \
                -DHEMP_TESTDIR='"$(abs_top_srcdir)/tests"'
