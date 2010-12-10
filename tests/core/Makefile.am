#check_PROGRAMS  =   namespace

check_PROGRAMS  =   action      \
                    codec       \
                    context     \
                    dialect     \
                    document    \
                    error       \
                    factory     \
                    filesystem  \
                    fragment    \
                    fragments   \
                    grammar     \
                    memory      \
                    namespace   \
                    proto       \
                    pool        \
                    ptree       \
                    slab        \
                    source      \
                    string      \
                    tag         \
                    tagset      \
                    type        \
                    utils       \
                    value       \
                    viewer
 
TESTS           = $(check_PROGRAMS)
LDADD           = ../../library/libhemp.la
CFLAGS          = @CFLAGS@ \
                -I"${top_srcdir}/include" \
                -DHEMP_TESTDIR='"$(abs_top_srcdir)/tests"'
