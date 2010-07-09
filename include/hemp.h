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
//#include "hemp/test.h"


struct hemp {
    hemp_hash_t dialects;
    hemp_hash_t dialect_factory;
};

hemp_t      hemp_init();
void        hemp_free(hemp_t);
hemp_bool_t hemp_register_dialect(hemp_t, hemp_cstr_t, hemp_build_fn);

#define hemp_null(h)    \
    hemp_free(h);       \
    h = NULL;                

#define hemp_dialect(hemp, name) \
    (hemp_dialect_t) hemp_hash_fetch(hemp->dialects, name)

#define hemp_add_dialect(hemp, name, dialect) \
    hemp_hash_store(hemp->dialects, name, (hemp_ptr_t) dialect)


#if defined(__cplusplus)
}  /* extern "C" { */
#endif

#endif /* HEMP_H */

