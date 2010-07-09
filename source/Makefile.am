lib_LTLIBRARIES     = libhemp.la
libhemp_la_SOURCES  = memory.c   \
                      slab.c     \
                      pool.c     \
                      hash.c     \
                      ptree.c    \
                      element.c  \
                      elements.c \
                      tags.c     \
                      sources.c  \
                      dialect.c  \
                      template.c \
                      scanner.c  \
                      debug.c    \
                      hemp_obj.c 

bin_PROGRAMS        = hemp

hemp_SOURCES        = hemp.c
hemp_LDADD          = libhemp.la

