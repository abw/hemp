#include <hemp/uri.h>



HEMP_INLINE hemp_uri
hemp_uri_init(
    hemp_uri uri
) {
    HEMP_INSTANCE(uri);
    hemp_uri_wipe(uri);
    return uri;
}


HEMP_INLINE hemp_uri
hemp_uri_wipe(
    hemp_uri uri
) {
    uri->uri        = NULL;
    uri->buffer     = NULL;
    uri->scheme     = NULL;
    uri->user       = NULL;
    uri->host       = NULL;
    uri->port       = NULL;
    uri->path       = NULL;
    uri->query      = NULL;
    uri->fragment   = NULL;
    uri->params     = NULL;
    return uri;
}


HEMP_INLINE void
hemp_uri_release(
    hemp_uri uri
) {
    if (uri->uri)
        hemp_mem_free(uri->uri);

    if (uri->buffer)
        hemp_mem_free(uri->buffer);

    if (uri->params)
        hemp_hash_free(uri->params);

    hemp_uri_wipe(uri);
}


HEMP_INLINE void
hemp_uri_free(
    hemp_uri uri
) {
    hemp_uri_release(uri);
    hemp_mem_free(uri);
}


/* This implements a quick-and-dirty URI splitter based on the Badger::URL
 * Perl module.  It splits well-formed URIs but does NOT validate them in 
 * any way.  It identifies the  basic components of URL-like syntaxes: 
 * scheme, user, host, port, path, query and fragment by splitting the URI on 
 * the delimiting characters ':', '/', '@', '?' and '#'.
 *
 *     scheme://user@host:port/path?query#fragment
 *
 * See http://labs.apache.org/webarch/uri/rfc/rfc3986.html#regexp for an 
 * overview of the algorithm in use.
 *
 * The path component is greedy and will consume all characters in the 
 * absence of any recognised delimiters.  e.g. "*" is considered a "valid"
 * URI path. The query part is also greedy up to any '#' character.  The 
 * fragment greedily consumes the remainder of the URI.  This needs to be 
 * improved so that we can incrementally match unquoted URIs in templates.
 */

hemp_bool
hemp_uri_split(
    hemp_uri    uri,
    hemp_string text
) {
    /* clean up any previous data */
    if (uri->buffer)
        hemp_uri_release(uri);

    /* Allocate a new string buffer long enough for the URI and some extra
     * NUL characters to place between the URI tokens.  This allows us to 
     * allocate all the memory in one go rather than doing 9 separate 
     * allocations and subsequent de-allocations. We only need the buffer to
     * be the same length as the  original as we don't need to preserve the 
     * separators and can replace then with NUL characters.
     */
     /* come to think of it, we might as well dup the original string and
      * then insert NUL chars in the right place.  If we allocate x2 the 
      * length then we've got extra room for a buffer for parameters.
      */
    hemp_string source = uri->uri    = hemp_string_clone(text, "uri");
    hemp_string buffer = uri->buffer = (hemp_string) hemp_mem_alloc( 
        strlen(text)
    );

    if (! buffer)
        hemp_mem_fail("uri buffer");

    /* Note that the path is greedy any will consume all characters, so 
     * there's no such thing as an invalid URI as far as we're concerned.
     * That's not to say that all the characters in the URI are valid
     * characters...
     */
    hemp_uri_match_scheme(uri, &source, &buffer);
    hemp_uri_match_authority(uri, &source, &buffer);
    hemp_uri_match_path(uri, &source, &buffer);
    hemp_uri_match_query(uri, &source, &buffer);
    hemp_uri_match_fragment(uri, &source, &buffer);

    return HEMP_TRUE;
}


HEMP_URI_MATCHER(hemp_uri_match_scheme) {
    hemp_string     source = *srcpos;
    hemp_string     buffer = *bufpos;
    hemp_bool       ok     = HEMP_FALSE;

    hemp_debug_msg("matching URI scheme: %s\n", source);

    if (isalpha(*source)) {
        *buffer++ = *source++;
        ok        = HEMP_TRUE;

        while (*source && ok) {
            switch (*source) {
                case '.':
                case '-':
                case '+':
                    /* these are all legal characters */
                    *buffer++ = *source++;
                    break;
                    
                default:
                    if (isalnum(*source))
                        *buffer++ = *source++;
                    else
                        ok = HEMP_FALSE;
            }
        }

        /* the only valid reason to exit the above loop (or bypass it 
         * altogether in the degenerate case) is when the next character 
         * is ':', otherwise we haven't matched a proper scheme
         */
        ok = (*source == ':');
    }

    if (ok) {
        source++;
        HEMP_URI_MATCHED(scheme);
        return HEMP_TRUE;
    }

    return HEMP_FALSE;
}


HEMP_URI_MATCHER(hemp_uri_match_authority) {
    hemp_string source = *srcpos;

    hemp_debug_msg("matching URI authority: %s\n", source);

    if (strncmp(source, "//", 2) == 0) {
        *srcpos = source + 2;
        hemp_uri_match_user(uri, srcpos, bufpos);
        hemp_uri_match_host(uri, srcpos, bufpos);
        hemp_uri_match_port(uri, srcpos, bufpos);
        return HEMP_TRUE;
    }

    return HEMP_FALSE;
}


HEMP_URI_MATCHER(hemp_uri_match_user) {
    hemp_string     source = *srcpos;
    hemp_string     buffer = *bufpos;

    hemp_debug_msg("matching user at %s\n", source);

    while (*source && *source != '@') {
        *buffer++ = *source++;
    }
    if (*source == '@') {
        source++;
        HEMP_URI_MATCHED(user);
        return HEMP_TRUE;
    }

    return HEMP_FALSE;
}


HEMP_URI_MATCHER(hemp_uri_match_host) {
    hemp_string     source = *srcpos;
    hemp_string     buffer = *bufpos;

    hemp_debug_msg("matching URI host: %s\n", source);

    while (*source && *source != ':' && *source != '/') {
        *buffer++ = *source++;
    }
    if (buffer != *bufpos) {
        HEMP_URI_MATCHED(host);
        return HEMP_TRUE;
    }

    return HEMP_FALSE;
}


HEMP_URI_MATCHER(hemp_uri_match_port) {
    hemp_string     source = *srcpos;
    hemp_string     buffer = *bufpos;

    hemp_debug_msg("matching URI port: %s\n", source);

    if (*source && *source == ':') {
        source++;
        
        while (*source && isnumber(*source)) {
            *buffer++ = *source++;
        }
    }

    if (buffer != *bufpos) {
        HEMP_URI_MATCHED(port);
        return HEMP_TRUE;
    }
    
    return HEMP_FALSE;
}


HEMP_URI_MATCHER(hemp_uri_match_path) {
    hemp_string     source = *srcpos;
    hemp_string     buffer = *bufpos;

    hemp_debug_msg("matching URI path: %s\n", source);

    while (*source && *source != '?' && *source != '#') {
        *buffer++ = *source++;
    }

    if (buffer != *bufpos) {
        HEMP_URI_MATCHED(path);
        return HEMP_TRUE;
    }

    return HEMP_FALSE;
}


HEMP_URI_MATCHER(hemp_uri_match_query) {
    hemp_string     source = *srcpos;
    hemp_string     buffer = *bufpos;

    hemp_debug_msg("matching URI query: %s\n", source);

    if (*source && *source == '?') {
        source++;
        
        while (*source && *source != '#') {
            *buffer++ = *source++;
        }
    }

    if (buffer != *bufpos) {
        HEMP_URI_MATCHED(query);
        hemp_uri_split_query(uri);
        return HEMP_TRUE;
    }

    return HEMP_FALSE;
}


HEMP_URI_MATCHER(hemp_uri_match_fragment) {
    hemp_string     source = *srcpos;
    hemp_string     buffer = *bufpos;

    hemp_debug_msg("matching URI fragment: %s\n", source);

    if (*source && *source == '#') {
        source++;
        
        while (*source) {
            *buffer++ = *source++;
        }
    }

    if (buffer != *bufpos) {
        HEMP_URI_MATCHED(fragment);
        return HEMP_TRUE;
    }

    return HEMP_FALSE;
}


hemp_bool
hemp_uri_split_query(
    hemp_uri    uri
) {
    hemp_string query = uri->query;
    hemp_hash   params;
    hemp_string key, value;

    if (uri->params)
        hemp_hash_free(uri->params);
    
    params = hemp_hash_new();

    uri->params = params;

    while (*query) {
        key = query;
        while (*query && *query != '=') {
            query++;
        }
        if (key == query) {
            // TODO: proper error handling
            hemp_fatal("Missing key in URI query: %s\n", key);
        }
        else if (*query != '=') {
            hemp_fatal("Missing '=' in URI query: %s\n", key);
        }

        // Maybe we should delay parsing params until something asks for them
        // at which point we create a hash and then effectively invalidate the 
        // query string.  Then we re-generate the query string on demand from
        // the hash as required.  Or just dup the query string onto the end of
        // the buffer and use that...
        *query++ = HEMP_NUL;        // FIXME: buggers up original query string
        value = query;

        while (*query && *query != '&' && *query != ';') {
            query++;
        }
        
        *query = HEMP_NUL;
        hemp_debug_msg("parsed param: [%s] => [%s]\n", key, value);
    }

    return HEMP_TRUE;
}
