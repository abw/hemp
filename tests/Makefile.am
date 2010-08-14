#check_PROGRAMS  =   value

check_PROGRAMS  =   action      \
                    cstr        \
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
                    scanner     \
                    source      \
                    slab        \
                    tag         \
                    tagset      \
                    template    \
                    text        \
                    tt3         \
                    utils       \
                    value
 
                    
#                  debug         \
#                  benchmark  
#                  hemp          \
#                  hub          \
#                  module        \
#                  type          


TESTS           = $(check_PROGRAMS)
LDADD           = ../library/libhemp.la tap.o
CFLAGS          = @CFLAGS@ -I../include
CPPFLAGS        = -DTESTDIR='"$(abs_top_srcdir)/tests"'

