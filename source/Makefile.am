lib_LIBRARIES       = libhemp.a
libhemp_a_SOURCES   = memory.c   \
                      slab.c     \
                      pool.c     \
                      ptree.c    \
                      element.c  \
                      elements.c \
                      tags.c     \
                      sources.c  \
                      scanner.c  \
                      template.c \
                      debug.c    \
                      test.c

bin_PROGRAMS        = hemp hemp_test
hemp_SOURCES        = hemp.c
hemp_LDADD          = libhemp.a
hemp_test_SOURCES   = hemp_test.c
hemp_test_LDADD     = libhemp.a
     
