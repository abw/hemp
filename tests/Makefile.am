#check_PROGRAMS  =   value scanner

check_PROGRAMS  =   action      \
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
#                  ptree         \
#                  type          


TESTS           = $(check_PROGRAMS)
LDADD           = ../library/libhemp.la tap.o
CFLAGS          = -I../include
CPPFLAGS        = -DDEBUG=2 -DTESTDIR='"$(abs_top_srcdir)/tests"'
#CPPFLAGS        = -DTESTDIR='"$(abs_top_srcdir)/tests"'

