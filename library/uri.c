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
    uri->paths      = NULL;
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

    if (uri->paths)
        hemp_list_free(uri->paths);

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


/*--------------------------------------------------------------------------
 * This implements a quick-and-dirty URI splitter based on the Badger::URL
 * Perl module.  It splits well-formed URIs but does NOT validate them in 
 * any way.  It identifies the  basic components of URL-like syntaxes: 
 * scheme, user, host, port, path, query and fragment by splitting the URI on 
 * the delimiting characters ':', '/', '@', '?' and '#'.
 *
 *   foo://example.com:8042/over/there?name=ferret#nose
 *   \_/   \______________/\_________/ \_________/ \__/
 *    |           |            |            |        |
 * scheme     authority       path        query   fragment
 *    |   _____________________|__
 *   / \ /                        \
 *   urn:example:animal:ferret:nose
 *
 * See http://labs.apache.org/webarch/uri/rfc/rfc3986.html#regexp for an 
 * overview of the algorithm in use.
 *
 * The path component is greedy and will consume all characters in the 
 * absence of any recognised delimiters.  e.g. "*" is considered a "valid"
 * URI path. The query part is also greedy up to any '#' character.  The 
 * fragment greedily consumes the remainder of the URI.  
 * 
 *--------------------------------------------------------------------------*/

hemp_bool
hemp_uri_split(
    hemp_uri    uri,
    hemp_string text
) {
    /* clean up any previous data */
    if (uri->buffer)
        hemp_uri_release(uri);

    /* We clone the original uri and allocate a new string buffer twice as 
     * long as the URI.  This gives us a buffer large enough to store all the 
     * component parts of the URI separated by NUL characters.  This allows 
     * us to allocate (and free) all the memory in one go instead of doing
     * it many times over.  Each part of the URI (scheme, user, host, port,
     * path, query, fragment) is copied into this buffer and the uri->XXX
     * pointers set to the appropriate offset.  The path is then copied into
     * the buffer, followed by the query string if present.  The original 
     * delimiters in the path and query string are then replaced with NUL 
     * NUL characters.  These packed strings are then used as the components
     * in the path list and the keys and values in the parameters hash. 
     */
    hemp_string source = uri->uri    = hemp_string_clone(text, "uri");
    hemp_string buffer = uri->buffer = (hemp_string) hemp_mem_alloc( 
        strlen(text) * 2 + 2    /* a couple extra to account for NULs */
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

    if (uri->path)
        hemp_uri_split_path(uri, &buffer);

    if (uri->query)
        hemp_uri_split_query(uri, &buffer);

    return HEMP_TRUE;
}


HEMP_URI_MATCHER(hemp_uri_match_scheme) {
    hemp_string     source = *srcpos;
    hemp_string     buffer = *bufpos;
    hemp_bool       ok     = HEMP_FALSE;

    hemp_debug_call("matching URI scheme: %s\n", source);

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

    hemp_debug_call("matching URI authority: %s\n", source);

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

    hemp_debug_call("matching user at %s\n", source);

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

    hemp_debug_call("matching URI host: %s\n", source);

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

    hemp_debug_call("matching URI port: %s\n", source);

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

    hemp_debug_call("matching URI path: %s\n", source);

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

    hemp_debug_call("matching URI query: %s\n", source);

    if (*source && *source == '?') {
        source++;
        
        while (*source && *source != '#') {
            *buffer++ = *source++;
        }
    }

    if (buffer != *bufpos) {
        HEMP_URI_MATCHED(query);
        return HEMP_TRUE;
    }

    return HEMP_FALSE;
}


HEMP_URI_MATCHER(hemp_uri_match_fragment) {
    hemp_string     source = *srcpos;
    hemp_string     buffer = *bufpos;

    hemp_debug_call("matching URI fragment: %s\n", source);

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
hemp_uri_split_path(
    hemp_uri        uri,
    hemp_string   * bufpos
) {
    hemp_string path   = uri->path;
    hemp_string buffer = *bufpos;
    hemp_string name;

    if (! path)
        return HEMP_FALSE;

    if (uri->paths)
        hemp_list_free(uri->paths);
    
    uri->paths = hemp_list_new();

    while (*path) {
        name = buffer;

        while (*path && *path != '/') {
            *buffer++ = *path++;
        }

        if (name == buffer) {
            /* we haven't moved so add an "empty" path entry */
//          hemp_debug_msg("found empty path\n");
            hemp_list_push(uri->paths, HempEmpty);
        }
        else {
            /* NUL terminate the copy in the buffer and add to list */
            *buffer++ = HEMP_NUL;
//          hemp_debug_msg("found path: [%s]\n", name);
            hemp_list_push(uri->paths, hemp_str_val(name));
        }

        if (*path)
            path++;

        name = buffer;
    }

    *bufpos = buffer;

    return HEMP_TRUE;
}


hemp_bool
hemp_uri_split_query(
    hemp_uri        uri,
    hemp_string   * bufpos
) {
    hemp_string query   = uri->query;
    hemp_string buffer  = *bufpos;
    hemp_string key, value;

    if (uri->params)
        hemp_hash_free(uri->params);
    
    uri->params = hemp_hash_new();

    while (*query) {
        key = buffer;

        while (*query && *query != '=') {
            *buffer++ = *query++;
        }

        if (key == buffer) {
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
        *buffer++ = HEMP_NUL;
        query++;
        value = buffer;

        while (*query && *query != '&' && *query != ';') {
            *buffer++ = *query++;
        }
        
        *buffer = HEMP_NUL;
        hemp_hash_store_string(uri->params, key, value);
//      hemp_debug_msg("parsed param: [%s] => [%s]\n", key, value);
    }

    *bufpos = buffer;

    return HEMP_TRUE;
}


/*--------------------------------------------------------------------------
 * NOTES
 *
 * I think it's probably sufficient (for now at least) to split a well-formed
 * URI into component parts rather than attempting a complete parse.  If we
 * go down the latter route then it would be better to use an existing URI
 * parsing library (e.g. liburiparse).  It would be nice to be able to parse
 * unquoted URIs in templates and intelligently detect the end of the URI,
 * but I don't think it's possible because so many different characters are
 * permitted in URIs.  e.g.  In "foo(file:blah)" the closing ')' is a valid 
 * path character in the file:blah) URI.  So I think here we have to mandate
 * the use of explit < > quote characters than can be pre-scanned to identify
 * the start and end of the uri, e.g. foo(<file:blah>).  In which case we can
 * be very liberal about what is accepted in a URI.
 *
 * Here's some relevant extracts from RFC3986 showing the URI syntax:
 *
 *   scheme      = ALPHA *( ALPHA / DIGIT / "+" / "-" / "." )
 *   authority   = [ userinfo "@" ] host [ ":" port ]
 *   userinfo    = *( unreserved / pct-encoded / sub-delims / ":" )
 *   host        = IP-literal / IPv4address / reg-name
 *   IP-literal  = "[" ( IPv6address / IPvFuture  ) "]"
 *   IPvFuture   = "v" 1*HEXDIG "." 1*( unreserved / sub-delims / ":" )
 *   IPv6address =  ...gnarly...
 *   IPv4address = dec-octet "." dec-octet "." dec-octet "." dec-octet
 *   reg-name    = *( unreserved / pct-encoded / sub-delims )
 *   port        = *DIGIT
 *   pchar       = unreserved / pct-encoded / sub-delims / ":" / "@"
 *   query       = *( pchar / "/" / "?" )
 *   fragment    = *( pchar / "/" / "?" )
 *   unreserved  = ALPHA / DIGIT / "-" / "." / "_" / "~"
 *   reserved    = gen-delims / sub-delims
 *   gen-delims  = ":" / "/" / "?" / "#" / "[" / "]" / "@"
 *   sub-delims  = "!" / "$" / "&" / "'" / "(" / ")" / "*" / "+" / "," / ";" / "="
 *   pct-encoded = "%" HEXDIG HEXDIG
 * 
 *--------------------------------------------------------------------------*/

