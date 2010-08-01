#include <stdio.h>
#include "hemp/memory.h"
#include "hemp/type.h"
#include "hemp/debug.h"




/* hemp_memory_copy(src, dest, length) 
 *
 * Generic memory copy for systems (if there are any) that don't provide 
 * memmove() or bcopy().
 */

#ifdef HEMP_ADD_MEM_COPY

hemp_ptr_t
hemp_mem_copy_fn(
    hemp_mem_t  src,
    hemp_ptr_t  dest,
    hemp_size_t len
) {
    hemp_size_t i;
    hemp_cstr_t s = (hemp_cstr_t) src;
    hemp_cstr_t d = (hemp_cstr_t) dest;

    debug_mem("WARNING: using hemp_mem_copy() is probably a bad idea\n");

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

#if DEBUG & DEBUG_MEM

hemp_mem_trace_t hemp_mem_traces   = NULL;
hemp_size_t      hemp_mem_used     = 0;
hemp_size_t      hemp_mem_capacity = 0;


/*
 * hemp_mem_new_trace()
 *
 * Returns the next unused memory trace record.
 */

hemp_mem_trace_t 
hemp_mem_new_trace() {
    hemp_mem_trace_t hmt;
    int r;

//  debug_yellow("%d/%d memory traces used\n", hemp_mem_used, hemp_mem_capacity);

    /* create more memory trace records if required */
    if (hemp_mem_used == hemp_mem_capacity) {
        hemp_mem_capacity += HEMP_MEM_BLKSIZ;
        hmt = (hemp_mem_trace_t) malloc(
            hemp_mem_capacity * sizeof(struct hemp_mem_trace)
        );
        if (! hmt)
            hemp_fatal("failed to allocate more hemp_mem_trace records");

        /* copy old records */
        if (hemp_mem_traces) {
            memcpy(hmt, hemp_mem_traces, hemp_mem_used * sizeof(struct hemp_mem_trace));
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
//      debug_yellow("increased number of memory traces to %d\n", hemp_mem_capacity);
    }

    /* return address of next available memory record */
//  debug_yellow("using memory trace %d/%d\n", hemp_mem_used, hemp_mem_capacity);
    return &(hemp_mem_traces[hemp_mem_used++]);
}



/*
 * hemp_mem_get_trace(ptr)
 *
 * Returns the memory trace record associated with a pointer.
 */

hemp_mem_trace_t 
hemp_mem_get_trace(
    void *ptr
) {
    int r;

    for(r = 0; r < hemp_mem_used; r++) {
        if (hemp_mem_traces[r].ptr == ptr) {
            return &(hemp_mem_traces[r]);
        }
    }

    hemp_fatal("no memory record for memory at %p\n", ptr);
}



/*
 * hemp_mem_debug_malloc(size)
 *
 * Replacement for malloc() which tracks memory allocations.
 */

void *
hemp_mem_debug_malloc(
    size_t size,
    char * file,
    unsigned int line
) {
    hemp_mem_trace_t hmt = hemp_mem_new_trace();
    hmt->ptr = malloc(size);
//  debug_mem("hemp_mem_debug_malloc(%u) => [%d] %p\n", size, hmt->id, hmt->ptr);
    
    if (hmt->ptr) {
        hmt->status = HEMP_MEM_MALLOC;
        hmt->size = size;
        hmt->file = file;
        hmt->line = line;
    }
    else {
        hemp_fatal("failed to malloc memory");
    }
    
    return hmt->ptr;
}


/*
 * hemp_mem_debug_realloc(ptr, size)
 *
 * Replacement for realloc() which tracks memory allocations.
 */

void *
hemp_mem_debug_realloc(
    void *ptr, 
    size_t size,
    char *file,
    unsigned int line
) {
    hemp_mem_trace_t hmt;

    if (! ptr)
        return hemp_mem_debug_malloc(size, file, line);

    hmt = hemp_mem_get_trace(ptr);
    hmt->ptr = realloc(hmt->ptr, size);
    
    if (hmt->ptr) {
        hmt->size = size;
    }
    else {
        hemp_fatal("failed to realloc memory");
    }

    return hmt->ptr;
}



/*
 * hemp_mem_debug_strdup(str)
 *
 * Replacement for strdup() which tracks memory allocations.
 */

char *
hemp_mem_debug_strdup(
    char *str,
    char *file,
    unsigned int line
) {
    char *dup = (char *) hemp_mem_debug_malloc(strlen(str) + 1, file, line);
    strcpy(dup, str);
    return dup;
}


/*
 * hemp_mem_debug_free(ptr)
 *
 * Replacement for free() which tracks memory allocations.
 */

void 
hemp_mem_debug_free(
    void *ptr
) {
    hemp_mem_trace_t hmt = hemp_mem_get_trace(ptr);
    free(hmt->ptr);
    hmt->status = HEMP_MEM_FREE;
    hmt->size   = 0;
    hmt->ptr    = NULL;
}


/*
 * hemp_mem_debug_report(verbose)
 *
 * Generated debugging output showing status of memory allocations.
 */

void hemp_mem_debug_report(
    hemp_bool_t verbose
) {
    hemp_mem_trace_t hmt;
    int r, i;
    char *status, *cptr, c;
    unsigned long count = 0;
    char buffer[HEMP_MEM_PEEKLEN + 1];

    if (verbose) {
        debug_magenta("\nSTATUS    ID         LOCATION       SIZE            MREC\n");
        debug_magenta("--------------------------------------------------------\n");
    }

    for(r = 0; r < hemp_mem_used; r++) {
        hmt = &(hemp_mem_traces[r]);
        count += hmt->size;

        if (! verbose) {
            continue;
        }

        cptr = (char *) hmt->ptr;

        for (i = 0; i < (hmt->size < HEMP_MEM_PEEKLEN ? hmt->size : HEMP_MEM_PEEKLEN); i++) {
            c = *(cptr + i);
            buffer[i] = c >= ' ' && c <= '~' ? c : ' ';
        }
        buffer[i] = '\0';

        status = (
            hmt->status == HEMP_MEM_WILD   ? "UNUSED"   :
            hmt->status == HEMP_MEM_FAIL   ? "FAIL"     :
            hmt->status == HEMP_MEM_MALLOC ? "MALLOC"   :
            hmt->status == HEMP_MEM_FREE   ? "FREE"     : 
            "????"
        );
        debug_mem("%6s  %4lu   %14p   %8lu  %14p  [%s]\n", 
            status, hmt->id, hmt->ptr, hmt->size, hmt, buffer);
        debug_blue("  line %3d of %s\n", hmt->line, hmt->file ? hmt->file : "???");
    }

    if (verbose) {
        debug_magenta("--------------------------------------------------------\n");
    }
    
    debug_yellow("Memory allocated: %8lu\n", count);
}


void 
hemp_mem_debug_cleanup() {
    if (hemp_mem_traces)
        free(hemp_mem_traces);
    hemp_mem_used = hemp_mem_capacity = 0;
}


#endif  /* DEBUG & DEBUG_MEM */

/*--------------------------------------------------------------------------
 * End of memory debugging functions
 *--------------------------------------------------------------------------*/
