#ifndef HEMP_H
#define HEMP_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <hemp/defaults.h>
#include <hemp/types.h>
#include <hemp/cstr.h>
#include <hemp/memory.h>
#include <hemp/slab.h>
#include <hemp/pool.h>
#include <hemp/hash.h>
#include <hemp/list.h>
#include <hemp/ptree.h>
#include <hemp/element.h>
#include <hemp/elements.h>
#include <hemp/tags.h>
#include <hemp/sources.h>
#include <hemp/dialect.h>
#include <hemp/scanner.h>
#include <hemp/template.h>
#include <hemp/module.h>
#include <hemp/utils.h>
#include <hemp/error.h>
#include <hemp/filesystem.h>
#include <hemp/debug.h>
#include <hemp/os.h>
#include <hemp/scope.h>
#include <hemp/type.h>
#include <hemp/text.h>
#include <hemp/hub.h>
#include <hemp/context.h>
#include <hemp/value.h>


#define HEMP_NAME       PACKAGE_NAME
#define HEMP_VERSION    PACKAGE_VERSION
#define HEMP_EMAIL      PACKAGE_BUGREPORT
#define HEMP_URL        PACKAGE_URL
#define HEMP_PROMPT     HEMP_NAME
#define HEMP_AUTHOR     "Andy Wardley"

#define HEMP_ERRNO_HELP     1
#define HEMP_ERRNO_GETOPT   2

struct hemp {
    hemp_hash_t     dialects;
    hemp_hash_t     dialect_factory;
    hemp_jump_t     *jump;
    hemp_bool_t     verbose;
    hemp_bool_t     debug;
};


hemp_t      hemp_init();
void        hemp_free(hemp_t);
hemp_bool_t hemp_register_dialect(hemp_t, hemp_cstr_t, hemp_build_fn);
void        hemp_hello();
hemp_cstr_t hemp_render(hemp_cstr_t scheme, hemp_cstr_t source);


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

