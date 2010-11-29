#include <hemp/scheme/file.h>


hemp_scheme
hemp_scheme_file_new(
    hemp_hemp   hemp,
    hemp_string name
) {
    hemp_scheme scheme  = hemp_scheme_new(hemp, name);
    scheme->namer       = &hemp_scheme_file_namer;
    scheme->checker     = &hemp_scheme_file_checker;
    scheme->reader      = &hemp_scheme_file_reader;
    scheme->cleaner     = &hemp_scheme_file_cleaner;
    return scheme;
}


hemp_string
hemp_scheme_file_namer(
    hemp_source source 
) {
    return source->name;
}


hemp_bool
hemp_scheme_file_checker(
    hemp_source source
) {
    // TODO: check file exists and is readable
    hemp_debug_red("TODO: hemp_scheme_file_checker()\n");
    return HEMP_FALSE;
}


hemp_string
hemp_scheme_file_reader(
    hemp_source source 
) {
    source->text = hemp_filesystem_read_file(source->name);

    if (source->text) {
        hemp_debug("read text from file: %s\n", source->name);
    }
    else {
        hemp_fatal("failed to read file: %s\n", source->name);          // fixme
    }
    
    return source->text;
}


void
hemp_scheme_file_cleaner(
    hemp_source source 
) {
    if (source->text) {
        hemp_debug_msg("cleaning file source: %s\n%s\n", source->name, source->text);
        hemp_mem_free(source->text);
    }
}
