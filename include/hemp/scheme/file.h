#ifndef HEMP_SCHEME_FILE_H
#define HEMP_SCHEME_FILE_H

#include <hemp/scheme.h>
#include <hemp/filesystem.h>


HempScheme
hemp_scheme_file_new(
    Hemp       hemp,
    HempString     name
);

HempString
hemp_scheme_file_namer(
    HempSource     source
);

HempString
hemp_scheme_file_reader(
    HempSource     source
);

HempBool
hemp_scheme_file_checker(
    HempSource     source
);

void
hemp_scheme_file_cleaner(
    HempSource     source
);


#endif /* HEMP_SCHEME_FILE_H */