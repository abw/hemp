#include <hemp/scheme/text.h>


HempScheme
hemp_scheme_text_new(
    Hemp   hemp,
    HempString name
) {
    HempScheme scheme  = hemp_scheme_new(hemp, name);
    scheme->checker     = &hemp_scheme_text_checker;
    scheme->reader      = &hemp_scheme_text_reader;
    return scheme;
}

    
HempString
hemp_scheme_text_reader(
    HempSource source
) {
    return (source->text = source->name);
}


HempBool
hemp_scheme_text_checker(
    HempSource source
) {
    return HEMP_TRUE;
}


//void
//hemp_scheme_text_cleaner(
//    HempSource source 
//) {
//    if (source->text) {
//        hemp_debug_msg("cleaning file source: %s\n%s\n", source->name, source->text);
//        hemp_mem_free(source->text);
//    }
//}

