#ifndef HEMP_H
#define HEMP_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <hemp/core.h>
#include <hemp/action.h>
#include <hemp/context.h>
#include <hemp/dialect.h>
#include <hemp/element.h>
#include <hemp/elements.h>
#include <hemp/factory.h>
#include <hemp/filesystem.h>
#include <hemp/grammar.h>
#include <hemp/language.h>
#include <hemp/namespace.h>
#include <hemp/params.h>
#include <hemp/pool.h>
#include <hemp/ptree.h>
#include <hemp/scanner.h>
#include <hemp/scheme.h>
#include <hemp/scope.h>
#include <hemp/source.h>
#include <hemp/symbol.h>
#include <hemp/tag.h>
#include <hemp/tagset.h>
#include <hemp/template.h>
#include <hemp/value.h>
#include <hemp/viewer.h>
#include <hemp/macros.h>
#include <hemp/type/text.h>
#include <hemp/type/list.h>
#include <hemp/type/hash.h>
#include <hemp/type/code.h>
#include <hemp/language/test.h>

    
//#include <hemp/module.h>
//#include <hemp/error.h>
//#include <hemp/filesystem.h>
//#include <hemp/type.h>
//#include <hemp/hub.h>
//#include <hemp/context.h>
//#include <hemp/templates.h>



/* hemp initialisation and cleanup functions */
hemp_hemp   hemp_init();
void        hemp_init_schemes(hemp_hemp);
void        hemp_init_errors(hemp_hemp);
void        hemp_free(hemp_hemp);
hemp_bool   hemp_free_scheme(   hemp_hash, hemp_pos, hemp_slot);
hemp_bool   hemp_free_language( hemp_hash, hemp_pos, hemp_slot);
hemp_bool   hemp_free_dialect(  hemp_hash, hemp_pos, hemp_slot);
hemp_bool   hemp_free_grammar(  hemp_hash, hemp_pos, hemp_slot);
hemp_bool   hemp_free_element(  hemp_hash, hemp_pos, hemp_slot);
hemp_bool   hemp_free_template( hemp_hash, hemp_pos, hemp_slot);
hemp_bool   hemp_free_viewer(   hemp_hash, hemp_pos, hemp_slot);

/* scheme functions */
void            hemp_add_scheme(hemp_hemp, hemp_scheme);
#define         hemp_scheme_instance(hemp, name) \
                    (hemp_scheme) hemp_hash_fetch_pointer(hemp->schemes, name)

/* source functions */
hemp_source   hemp_source_instance(hemp_hemp, hemp_string, hemp_string);

/* template functions */
void            hemp_init_templates(hemp_hemp);
hemp_template hemp_template_instance(hemp_hemp, hemp_string, hemp_string, hemp_string);

/* runtime functions */
hemp_context  hemp_context_instance(hemp_hemp);

//void            hemp_throw(hemp_hemp, hemp_errno_e, ...);
hemp_string      hemp_error_format(hemp_hemp, hemp_errno);
hemp_error    hemp_error_message(hemp_hemp, hemp_errno, ...);
hemp_error    hemp_error_scan_pos(hemp_error, hemp_scan_pos);
void            hemp_error_throw(hemp_hemp, hemp_error);
void            hemp_scan_error(hemp_hemp, HEMP_SCAN_ARGS, hemp_errno, ...);


/* tag functions */
//void            hemp_init_tags(hemp_hemp);
//void            hemp_add_tag(hemp_hemp, hemp_tag);
//
//#define         hemp_tag(hemp, name) \
//                    (hemp_tag) hemp_hash_fetch(hemp->tags, name)

hemp_string hemp_version();




void hemp_register_elements(hemp_hemp, hemp_symbols);

/* language macros */

#define hemp_register_language(hemp,name,constructor)                       \
    hemp_factory_register(                                                  \
        hemp->languages,                                                    \
        name,                                                               \
        constructor,                                                        \
        hemp                                                                \
    )

#define hemp_language_instance(hemp,name) ({                                \
    hemp_language _lang = (hemp_language) hemp_factory_instance(            \
        hemp->languages, name                                               \
    );                                                                      \
    if (! _lang) hemp_throw(hemp, HEMP_ERROR_INVALID, "language", name);    \
    _lang;                                                                  \
})


/* element macros */

#define hemp_register_element(hemp,name,constructor)                        \
    hemp_factory_register(                                                  \
        hemp->elements,                                                     \
        name,                                                               \
        constructor,                                                        \
        hemp                                                                \
    )

#define hemp_symbol_instance(hemp,type,start,end) ({                        \
        hemp_action _cons = (hemp_action) hemp_factory_constructor(         \
            hemp->elements, type                                            \
        );                                                                  \
        if (! _cons)                                                        \
            hemp_throw(hemp, HEMP_ERROR_INVALID, "element", type);          \
        (hemp_symbol) hemp_action_run(                                      \
            _cons, hemp_symbol_new(type, start, end)                        \
        );                                                                  \
    })


/* grammar macros */

#define hemp_register_grammar(hemp,name,constructor)                        \
    hemp_factory_register(                                                  \
        hemp->grammars,                                                     \
        name,                                                               \
        constructor,                                                        \
        hemp                                                                \
    )

#define hemp_grammar(hemp,name) ({                                          \
        hemp_grammar _gram = (hemp_grammar) hemp_factory_instance(          \
            hemp->grammars, name                                            \
        );                                                                  \
        if (! _gram) hemp_throw(hemp, HEMP_ERROR_INVALID, "grammar", name); \
        _gram;                                                              \
    })


/* dialect macros */

#define hemp_register_dialect(hemp,name,constructor)                        \
    hemp_factory_register(                                                  \
        hemp->dialects,                                                     \
        name,                                                               \
        constructor,                                                        \
        hemp                                                                \
    )

#define hemp_dialect_instance(hemp,name) ({                                 \
        hemp_dialect _dial = (hemp_dialect) hemp_factory_instance(          \
            hemp->dialects, name                                            \
        );                                                                  \
        if (! _dial) hemp_throw(hemp, HEMP_ERROR_INVALID, "dialect", name); \
        _dial;                                                              \
    })


/* view macros */

#define hemp_register_viewer(hemp,name,constructor)                         \
    hemp_factory_register(                                                  \
        hemp->viewers,                                                      \
        name,                                                               \
        constructor,                                                        \
        hemp                                                                \
    )

#define hemp_viewer_instance(hemp,name) ({                                  \
        hemp_viewer _viewer = (hemp_viewer) hemp_factory_instance(          \
            hemp->viewers, name                                             \
        );                                                                  \
        if (! _viewer) hemp_throw(hemp, HEMP_ERROR_INVALID, "viewer", name);\
        _viewer;                                                            \
    })


#if defined(__cplusplus)
}  /* extern "C" { */
#endif

#endif /* HEMP_H */

