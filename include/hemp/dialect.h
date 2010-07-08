#ifndef HEMP_DIALECT_H
#define HEMP_DIALECT_H

#include "hemp/memory.h"
#include "hemp/tags.h"
#include "hemp/elements.h"
#include "hemp/sources.h"

typedef struct hemp_dialect * hemp_dialect_t;
typedef hemp_bool_t (* hemp_scan_fn)(hemp_template_t);

extern hemp_dialect_t HempDialectTT3;

struct hemp_dialect {
    hemp_ident_t    name;
    hemp_scan_fn    scanner;
//    hemp_tagset_t   tagset;
};

/*
hemp_dialect_t hemp_dialect_init(hemp_name_t);
void           hemp_dialect_free(hemp_dialect_t);
*/

hemp_bool_t hemp_dialect_tt3_scan(hemp_template_t);

#endif /* HEMP_DIALECT_H */