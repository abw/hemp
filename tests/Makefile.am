check_PROGRAMS  = debug         \
                  dialect       \
                  element       \
                  error         \
                  hash          \
                  hemp          \
                  list          \
                  memory        \
                  module        \
                  pool          \
                  scanner       \
                  slab          \
                  sources       \
                  filesystem    \
                  template      \
                  text

TESTS           = $(check_PROGRAMS)
LDADD           = ../lib/libhemp.la tap.o
CPPFLAGS        = -DTESTDIR='"$(abs_top_srcdir)/tests"'

