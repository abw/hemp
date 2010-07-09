lib_LTLIBRARIES     = libhemp.la
libhemp_la_SOURCES  = dialect.c  \
                      debug.c    \
                      element.c  \
                      elements.c \
                      hash.c     \
                      hemp_obj.c \
                      memory.c   \
                      module.c   \
                      pool.c     \
                      ptree.c    \
                      scanner.c  \
                      slab.c     \
                      sources.c  \
                      tags.c     \
                      template.c 

bin_PROGRAMS        = hemp

hemp_SOURCES        = hemp.c
hemp_LDADD          = libhemp.la

