lib_LTLIBRARIES     = libhemp.la
libhemp_la_SOURCES  = cstr.c                \
                      dialect.c             \
                      debug.c               \
                      element.c             \
                      elements.c            \
                      element/dquote.c      \
                      element/integer.c     \
                      element/number.c      \
                      element/squote.c      \
                      element/text.c        \
                      filesystem.c          \
                      hash.c                \
                      hemp_obj.c            \
                      list.c                \
                      memory.c              \
                      module.c              \
                      pool.c                \
                      ptree.c               \
                      scanner.c             \
                      slab.c                \
                      sources.c             \
                      tags.c                \
                      tag/comment.c         \
                      tag/control.c         \
                      tag/embed.c           \
                      tag/inline.c          \
                      tag/outline.c         \
                      tag/variable.c        \
                      template.c            \
                      utils.c

bin_PROGRAMS        = hemp

hemp_SOURCES        = hemp.c
hemp_LDADD          = libhemp.la


