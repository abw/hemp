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
#                 tests

TESTS = $(check_PROGRAMS)

# I bet there's some really easy way to set these values for all files, 
# but I'm a complete autoconf/automake noob, so I'm just sticking with the
# tediously repetitive way for now

debug_SOURCES       = debug.c tap.c
debug_LDADD         = ../source/libhemp.la

dialect_SOURCES     = dialect.c tap.c
dialect_LDADD       = ../source/libhemp.la

element_SOURCES     = element.c tap.c
element_LDADD       = ../source/libhemp.la

error_SOURCES       = error.c tap.c
error_LDADD         = ../source/libhemp.la

hash_SOURCES        = hash.c tap.c
hash_LDADD          = ../source/libhemp.la

hemp_SOURCES        = hemp.c tap.c
hemp_LDADD          = ../source/libhemp.la

list_SOURCES        = list.c tap.c
list_LDADD          = ../source/libhemp.la

memory_SOURCES      = memory.c tap.c
memory_LDADD        = ../source/libhemp.la

module_SOURCES      = module.c tap.c
module_LDADD        = ../source/libhemp.la

pool_SOURCES        = pool.c tap.c
pool_LDADD          = ../source/libhemp.la

scanner_SOURCES     = scanner.c tap.c
scanner_LDADD       = ../source/libhemp.la
scanner_CPPFLAGS    = -DTESTDIR='"$(abs_top_srcdir)/tests"'

slab_SOURCES        = slab.c tap.c
slab_LDADD          = ../source/libhemp.la

sources_SOURCES     = sources.c tap.c
sources_LDADD       = ../source/libhemp.la

filesystem_SOURCES  = filesystem.c tap.c 
filesystem_LDADD    = ../source/libhemp.la
filesystem_CPPFLAGS = -DTESTDIR='"$(abs_top_srcdir)/tests"'


template_SOURCES    = template.c tap.c
template_LDADD      = ../source/libhemp.la

text_SOURCES        = text.c tap.c
text_LDADD          = ../source/libhemp.la

#tests_SOURCES      = tests.c tap.c
#tests_LDADD        = ../source/libhemp.la


