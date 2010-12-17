#include <hemp/scheme/text.h>


hemp_scheme
hemp_scheme_text_new(
    hemp_hemp   hemp,
    hemp_string name
) {
    hemp_scheme scheme  = hemp_scheme_new(hemp, name);
    scheme->checker     = &hemp_scheme_text_checker;
    scheme->reader      = &hemp_scheme_text_reader;
    return scheme;
}

    
hemp_string
hemp_scheme_text_reader(
    hemp_source source
) {
    return (source->text = source->name);
}


hemp_bool
hemp_scheme_text_checker(
    hemp_source source
) {
    return HEMP_TRUE;
}


//void
//hemp_scheme_text_cleaner(
//    hemp_source source 
//) {
//    if (source->text) {
//        hemp_debug_msg("cleaning file source: %s\n%s\n", source->name, source->text);
//        hemp_mem_free(source->text);
//    }
//}

