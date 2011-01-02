#include <hemp/scheme/file.h>


HempScheme
hemp_scheme_file_new(
    Hemp   hemp,
    HempString name
) {
    HempScheme scheme  = hemp_scheme_new(hemp, name);
    scheme->namer       = &hemp_scheme_file_namer;
    scheme->checker     = &hemp_scheme_file_checker;
    scheme->reader      = &hemp_scheme_file_reader;
    scheme->cleaner     = &hemp_scheme_file_cleaner;
    return scheme;
}


HempString
hemp_scheme_file_namer(
    HempSource source 
) {
    return source->name;
}


HempBool
hemp_scheme_file_checker(
    HempSource source
) {
    // TODO: check file exists and is readable
    hemp_debug_red("TODO: hemp_scheme_file_checker()\n");
    return HEMP_FALSE;
}


HempString
hemp_scheme_file_reader(
    HempSource source 
) {
    source->text = hemp_filesystem_read_file(source->name);

    if (! source->text) {
        hemp_fatal("failed to read file: %s\n", source->name);          // fixme
    }
    
    return source->text;
}


void
hemp_scheme_file_cleaner(
    HempSource source 
) {
    if (source->text) {
        hemp_debug_call("cleaning file source: %s\n%s\n", source->name, source->text);
        hemp_mem_free(source->text);
    }
}
