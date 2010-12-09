check_PROGRAMS  =   stack

#check_PROGRAMS  =   action      \
#                    codec       \
#                    context     \
#                    dialect     \
#                    error       \
#                    factory     \
#                    filesystem  \
#                    fragment    \
#                    fragments   \
#                    grammar     \
#                    memory      \
#                    namespace   \
#                    proto       \
#                    pool        \
#                    ptree       \
#                    slab        \
#                    source      \
#                    stack       \
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
