check_PROGRAMS  =   hash        \
                    list        \
                    memory      \
                    pool        \
                    ptree       \
                    source      \
                    slab        \
                    tag         \
                    tagset      \
                    text
                    

#                  debug         \
#                  benchmark  
#                  dialect       \
#                  element       \
#                  error         \
#                  hemp          \
#                  hub          \
#                  module        \
#                  ptree         \
#                  scanner       \
#                  filesystem    \
#                  template      \
#                  type          \
#                  value


TESTS           = $(check_PROGRAMS)
LDADD           = ../library/libhemp.la tap.o
CFLAGS          = -I../include
CPPFLAGS        = -DTESTDIR='"$(abs_top_srcdir)/tests"'

