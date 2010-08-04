#ifndef HEMP_H
#define HEMP_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <hemp/core.h>
#include <hemp/action.h>
#include <hemp/dialect.h>
#include <hemp/element.h>
#include <hemp/elements.h>
#include <hemp/factory.h>
#include <hemp/filesystem.h>
#include <hemp/hash.h>
#include <hemp/pool.h>
#include <hemp/ptree.h>
#include <hemp/scanner.h>
#include <hemp/scheme.h>
#include <hemp/scope.h>
#include <hemp/source.h>
#include <hemp/tag.h>
#include <hemp/tagset.h>
#include <hemp/template.h>
#include <hemp/text.h>

    
//#include <hemp/module.h>
//#include <hemp/error.h>
//#include <hemp/filesystem.h>
//#include <hemp/type.h>
//#include <hemp/hub.h>
//#include <hemp/context.h>
//#include <hemp/value.h>
//#include <hemp/templates.h>


struct hemp_s {
    hemp_factory_p   elements;
    // TODO: replace these with dialects factory
    hemp_hash_p      dialects;
    hemp_hash_p      dialect_registry;

    hemp_hash_p      schemes;
//  hemp_hash_p      tags;
    hemp_hash_p      templates;
    hemp_dialect_p   dialect;

    hemp_bool_t      verbose;
    hemp_bool_t      debug;
//  hemp_hash_t dialect_factory;
    hemp_jump_p      jump;
    hemp_error_p     error;
    hemp_cstr_p     *errmsg;
};


/* hemp initialisation and cleanup functions */
hemp_p          hemp_init();
void            hemp_free(hemp_p);

/* scheme functions */
void            hemp_init_schemes(hemp_p);
void            hemp_add_scheme(hemp_p, hemp_scheme_p);
hemp_bool_t     hemp_free_scheme(hemp_hash_p, hemp_pos_t, hemp_hash_item_p);

#define         hemp_scheme(hemp, name) \
                    (hemp_scheme_p) hemp_hash_fetch(hemp->schemes, name)

/* dialect functions */
void            hemp_init_dialects(hemp_p);
void            hemp_register_dialect(hemp_p, hemp_cstr_p, hemp_dialect_f);
void            hemp_add_dialect(hemp_p, hemp_dialect_p);
hemp_bool_t     hemp_free_dialect(hemp_hash_p, hemp_pos_t, hemp_hash_item_p);

hemp_dialect_p  hemp_dialect(hemp_p, hemp_cstr_p);


/* source functions */
hemp_source_p   hemp_source(hemp_p, hemp_cstr_p, hemp_cstr_p);

/* tag functions */
//void            hemp_init_tags(hemp_p);
//void            hemp_add_tag(hemp_p, hemp_tag_p);
//hemp_bool_t     hemp_free_tag(hemp_hash_p, hemp_pos_t, hemp_hash_item_p);
//
//#define         hemp_tag(hemp, name) \
//                    (hemp_tag_p) hemp_hash_fetch(hemp->tags, name)


/* template functions */
void            hemp_init_templates(hemp_p);
hemp_bool_t     hemp_free_template(hemp_hash_p, hemp_pos_t, hemp_hash_item_p);
hemp_template_p hemp_template(hemp_p, hemp_cstr_p, hemp_cstr_p, hemp_cstr_p);


/* element functions */
//void            hemp_element_constructor(hemp_p, hemp_cstr_p, hemp_actor_f);
//void            hemp_element_instance(hemp_p, hemp_cstr_p);

#define hemp_element_constructor(hemp,name,constructor)     \
    hemp_factory_constructor(                               \
        hemp->elements,                                     \
        name,                                               \
        constructor,                                        \
        hemp                                                \
    )

#define hemp_element_instance(hemp,name)                    \
    hemp_factory_instance(                                  \
        hemp->elements,                                     \
        name                                                \
    )

#define hemp_element(hemp,name) ({                                          \
        hemp_element_p _elem = (hemp_element_p) hemp_factory_instance(      \
            hemp->elements, name                                            \
        );                                                                  \
        if (! _elem) HEMP_THROW(HEMP_ERROR_INVALID);                       \
        _elem;   \
    })



//hemp_bool_t hemp_register_dialect(hemp_t, hemp_cstr_t, hemp_build_fn);
//void        hemp_hello();
//hemp_cstr_t hemp_render(hemp_cstr_t scheme, hemp_cstr_t source);
//
//
//#define hemp_dialect(hemp, name) \
//    (hemp_dialect_t) hemp_hash_fetch(hemp->dialects, name)
//
//#define hemp_add_dialect(hemp, name, dialect) \
//    hemp_hash_store(hemp->dialects, name, (hemp_ptr_t) dialect)
//

#if defined(__cplusplus)
}  /* extern "C" { */
#endif

#endif /* HEMP_H */

