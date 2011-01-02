#ifndef HEMP_URI_H
#define HEMP_URI_H

#include <hemp/core.h>

struct hemp_uri {
    HempString     uri;
    HempString     buffer;
    HempString     scheme;
    HempString     authority;
    HempString     user;
    HempString     host;
    HempString     port;
    HempString     path;
    HempString     query;
    HempString     fragment;
    HempList       paths;
    HempHash       params;
};


HEMP_INLINE HempUri
hemp_uri_init(
    HempUri        uri
);

HEMP_INLINE HempUri
hemp_uri_from_string(
    HempString     string
);

HEMP_INLINE void
hemp_uri_copy_authority(
    HempUri from,
    HempUri to
);

HEMP_INLINE HempUri
hemp_uri_relative_string(
    HempUri        base,
    HempString     rel
);

HEMP_INLINE HempUri
hemp_uri_relative_uri(
    HempUri        base,
    HempUri        rel
);

void 
hemp_uri_buffer(
    HempUri uri
);

HEMP_INLINE HempUri
hemp_uri_wipe(
    HempUri        uri
);

HEMP_INLINE void
hemp_uri_release(
    HempUri        uri
);

HEMP_INLINE void
hemp_uri_free(
    HempUri        uri
);

HempBool
hemp_uri_split(
    HempUri        uri,
    HempString     text
);

HempBool
hemp_uri_split_path(
    HempUri        uri,
    HempString   * bufpos
);

HempBool
hemp_uri_split_query(
    HempUri        uri,
    HempString   * bufpos
);

HempString
hemp_uri_join(
    HempUri uri
);


HEMP_INLINE HempBool
hemp_uri_schemes_equal(
    HempUri base,
    HempUri rel
);

HempString
hemp_uri_path_relative(
    HempString base,
    HempString rel
);

HempString
hemp_uri_path_join(
    HempString base,
    HempString rel,
    HempBool   is_dir
);

HempString
hemp_uri_merge_paths(
    HempUri    base,
    HempUri    rel
);

HempString
hemp_uri_collapse_path(
    HempString path
);



#define HEMP_URI_MATCHER(f)         \
    HEMP_INLINE HempBool f(        \
        HempUri        uri,        \
        HempString   * srcpos,     \
        HempString   * bufpos      \
    )

#define HEMP_URI_MATCHED(item)      \
    uri->item = *bufpos;            \
    *buffer++ = HEMP_NUL;           \
    *bufpos   = buffer;             \
    *srcpos   = source;


HEMP_URI_MATCHER(hemp_uri_match_scheme);
HEMP_URI_MATCHER(hemp_uri_match_authority);
HEMP_URI_MATCHER(hemp_uri_match_user);
HEMP_URI_MATCHER(hemp_uri_match_host);
HEMP_URI_MATCHER(hemp_uri_match_port);
HEMP_URI_MATCHER(hemp_uri_match_path);
HEMP_URI_MATCHER(hemp_uri_match_query);
HEMP_URI_MATCHER(hemp_uri_match_fragment);


#define hemp_uri_new()                  \
    hemp_uri_init(NULL)

#define hemp_uri_string(s)              \
    hemp_uri_split( hemp_uri_init(NULL), s )

#define hemp_uri_has_path(u)            \
    (u->path && *(u->path))

#define hemp_uri_no_path(u)             \
    (! hemp_uri_has_path(u))

#define hemp_uri_path_is_absolute(u)    \
    (u->path && *(u->path) == '/')


#endif /* HEMP_URI_H */