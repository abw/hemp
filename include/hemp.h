#ifndef HEMP_H
#define HEMP_H

#if defined(__cplusplus)
extern "C" {
#endif

#include "hemp/types.h"
#include "hemp/memory.h"
#include "hemp/slab.h"
#include "hemp/pool.h"
#include "hemp/hash.h"
#include "hemp/ptree.h"
#include "hemp/element.h"
#include "hemp/elements.h"
#include "hemp/tags.h"
#include "hemp/sources.h"
#include "hemp/dialect.h"
#include "hemp/scanner.h"
#include "hemp/template.h"
#include "hemp/debug.h"
#include "hemp/test.h"


typedef struct hemp * hemp_t;

struct hemp {
    hemp_hash_t dialects;
};

hemp_t  hemp_init();
void    hemp_free(hemp_t);


#define hemp_dialect(hemp, name) \
    (hemp_dialect_t) hemp_hash_fetch(hemp->dialects, name)

#define hemp_add_dialect(hemp, name, dialect) \
    hemp_hash_store(hemp->dialects, name, (hemp_ptr_t) dialect)


#if defined(__cplusplus)
}  /* extern "C" { */
#endif

#endif /* HEMP_H */

