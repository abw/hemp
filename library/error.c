#include <hemp/core.h>
#include <hemp/scanner.h>


hemp_string hemp_errmsg[] = {
    "No error",
    "Unknown error",
    "Failed to allocate memory for a new %s",
    "No %s specified",
    "Invalid %s specified: %s",
    "Duplicate %s specified: %s",
    "Number is too large: %s",
    "Invalid token: %s",
    "Unterminated %s, missing closing %s",
    "Missing expression for '%s'",
    "Missing body for '%s'",
    "Incomplete '%s' expression, missing closing '%s'",
    "Unexpected token: %s",
    "Cannot convert %s to %s: %s",
    "%s value",
    "Cannot fetch '%s' from %s",
    "Cannot store '%s' in %s",
    "Invalid option: %s",
    NULL
};

hemp_location
hemp_location_new() {
    hemp_location location;
    HEMP_ALLOCATE(location);
    
    location->position  = 0;
    location->line      = 0;
    location->column    = 0;
    location->extract   = NULL;

    return location;
}


void
hemp_location_free(
    hemp_location   location
) {
    hemp_mem_free(location);
}



hemp_error
hemp_error_new(
    hemp_errno number
) {
    hemp_error error;
    
    if (number < 0 || number >= HEMP_ERROR_MAX) 
        hemp_fatal("Invalid error number: %d", number);

    HEMP_ALLOCATE(error);

    error->number   = number;
    error->message  = NULL;
    error->parent   = NULL;
    error->document = NULL;
    error->location = NULL;

    return error;
}


hemp_error
hemp_error_init(
    hemp_errno  number,
    hemp_string message
) {
    hemp_error error = hemp_error_new(number);

    /* We use strdup() to deliberately avoid the memory trace that is wrapped 
     * around mem_string_copy() and friend when memory debugging is enabled.
     * This is because the vasprintf() functions called below also avoid our
     * basic memory allocation tracking.  So we call free() not hemp_mem_free()
     * in hemp_error_free() below.
     */
    error->message = strdup(message);

    if (! error->message)
        hemp_mem_fail("error message");
    
    return error;
}


hemp_error
hemp_error_initf(
    hemp_errno  number,
    hemp_string format,
    ...
) {
    hemp_error error = hemp_error_new(number);

    va_list args;
    va_start(args, format);
    vasprintf(&error->message, format, args);
    va_end(args);
    return error;
}


hemp_error
hemp_error_initfv(
    hemp_errno  number,
    hemp_string format,
    va_list     args
) {
    hemp_error error = hemp_error_new(number);
    vasprintf(&error->message, format, args);
    return error;
}


HEMP_INLINE hemp_error
hemp_error_document(
    hemp_error    error,
    hemp_document document
) {
    error->document = document;
    return error;
}


HEMP_INLINE hemp_error
hemp_error_location(
    hemp_error      error,
    hemp_string     source,
    hemp_string     marker
) {
    if (source && marker) {
        /* re-use any existing location structure or creates a new one */
        error->location = hemp_string_location(
            source, marker, error->location
        );
    }
    return error;
}


HEMP_INLINE hemp_error
hemp_error_document_location(
    hemp_error      error,
    hemp_document   document,
    hemp_string     marker
) {
    hemp_error_document(error, document);
    hemp_error_location(error, document->source->text, marker);
    return error;
}


void
hemp_error_free(
    hemp_error error
) {
    /* memory allocated by vasprintf() so don't use hemp_mem_free() because 
     * we're not tracking it and it'll blow a fuse when debugging memory
     */
    // TODO: add code to notify the memory tracker so we don't need this...
    if (error->message)
        free(error->message);

    if (error->location)
        hemp_location_free(error->location);

    hemp_mem_free(error);
}

