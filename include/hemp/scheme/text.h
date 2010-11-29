#ifndef HEMP_SCHEME_TEXT_H
#define HEMP_SCHEME_TEXT_H

#include <hemp/scheme.h>


hemp_scheme 
hemp_scheme_text_new( 
    hemp_hemp           hemp, 
    hemp_string         name
);

hemp_string 
hemp_scheme_text_reader(
    hemp_source         source
);

hemp_bool
hemp_scheme_text_checker(
    hemp_source         source
);


#endif /* HEMP_SCHEME_TEXT_H */