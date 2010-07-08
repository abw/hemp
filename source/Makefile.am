lib_LTLIBRARIES     = libhemp.la
libhemp_la_SOURCES  = memory.c   \
                      slab.c     \
                      pool.c     \
                      ptree.c    \
                      element.c  \
                      elements.c \
                      tags.c     \
                      sources.c  \
                      scanner.c  \
                      debug.c    \
                      test.c

bin_PROGRAMS        = hemp hemp_test
hemp_SOURCES        = hemp.c
hemp_test_SOURCES   = hemp_test.c
hemp_test_LDADD     = libhemp.la
     
