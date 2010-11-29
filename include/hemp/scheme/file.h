#ifndef HEMP_SCHEME_FILE_H
#define HEMP_SCHEME_FILE_H

#include <hemp/scheme.h>
#include <hemp/filesystem.h>


hemp_scheme
hemp_scheme_file_new(
    hemp_hemp       hemp,
    hemp_string     name
);

hemp_string
hemp_scheme_file_namer(
    hemp_source     source
);

hemp_string
hemp_scheme_file_reader(
    hemp_source     source
);

hemp_bool
hemp_scheme_file_checker(
    hemp_source     source
);

void
hemp_scheme_file_cleaner(
    hemp_source     source
);


#endif /* HEMP_SCHEME_FILE_H */