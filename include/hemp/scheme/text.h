#ifndef HEMP_SCHEME_TEXT_H
#define HEMP_SCHEME_TEXT_H

#include <hemp/scheme.h>


HempScheme 
hemp_scheme_text_new( 
    Hemp       hemp, 
    HempString     name
);

HempString 
hemp_scheme_text_reader(
    HempSource     source
);

HempBool
hemp_scheme_text_checker(
    HempSource     source
);


#endif /* HEMP_SCHEME_TEXT_H */