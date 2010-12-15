#ifndef HEMP_URI_H
#define HEMP_URI_H

#include <hemp/core.h>

struct hemp_uri {
    hemp_string     uri;
    hemp_string     buffer;
    hemp_string     scheme;
    hemp_string     user;
    hemp_string     host;
    hemp_string     port;
    hemp_string     path;
    hemp_string     query;
    hemp_string     fragment;
    hemp_list       paths;
    hemp_hash       params;
};


HEMP_INLINE hemp_uri
hemp_uri_init(
    hemp_uri        uri
);

HEMP_INLINE hemp_uri
hemp_uri_from_string(
    hemp_string     string
);

HEMP_INLINE hemp_uri
hemp_uri_relative_string(
    hemp_uri        base,
    hemp_string     rel
);

HEMP_INLINE hemp_uri
hemp_uri_wipe(
    hemp_uri        uri
);

HEMP_INLINE void
hemp_uri_release(
    hemp_uri        uri
);

HEMP_INLINE void
hemp_uri_free(
    hemp_uri        uri
);

hemp_bool
hemp_uri_split(
    hemp_uri        uri,
    hemp_string     text
);

hemp_bool
hemp_uri_split_path(
    hemp_uri        uri,
    hemp_string   * bufpos
);

hemp_bool
hemp_uri_split_query(
    hemp_uri        uri,
    hemp_string   * bufpos
);


#define HEMP_URI_MATCHER(f)         \
    HEMP_INLINE hemp_bool f(        \
        hemp_uri        uri,        \
        hemp_string   * srcpos,     \
        hemp_string   * bufpos      \
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


#define hemp_uri_new()  \
    hemp_uri_init(NULL)

#define hemp_uri_string(s)  \
    hemp_uri_split( hemp_uri_init(NULL), s )

#endif /* HEMP_URI_H */