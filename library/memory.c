#include <hemp/memory.h>


/* hemp_mem_fail(item) 
 *
 * Raises a fatal error to report a memory allocation error.  The argument
 * is a char * string indicating the item being allocated for insertion 
 * into the HEMP_ERRMSG_MALLOC message format, e.g. "Failed to allocate
 * memory for a new %s"
 */

void hemp_mem_fail(
    hemp_cstr_p type
) {
    hemp_fatal(hemp_errmsg[HEMP_ERROR_MEMORY], type);
}



/* hemp_mem_copy_fn(src, dest, length) 
 *
 * Generic memory copy for systems (if there are any) that don't provide 
 * memmove() or bcopy().
 */

#ifdef HEMP_ADD_MEM_COPY

hemp_mem_p
hemp_mem_copy(
    hemp_mem_p  src,
    hemp_mem_p  dest,
    hemp_size_t len
) {
    hemp_size_t i;
    hemp_cstr_p s = (hemp_cstr_p) src;
    hemp_cstr_p d = (hemp_cstr_p) dest;

    if (d > s) {
        d += len;
        s += len;
        for (i = 0; i < len; ++i)
            *(--d) = *(--s);
        return d;
    }
    else {
        for (i = 0; i < len; ++i)
            *d++ = *s++;
        return d - len;
    }
}

#endif





/*--------------------------------------------------------------------------
 * Memory debugging functions
 *--------------------------------------------------------------------------*/

#if HEMP_DEBUG & HEMP_DEBUG_MEM

hemp_mem_trace_p hemp_mem_traces   = NULL;
hemp_size_t      hemp_mem_used     = 0;
hemp_size_t      hemp_mem_capacity = 0;


/*
 * hemp_mem_new_trace()
 *
 * Returns the next unused memory trace record.
 */

hemp_mem_trace_p
hemp_mem_new_trace() {
    hemp_mem_trace_p hmt;
    int r;

//  hemp_debug_yellow("%d/%d memory traces used\n", hemp_mem_used, hemp_mem_capacity);

    /* create more memory trace records if required */
    if (hemp_mem_used == hemp_mem_capacity) {
        hemp_mem_capacity += HEMP_MEM_BLKSIZ;
        hmt = (hemp_mem_trace_p) malloc(
            hemp_mem_capacity * sizeof(struct hemp_mem_trace_s)
        );
        if (! hmt)
            hemp_fatal("failed to allocate more hemp_mem_trace records");

        /* copy old records */
        if (hemp_mem_traces) {
            memcpy(hmt, hemp_mem_traces, hemp_mem_used * sizeof(struct hemp_mem_trace_s));
            free(hemp_mem_traces);
        }
        hemp_mem_traces = hmt;

        /* initialise new records */
        for (r = hemp_mem_used; r < hemp_mem_capacity; r++) {
            hmt         = &(hemp_mem_traces[r]);
            hmt->id     = r;
            hmt->status = HEMP_MEM_WILD;
            hmt->ptr    = NULL;
            hmt->size   = 0;
            hmt->line   = 0;
            hmt->file   = NULL;
        }
//      hemp_debug_yellow("increased number of memory traces to %d\n", hemp_mem_capacity);
    }

    /* return address of next available memory record */
//  hemp_debug_yellow("using memory trace %d/%d\n", hemp_mem_used, hemp_mem_capacity);
    return &(hemp_mem_traces[hemp_mem_used++]);
}



/*
 * hemp_mem_get_trace(ptr, file, line)
 *
 * Returns the memory trace record associated with a pointer.
 */

hemp_mem_trace_p
hemp_mem_get_trace(
    hemp_mem_p ptr,
    hemp_cstr_p file,
    hemp_pos_t  line
) {
    int r = hemp_mem_used;

//  for(r = 0; r < hemp_mem_used; r++) {
    
    /* work backwards so we get latest memory record first */
    while (--r >= 0) {
        if (hemp_mem_traces[r].ptr == ptr) {
            return &(hemp_mem_traces[r]);
        }
    }

    hemp_fatal(
        "no trace record for memory at %p\n"
        "free() called from %s at line %d\n", 
        ptr, file, line
    );
}



/*
 * hemp_mem_trace_malloc(size)
 *
 * Replacement for malloc() which tracks memory allocations.
 */

hemp_mem_p
hemp_mem_trace_malloc(
    hemp_size_t size,
    hemp_cstr_p file,
    hemp_pos_t  line
) {
    hemp_mem_trace_p hmt = hemp_mem_new_trace();
    hmt->ptr = malloc(size);

    if (! hmt->ptr)
        hemp_fatal("failed to malloc memory");

    hmt->status = HEMP_MEM_MALLOC;
    hmt->size = size;
    hmt->file = file;
    hmt->line = line;

    return hmt->ptr;
}


/*
 * hemp_mem_trace_realloc(ptr, size)
 *
 * Replacement for realloc() which tracks memory allocations.
 */

hemp_mem_p
hemp_mem_trace_realloc(
    hemp_mem_p  ptr, 
    hemp_size_t size,
    hemp_cstr_p file,
    hemp_pos_t  line
) {
    hemp_mem_trace_p hmt;

    if (! ptr)
        return hemp_mem_trace_malloc(size, file, line);

    hmt = hemp_mem_get_trace(ptr, file, line);
    ptr = realloc(ptr, size);

    if (! ptr)
        hemp_fatal("failed to realloc memory");

    /* TODO: re-allocated ptr may be at a new memory location */

    if (hmt->ptr == ptr) {
        hmt->size = size;
    }
    else {
        /* pointer has moved */
//      hmt->size = 0;
        hmt->status = HEMP_MEM_MOVED;

        hmt = hemp_mem_new_trace();
        hmt->ptr    = ptr;
        hmt->status = HEMP_MEM_MALLOC;
        hmt->size   = size;
        hmt->file   = file;
        hmt->line   = line;
    }

    return ptr;
}



/*
 * hemp_mem_trace_strdup(str)
 *
 * Replacement for strdup() which tracks memory allocations.
 */

hemp_cstr_p
hemp_mem_trace_strdup(
    hemp_cstr_p str,
    hemp_cstr_p file,
    hemp_pos_t  line
) {
    hemp_cstr_p dup = (hemp_cstr_p) hemp_mem_trace_malloc(strlen(str) + 1, file, line);
    strcpy(dup, str);
    return dup;
}


/*
 * hemp_mem_trace_free(ptr)
 *
 * Replacement for free() which tracks memory allocations.
 */

void 
hemp_mem_trace_free(
    hemp_mem_p  ptr,
    hemp_cstr_p file,
    hemp_pos_t  line
) {
//    printf("free(%s)\n");
    hemp_mem_trace_p hmt = hemp_mem_get_trace(ptr, file, line);
    free(hmt->ptr);
    hmt->status = HEMP_MEM_FREE;
//  hmt->size   = 0;
    hmt->ptr    = NULL;
}


/*
 * hemp_mem_trace_report(verbose)
 *
 * Generated debugging output showing status of memory allocations.
 */

hemp_size_t
hemp_mem_trace_report(
    hemp_bool_t verbose
) {
    hemp_mem_trace_p hmt;
    int r, i;
    char *status, *cptr, c;
    char buffer[HEMP_MEM_PEEKLEN + 1];
    hemp_size_t count = 0, total = 0;

    if (verbose) {
        hemp_debug_magenta("\nSTATUS    ID         LOCATION       SIZE            MREC\n");
        hemp_debug_magenta("--------------------------------------------------------\n");
    }

    for(r = 0; r < hemp_mem_used; r++) {
        hmt = &(hemp_mem_traces[r]);

        if (hmt->status == HEMP_MEM_MALLOC)
            count += hmt->size;

        total += hmt->size;

        if (! verbose) {
            continue;
        }

        cptr = (char *) hmt->ptr;

        i = 0;
        if (cptr) {
            for (i = 0; i < (hmt->size < HEMP_MEM_PEEKLEN ? hmt->size : HEMP_MEM_PEEKLEN); i++) {
                c = *(cptr + i);
                if (c == 0)
                    break;
                buffer[i] = c >= ' ' && c <= '~' ? c : ' ';
            }
        }
        buffer[i] = '\0';

        status = (
            hmt->status == HEMP_MEM_WILD   ? "UNUSED"   :
            hmt->status == HEMP_MEM_FAIL   ? "FAIL"     :
            hmt->status == HEMP_MEM_MALLOC ? "MALLOC"   :
            hmt->status == HEMP_MEM_MOVED  ? "MOVED"    :
            hmt->status == HEMP_MEM_FREE   ? "FREE"     : 
            "????"
        );
        if (verbose) {
            hemp_debug_blue(
                "line %3d of %s:\n", hmt->line, hmt->file ? hmt->file : "???"
            );
            hemp_debug_cyan(
                "%6s  %4lu   %14p   %8lu  %14p  [%s]\n", 
                status, hmt->id, hmt->ptr, hmt->size, hmt, buffer
            );
        }
    }

    if (verbose) {
        hemp_debug_magenta("--------------------------------------------------------\n");
        hemp_debug_green("Memory used: %8lu\n", total);
        hemp_debug_yellow("Memory wild: %8lu\n", count);
    }
    
    return count;
}


void
hemp_mem_trace_reset() {
    if (hemp_mem_traces)
        free(hemp_mem_traces);

    hemp_mem_used     = 0;
    hemp_mem_capacity = 0;
    hemp_mem_traces   = NULL;
}

#else  /* HEMP_DEBUG & HEMP_DEBUG_MEM */

hemp_size_t
hemp_mem_trace_report(
    hemp_bool_t verbose
) {
    return -1;
}

#endif  /* HEMP_DEBUG & HEMP_DEBUG_MEM */

/*--------------------------------------------------------------------------
 * End of memory debugging functions
 *--------------------------------------------------------------------------*/
