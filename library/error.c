#include <hemp/core.h>
#include <hemp/scanner.h>


HempString hemp_errmsg[] = {
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
    "Missing fragment after '%s'",
    "Fragment mismatch: %s vs %s",
    "Unexpected token: %s",
    "Cannot convert %s to %s: %s",
    "Undefined value",
    "Cannot fetch '%s' from %s",
    "Cannot store '%s' in %s",
    "Invalid option: %s",
    NULL
};

HempLocation
hemp_location_new() {
    HempLocation location;
    HEMP_ALLOCATE(location);
    
    location->position  = 0;
    location->line      = 0;
    location->column    = 0;
    location->extract   = NULL;

    return location;
}


void
hemp_location_free(
    HempLocation   location
) {
    hemp_mem_free(location);
}



HempError
hemp_error_new(
    HempErrno number
) {
    HempError error;
    
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


HempError
hemp_error_init(
    HempErrno  number,
    HempString message
) {
    HempError error = hemp_error_new(number);

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


HempError
hemp_error_initf(
    HempErrno  number,
    HempString format,
    ...
) {
    HempError error = hemp_error_new(number);

    va_list args;
    va_start(args, format);
    vasprintf(&error->message, format, args);
    va_end(args);
    return error;
}


HempError
hemp_error_initfv(
    HempErrno  number,
    HempString format,
    va_list     args
) {
    HempError error = hemp_error_new(number);
    vasprintf(&error->message, format, args);
    return error;
}


HEMP_INLINE HempError
hemp_error_document(
    HempError    error,
    HempDocument document
) {
    error->document = document;
    return error;
}


HEMP_INLINE HempError
hemp_error_location(
    HempError      error,
    HempString     source,
    HempString     marker
) {
    if (source && marker) {
        /* re-use any existing location structure or creates a new one */
        error->location = hemp_string_location(
            source, marker, error->location
        );
    }
    return error;
}


HEMP_INLINE HempError
hemp_error_document_location(
    HempError      error,
    HempDocument   document,
    HempString     marker
) {
    hemp_error_document(error, document);
    hemp_error_location(error, document->source->text, marker);
    return error;
}


void
hemp_error_free(
    HempError error
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

