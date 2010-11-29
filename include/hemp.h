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
#include <hemp/scheme/text.h>
#include <hemp/scheme/file.h>
#include <hemp/type/text.h>
#include <hemp/type/list.h>
#include <hemp/type/hash.h>
#include <hemp/type/code.h>
#include <hemp/language/test.h>

//#include <hemp/module.h>



/* hemp initialisation and cleanup functions */
hemp_hemp   hemp_init();
void        hemp_init_factories(hemp_hemp);
void        hemp_init_schemes(hemp_hemp);
void        hemp_init_languages(hemp_hemp);
void        hemp_init_errors(hemp_hemp);

void        hemp_free(hemp_hemp);
void        hemp_free_factories(hemp_hemp);
hemp_bool   hemp_free_dialect ( hemp_hash, hemp_pos, hemp_slot );
hemp_bool   hemp_free_element ( hemp_hash, hemp_pos, hemp_slot );
hemp_bool   hemp_free_grammar ( hemp_hash, hemp_pos, hemp_slot );
hemp_bool   hemp_free_language( hemp_hash, hemp_pos, hemp_slot );
hemp_bool   hemp_free_scheme  ( hemp_hash, hemp_pos, hemp_slot );
hemp_bool   hemp_free_template( hemp_hash, hemp_pos, hemp_slot );
hemp_bool   hemp_free_viewer  ( hemp_hash, hemp_pos, hemp_slot );

/* scheme functions */

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


/*--------------------------------------------------------------------------
 * Macros for registering component constructors and instantiating components
 *--------------------------------------------------------------------------*/

#define hemp_register(hemp, type, name, constructor)                        \
    hemp_factory_register(                                                  \
        hemp->type,                                                         \
        name,                                                               \
        (hemp_actor) constructor,                                           \
        hemp                                                                \
    )

#define hemp_instance(hemp, type, name) ({                                  \
    hemp_memory _item = hemp_factory_instance(                              \
        hemp->type,                                                         \
        name                                                                \
    );                                                                      \
    if (! _item)                                                            \
        hemp_throw(hemp, HEMP_ERROR_INVALID, #type, name);                  \
    _item;                                                                  \
})    


#define hemp_register_dialect(hemp, name, constructor)                      \
    hemp_register(hemp, dialect, name, constructor)

#define hemp_register_element(hemp, name, constructor)                      \
    hemp_register(hemp, element, name, constructor)

#define hemp_register_grammar(hemp, name, constructor)                      \
    hemp_register(hemp, grammar, name, constructor)

#define hemp_register_language(hemp, name, constructor)                     \
    hemp_register(hemp, language, name, constructor)

#define hemp_register_scheme(hemp, name, constructor)                       \
    hemp_register(hemp, scheme, name, constructor)

#define hemp_register_viewer(hemp, name, constructor)                       \
    hemp_register(hemp, viewer, name, constructor)


#define hemp_dialect_instance(hemp, name)                                   \
    hemp_instance(hemp, dialect, name)

#define hemp_grammar_instance(hemp, name)                                   \
    hemp_instance(hemp, grammar, name)

#define hemp_language_instance(hemp, name)                                  \
    hemp_instance(hemp, language, name)

#define hemp_scheme_instance(hemp, name)                                    \
    hemp_instance(hemp, scheme, name)

#define hemp_viewer_instance(hemp, name)                                    \
    hemp_instance(hemp, viewer, name)


#define hemp_symbol_instance(hemp,type,start,end) ({                        \
        hemp_action _cons = (hemp_action) hemp_factory_constructor(         \
            hemp->element, type                                             \
        );                                                                  \
        if (! _cons)                                                        \
            hemp_throw(hemp, HEMP_ERROR_INVALID, "element", type);          \
        (hemp_symbol) hemp_action_run(                                      \
            _cons, hemp_symbol_new(type, start, end)                        \
        );                                                                  \
    })

#define hemp_source_instance(hemp, scheme, source)                          \
    hemp_source_new(                                                        \
        hemp_scheme_instance(hemp, scheme),                                 \
        source                                                              \
    )


#if defined(__cplusplus)
}  /* extern "C" { */
#endif

#endif /* HEMP_H */

