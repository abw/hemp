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
hemp_uri_copy(
    hemp_uri src
) {
    /* HMMM... no, this isn't right... not used */
    hemp_uri uri;
    HEMP_ALLOCATE(uri);
    hemp_mem_copy(src, uri, sizeof(struct hemp_uri));

    /* copy is *NOT* reponsible for buffer */
    uri->buffer = NULL;

    return uri;
}


HEMP_INLINE void
hemp_uri_copy_authority(
    hemp_uri from,
    hemp_uri to
) {
    to->authority   = from->authority;
    to->user        = from->user;
    to->host        = from->host;
    to->port        = from->port;
}


HEMP_INLINE hemp_uri
hemp_uri_from_string(
    hemp_string string
) {
    hemp_uri uri = hemp_uri_new();
    hemp_uri_split(uri, string);
    return uri;
}


HEMP_INLINE hemp_uri
hemp_uri_wipe(
    hemp_uri uri
) {
    uri->uri        = NULL;
    uri->buffer     = NULL;
    uri->scheme     = NULL;
    uri->authority  = NULL;
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
        uri->authority = *srcpos;       // FIXME
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
        
        while (*source && isdigit(*source)) {
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
 * Join
 *--------------------------------------------------------------------------*/

hemp_string
hemp_uri_path_relative(
    hemp_string base,
    hemp_string rel
) {
    hemp_size   baselen = 0;
    hemp_string merged  = NULL;
    hemp_string slash   = NULL;
    hemp_string src     = rel;
    hemp_string dst;
    hemp_char   next;

    /* If the relative path is absolute (starts: /) then we can ignore the
     * base path altogether.  Otherwise we can ignore everything after the
     * final slash in the base path
     */
    if (*rel != '/' && (slash = strrchr(base, '/'))) {
        slash++;
        baselen = slash - base;
    }

    merged  = hemp_mem_alloc(baselen + strlen(rel) + 1);
    *merged = HEMP_NUL;     /* for debugging, so we can print string */

    if (! merged)
        hemp_mem_fail("URI path");

    if (baselen) {
        strncpy(merged, base, baselen);
        dst = merged + baselen;
        *dst = HEMP_NUL;
    }
    else {
        dst = merged;
    }

    while (*src) {
        switch (*src) {
            case '/':
                dst = merged;
                *dst++ = *src++;
                *dst = HEMP_NUL;
                break;

            case '.':
                next = *(src + 1);

                if (next == HEMP_NUL || next == '/') {
                    /* ignore './' or final '.' */
                    src += next ? 2 : 1;
                    break;
                }
                else if (next == '.') {
                    next = *(src + 2);

                    if (next == HEMP_NUL || next == '/') {
                        /* found '../' or final '..' */
                        src += next ? 3 : 2;

                        /* rewind to find the last slash in the destination */
                        while (dst > merged && *dst != '/') {
                            dst--;
                        }

                        /* if the last slash is the final character and not
                         * also the first (i.e. not '/') then walk back to
                         * the previous '/'
                         */
                        if (dst > merged && *dst == '/' && ! *(dst + 1)) {
                            do {
                                dst--;
                            }
                            while (dst > merged && *dst != '/');

                            /* start copying again after the slash */
                            if (*dst == '/')
                                dst++;
                            *dst = HEMP_NUL;
                        }
                        else {
                            dst++;
                            *dst = HEMP_NUL;
                        }
                        break;
                    }
                }

                /* drop through to default case */

            default:
                /* copy everything up to and including the next '/' (or NUL) */
                do {
                    *dst++ = *src;
                }
                while (*src && *src++ != '/');
                *dst = HEMP_NUL;
        }
    }
    *dst = HEMP_NUL;

    return merged;
}



#define hemp_uri_strcpy(src, srcpos, endpos) ({     \
    srcpos = src;                                   \
    while (*srcpos)                                 \
        *endpos++ = *srcpos++;                      \
})

#define hemp_uri_bufsave(dest, bufpos, endpos) ({   \
    *endpos++ = HEMP_NUL;                           \
    dest = bufpos;                                  \
    bufpos = endpos;                                \
})


hemp_string 
hemp_uri_join(
    hemp_uri uri
) {
    hemp_string buffer;
    hemp_string endpos;
    hemp_string srcpos;
    hemp_string bufpos;
    hemp_size   len = 0;

    len += uri->scheme ? strlen(uri->scheme)     + 1 : 0;   /* +1 for ':'   */

    if (uri->authority) {
        len += 2;                                           /* +2 for '//'  */
        len += uri->user ? strlen(uri->user)     + 1 : 0;   /* +1 for '@'   */
        len += uri->host ? strlen(uri->host)         : 0;
        len += uri->port ? strlen(uri->port)     + 1 : 0;   /* +1 for ':'   */
    }

    len += uri->path     ? strlen(uri->path)         : 0;
    len += uri->query    ? strlen(uri->query)    + 1 : 0;   /* +1 for '?'   */
    len += uri->fragment ? strlen(uri->fragment) + 1 : 0;   /* +1 for '#'   */

    len++;      /* don't forget the terminating NUL */

//  hemp_debug_msg("computed length: %d\n", len);

    buffer = hemp_mem_alloc(len);
    endpos = buffer;

    if (! buffer)
        hemp_mem_fail("URI buffer");

    if (uri->scheme) {
        hemp_uri_strcpy(uri->scheme, srcpos, endpos);
        *endpos++ = ':';
    }

    if (uri->authority) {
        hemp_uri_strcpy("//", srcpos, endpos);

        if (uri->user) {
            hemp_uri_strcpy(uri->user, srcpos, endpos);
            *endpos++ = '@';
        }
        if (uri->host) {
            hemp_uri_strcpy(uri->host, srcpos, endpos);
        }
        if (uri->port) {
            *endpos++ = ':';
            hemp_uri_strcpy(uri->port, srcpos, endpos);
        }
    }

    /* there should always be a path, even if it's zero length, but let's
     * play it safe, just in case....
     */
    if (uri->path) {
        hemp_uri_strcpy(uri->path, srcpos, endpos);
    }

    if (uri->query) {
        *endpos++ = '?';
        hemp_uri_strcpy(uri->query, srcpos, endpos);
    }

    if (uri->fragment) {
        *endpos++ = '#';
        hemp_uri_strcpy(uri->fragment, srcpos, endpos);
    }

    if (uri->uri)
        hemp_mem_free(uri->uri);

    *endpos++ = HEMP_NUL;
    uri->uri  = buffer;

    /* Now do it all over again to generate the individual components. */
    /* This is possibly silly, but I'm relying on it for testing ATM   */
    buffer = hemp_mem_alloc(len);
    endpos = buffer;
    bufpos = buffer;

    if (! buffer)
        hemp_mem_fail("URI buffer");

    if (uri->scheme) {
        hemp_uri_strcpy(uri->scheme, srcpos, endpos);
        hemp_uri_bufsave(uri->scheme, bufpos, endpos);
    }

    if (uri->authority) {
        if (uri->user) {
            hemp_uri_strcpy(uri->user, srcpos, endpos);
            hemp_uri_bufsave(uri->user, bufpos, endpos);
        }
        if (uri->host) {
            hemp_uri_strcpy(uri->host, srcpos, endpos);
            hemp_uri_bufsave(uri->host, bufpos, endpos);
        }
        if (uri->port) {
            hemp_uri_strcpy(uri->port, srcpos, endpos);
            hemp_uri_bufsave(uri->port, bufpos, endpos);
        }
    }

    if (uri->path) {
        hemp_uri_strcpy(uri->path, srcpos, endpos);
        hemp_uri_bufsave(uri->path, bufpos, endpos);
    }

    if (uri->query) {
        hemp_uri_strcpy(uri->query, srcpos, endpos);
        hemp_uri_bufsave(uri->query, bufpos, endpos);
    }

    if (uri->fragment) {
        hemp_uri_strcpy(uri->fragment, srcpos, endpos);
        hemp_uri_bufsave(uri->fragment, bufpos, endpos);
    }

    if (uri->buffer)
        hemp_mem_free(uri->buffer);

    *endpos++   = HEMP_NUL;
    uri->buffer = buffer;

    return uri->uri;
}



/*--------------------------------------------------------------------------
 * relative paths
 *
 * This is a big hack, thrown together quickly to get something working.
 * Needs a proper clean up.
 *--------------------------------------------------------------------------*/



HEMP_INLINE hemp_uri
hemp_uri_relative_string(
    hemp_uri        base,
    hemp_string     relstr
) {
    hemp_uri        rel   = hemp_uri_from_string(relstr);
    hemp_uri        uri   = hemp_uri_relative_uri(base, rel);
    hemp_uri_free(rel);
    return uri;
}


HEMP_INLINE hemp_uri
hemp_uri_relative_uri(
    hemp_uri        base,
    hemp_uri        rel
) {
    hemp_uri        uri    = hemp_uri_new();
    hemp_string     scheme = NULL;
    hemp_string     path   = NULL;

    if (! hemp_uri_schemes_equal(base, rel)) {
        scheme = rel->scheme;
    }

    if (scheme) {
        hemp_uri_copy_authority(rel, uri);
        uri->scheme = scheme;
        uri->path   = path = hemp_uri_collapse_path(rel->path);
        uri->query  = rel->query;
    }
    else {
        if (rel->authority) {
            hemp_uri_copy_authority(rel, uri);
            uri->path   = path = hemp_uri_collapse_path(rel->path);
            uri->query  = rel->query;
        }
        else {
            if (hemp_uri_no_path(rel)) {
                uri->path  = base->path;  
                uri->query = rel->query
                    ? rel->query
                    : base->query;
            }
            else {
                if (hemp_uri_path_is_absolute(rel)) {
                    uri->path = path = hemp_uri_collapse_path(rel->path);
                }
                else {
                    uri->path = path = hemp_uri_merge_paths(base, rel);
                    uri->path = hemp_uri_collapse_path(uri->path);
                    hemp_mem_free(path);
                    path = uri->path;
                }
                uri->query = rel->query;
            }
            hemp_uri_copy_authority(base, uri);
            // uri->authority = base->authority;
        }
        uri->scheme = base->scheme;
    }
    uri->fragment = rel->fragment;

    /* we now need to create a new memory buffer into which we can copy
     * the various elements, most of which are pointers to elements in the
     * base or relative uris that we don't own
     */
    hemp_uri_join(uri);

    if (path) {
        hemp_mem_free(path);
    }

    return uri;
}


HEMP_INLINE hemp_bool
hemp_uri_schemes_equal(
    hemp_uri base,
    hemp_uri rel
) {
    if (base->scheme && rel->scheme) {
        /* if both are defined then the string values should match */
        return hemp_string_eq(base->scheme, rel->scheme);
    }
    else if (base->scheme || rel->scheme) {
        /* if only one is defined then they're different */
        return HEMP_FALSE;
    }
    else {
        /* if both are undefined then they're the same */
        return HEMP_TRUE;
    }
}


hemp_string
hemp_uri_merge_paths(
    hemp_uri    base,
    hemp_uri    rel
) {
    hemp_string merged  = NULL;
    hemp_size   baselen = 0;
    hemp_string slash;

    /* From rfc 3986:
     *
     * If the base URI has a defined authority component and an empty path, 
     * then return a string consisting of "/" concatenated with the 
     * reference's path; 
     * 
     * Otherwise, return a string consisting of the reference's path 
     * component appended to all but the last segment of the base URI's path 
     * (i.e., excluding any characters after the right-most "/" in the base 
     * URI path, or excluding the entire base URI path if it does not contain 
     * any "/" characters). 
     */

//  hemp_debug_msg("[%s] + [%s]\n", base->path, rel->path);

    if (base->authority && hemp_uri_no_path(base)) {
        merged = hemp_mem_alloc(strlen(rel->path) + 2);
        if (! merged) 
            hemp_mem_fail("URI path");

        strcpy(merged, "/");
        strcat(merged, rel->path);
    }
    else {
        slash = strrchr(base->path, '/');

        if (slash) {
            slash++;
            baselen = slash - base->path;
        }
        
        merged = hemp_mem_alloc(baselen + strlen(rel->path) + 1);
        if (! merged) 
            hemp_mem_fail("URI path");

        if (baselen) {
            strncpy(merged, base->path, baselen);
            strcpy(merged + baselen, rel->path);
        }
        else {
            strcpy(merged, rel->path);
        }
    }

    return merged;
}


hemp_string
hemp_uri_collapse_path(
    hemp_string path
) {
    hemp_string collapsed = hemp_mem_alloc(strlen(path) + 1);
    hemp_string src       = path;
    hemp_string dst       = collapsed;
    hemp_string slash     = NULL;
    
    if (! collapsed)
        hemp_mem_fail("URI path");

//  hemp_debug_msg("collapsing: %s\n", path);

    /* From rfc 3986:
     *
     * While the input buffer is not empty, loop as follows:
     * A If the input buffer begins with a prefix of "../" or "./",
     *   then remove that prefix from the input buffer; otherwise,
     * B if the input buffer begins with a prefix of "/./" or "/.",
     *   where "." is a complete path segment, then replace that prefix 
     *   with "/" in the input buffer; otherwise,
     * C if the input buffer begins with a prefix of "/../" or "/..",
     *   where ".." is a complete path segment, then replace that prefix 
     *   with "/" in the input buffer and remove the last segment and 
     *   its preceding "/" (if any) from the output buffer; otherwise,
     * D if the input buffer consists only of "." or "..", then remove
     *   that from the input buffer; otherwise,
     * E move the first path segment in the input buffer to the end of
     *   the output buffer, including the initial "/" character (if any) 
     *   and any subsequent characters up to, but not including, the next
     *   "/" character or the end of the input buffer.
     */
    while (*src) {
        if (strncmp(src, "./",  2) == 0) {
//          hemp_debug_msg("skipping ./\n");
            src += 2;
        }
        else if (strncmp(src, "../", 3) == 0) {
//          hemp_debug_msg("skipping ../\n");
            src += 3;
        }
        else if (strncmp(src, "/./", 3) == 0) {
//          hemp_debug_msg("munging /./ => /\n");
            src += 2;       /* move onto second slash */
        }
        else if (strcmp(src, "/.") == 0) {
//          hemp_debug_msg("munging /. => /\n");
            src += 2;
            *dst++ = '/';
        }
        else if (strncmp(src, "/../", 4) == 0) {
//          hemp_debug_msg("upping /../\n");
            src += 3;
            /* remove last element and '/' on output buffer */
            /* arse... must walk back to find preceding slash */
            if (slash) {
                dst   = slash;
                *dst  = HEMP_NUL;
                slash = strrchr(collapsed, '/');
            }
            else {
                dst = collapsed;
            }
        }
        else if (strcmp(src, "/..") == 0) {
//          hemp_debug_msg("upping /..\n");
            src += 3;
            /* remove last element and add traing slash */
            dst = slash
                ? slash
                : collapsed;
            *dst++ = '/';
        }
        else if (strcmp(src, ".") == 0) {
//          hemp_debug_msg("skipping .\n");
            src += 1;
        }
        else if (strcmp(src, "..") == 0) {
//          hemp_debug_msg("skipping ..\n");
            src += 2;
        }
        else {
//          hemp_debug_msg("copy next segment: %s\n", src);
            if (*src == '/') {
                slash  = dst;
                *dst++ = *src++;
            }
            while (*src && *src != '/') {
                *dst++ = *src++;
            }
        }
//        *dst = HEMP_NUL;
//        hemp_debug_msg("<< SRC: %s\n", src);
//        hemp_debug_msg(">> DST: %s\n", collapsed);
    }
    *dst = HEMP_NUL;

//  hemp_debug_msg("collapsed: %s\n", collapsed);
    
    return collapsed;
}


/*--------------------------------------------------------------------------
 * NOTES
 *
 * I think it's probably sufficient (for now at least) to split a well-formed
 * URI into component parts rather than attempting a complete parse.  If we
 * go down the latter route then it would be better to use an existing URI
 * parsing library (e.g. liburiparse).  It would be nice to be able to parse
 * unquoted URIs in documents and intelligently detect the end of the URI,
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

