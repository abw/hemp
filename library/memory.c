#include <hemp/memory.h>


void hemp_mem_fail(
    hemp_string type
) {
    hemp_fatal(hemp_errmsg[HEMP_ERROR_MEMORY], type);
}


#ifdef HEMP_ADD_MEM_COPY

hemp_memory
hemp_mem_copy(
    hemp_memory src,
    hemp_memory dest,
    hemp_size   len
) {
    hemp_size   i;
    hemp_string s = (hemp_string) src;
    hemp_string d = (hemp_string) dest;

//  hemp_debug("copying %d bytes of memory from %p to %p\n", len, src, dest);
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

hemp_mem_trace hemp_mem_traces   = NULL;
hemp_size      hemp_mem_used     = 0;
hemp_size      hemp_mem_capacity = 0;


hemp_mem_trace
hemp_mem_trace_new() {
    hemp_mem_trace hmt;
    int r;

    /* create more memory trace records if required */
    if (hemp_mem_used == hemp_mem_capacity) {
        hemp_mem_capacity += HEMP_MEM_BLKSIZ;
//      hemp_debug(
//          "allocating %d bytes for %d records x %d bytes\n", 
//          hemp_mem_capacity * sizeof(struct hemp_mem_trace),
//          hemp_mem_capacity, sizeof(struct hemp_mem_trace)
//      );
        hmt = (hemp_mem_trace) malloc(
            hemp_mem_capacity * sizeof(struct hemp_mem_trace)
        );
        if (! hmt)
            hemp_fatal("failed to allocate more hemp_mem_trace records");

//      hemp_debug("expanded records\n");
//      hemp_mem_trace_report(1);

        /* copy old records */
        if (hemp_mem_traces) {
//          hemp_debug("copying %d old records = %d bytes\n", hemp_mem_used, hemp_mem_used * sizeof(struct hemp_mem_trace));
            hemp_mem_copy(
                hemp_mem_traces, hmt,
                hemp_mem_used * sizeof(struct hemp_mem_trace)
            );
            free(hemp_mem_traces);
        }
        hemp_mem_traces = hmt;

//        hemp_debug("copied %d records\n", hemp_mem_used);
//        hemp_mem_trace_report(1);
//        hemp_debug("initialising new records %d to %d\n", hemp_mem_used, hemp_mem_capacity - 1);

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
//        hemp_mem_trace_report(1);
    }

    return &(hemp_mem_traces[hemp_mem_used++]);
}


hemp_mem_trace
hemp_mem_trace_record(
    hemp_memory ptr,
    hemp_string file,
    hemp_pos    line
) {
    int r = hemp_mem_used;

//    hemp_debug("looking for memory trace for %p\n", ptr);

    /* work backwards so we get latest memory record first */
    while (--r >= 0) {
//        hemp_debug("%3d: %p\n", r, hemp_mem_traces[r].ptr);
        if (hemp_mem_traces[r].ptr == ptr) {
            return &(hemp_mem_traces[r]);
        }
    }

//    hemp_mem_trace_report(1);
    hemp_fatal(
        "no trace record for memory at %p\n"
        "free() called from %s at line %d\n", 
        ptr, file, line
    );

    return NULL;
}



hemp_memory
hemp_mem_trace_malloc(
    hemp_size   size,
    hemp_string file,
    hemp_pos    line
) {
    hemp_mem_trace hmt = hemp_mem_trace_new();
    hmt->ptr = malloc(size);

    if (! hmt->ptr)
        hemp_fatal("failed to malloc memory");

    hmt->status = HEMP_MEM_MALLOC;
    hmt->size = size;
    hmt->line = line;
    hmt->file = NULL;

    if (file) {
        /* Sigh.  We have to copy this now that we're using dynamically 
         * loaded modules because the memory containing the file name (from
         * __FILE__) may have become inaccessible by the time we run a 
         * memory trace report (I suspect because we've unloaded the modules
         * by the time the trace report happens.
         */
        hmt->file = malloc(strlen(file) + 1);
        strcpy(hmt->file, file);
    }
        

//  hemp_debug("%d: memory tracing %p via record at %p\n", hmt->id, hmt->ptr, hmt);
//  hemp_debug("  FILE: (%p) %s  LINE: %d\n", hmt->file, hmt->file ? hmt->file : "-", line);
    
    return hmt->ptr;
}


hemp_memory
hemp_mem_trace_external(
    hemp_memory ptr,
    hemp_size   size,
    hemp_string file,
    hemp_pos    line
) {
    hemp_mem_trace hmt  = hemp_mem_trace_new();
    hmt->ptr            = ptr;
    hmt->status         = HEMP_MEM_EXTERNAL;
    hmt->size           = size;
    hmt->file           = file;
    hmt->line           = line;
    return hmt->ptr;
}


hemp_memory
hemp_mem_trace_realloc(
    hemp_memory ptr, 
    hemp_size   size,
    hemp_string file,
    hemp_pos    line
) {
    hemp_mem_trace hmt;

    if (! ptr)
        return hemp_mem_trace_malloc(size, file, line);

    hmt = hemp_mem_trace_record(ptr, file, line);
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

        hmt         = hemp_mem_trace_new();
        hmt->ptr    = ptr;
        hmt->status = HEMP_MEM_MALLOC;
        hmt->size   = size;
        hmt->file   = file;
        hmt->line   = line;
    }

    return ptr;
}


hemp_string
hemp_mem_trace_strdup(
    hemp_string str,
    hemp_string file,
    hemp_pos    line
) {
    hemp_string dup = (hemp_string) hemp_mem_trace_malloc(strlen(str) + 1, file, line);
    strcpy(dup, str);
    return dup;
}


void 
hemp_mem_trace_free(
    hemp_memory ptr,
    hemp_string file,
    hemp_pos    line
) {
    hemp_mem_trace hmt = hemp_mem_trace_record(ptr, file, line);
    free(hmt->ptr);
    hmt->status = HEMP_MEM_FREE;
//  hmt->size   = 0;
    hmt->ptr    = NULL;
}


hemp_size
hemp_mem_trace_report(
    hemp_bool verbose
) {
    hemp_mem_trace hmt;
    int r, i;
    char *status, *cptr, c;
    char buffer[HEMP_MEM_PEEKLEN + 1];
    hemp_size count = 0, total = 0;

    if (verbose) {
        hemp_debug_cyan("\nID   STATUS      SIZE       LOCATION\n");
        hemp_debug_cyan(HEMP_MEM_DASHES);
    }

    for(r = 0; r < hemp_mem_used; r++) {
        hmt = &(hemp_mem_traces[r]);

        if (hmt->status == HEMP_MEM_MALLOC || hmt->status == HEMP_MEM_EXTERNAL)
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
            hmt->status == HEMP_MEM_WILD     ? "UNUSED"   :
            hmt->status == HEMP_MEM_FAIL     ? "FAIL"     :
            hmt->status == HEMP_MEM_MALLOC   ? "MALLOC"   :
            hmt->status == HEMP_MEM_MOVED    ? "MOVED"    :
            hmt->status == HEMP_MEM_EXTERNAL ? "EXTERNAL" :
            hmt->status == HEMP_MEM_FREE     ? "FREE"     : 
            "????"
        );
        if (verbose) {
            hemp_debug(
                "%s%-4lu %6s  %8lu %14p  line %3d of %-12s [%s]\n", 
                hmt->status == HEMP_MEM_MALLOC ? HEMP_ANSI_RED : HEMP_ANSI_GREEN,
                hmt->id, status, hmt->size, hmt->ptr, 
                hmt->line, hmt->file ? hmt->file : "???", 
                buffer
            );
        }
    }

    if (verbose) {
        hemp_debug_cyan(HEMP_MEM_DASHES);
        hemp_debug_yellow("Memory used: %8lu\n", total);
        hemp_debug_red("Memory wild: %8lu\n", count);
    }
    
    return count;
}


void
hemp_mem_trace_reset() {
    hemp_debug("memory trace reset!\n");
    if (hemp_mem_traces)
        free(hemp_mem_traces);

    hemp_mem_used     = 0;
    hemp_mem_capacity = 0;
    hemp_mem_traces   = NULL;
}

#else  /* HEMP_DEBUG & HEMP_DEBUG_MEM */

hemp_size
hemp_mem_trace_report(
    hemp_bool verbose
) {
    return -1;
}

#endif  /* HEMP_DEBUG & HEMP_DEBUG_MEM */
