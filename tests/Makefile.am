check_PROGRAMS  =  scanner 

#                  debug         \
#                  benchmark  
#                  dialect       \
#                  element       \
#                  error         \
#                  hash          \
#                  hemp          \
#                  list          \
#                  memory        \
#                  module        \
#                  pool          \
#                  ptree         \
#                  scanner       \
#                  slab          \
#                  sources       \
#                  filesystem    \
#                  template      \
#                  type          \
#                  text


TESTS           = $(check_PROGRAMS)
LDADD           = ../library/libhemp.la tap.o
CPPFLAGS        = -DTESTDIR='"$(abs_top_srcdir)/tests"'

