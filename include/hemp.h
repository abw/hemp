#ifndef HEMP_H
#define HEMP_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <hemp/core.h>
#include <hemp/action.h>
#include <hemp/codec.h>
#include <hemp/context.h>
#include <hemp/dialect.h>
#include <hemp/element.h>
#include <hemp/factory.h>
#include <hemp/filesystem.h>
#include <hemp/fragment.h>
#include <hemp/fragments.h>
#include <hemp/grammar.h>
#include <hemp/language.h>
#include <hemp/module.h>
#include <hemp/namespace.h>
#include <hemp/proto.h>
#include <hemp/pool.h>
#include <hemp/ptree.h>
#include <hemp/scanner.h>
#include <hemp/scheme.h>
#include <hemp/scope.h>
#include <hemp/source.h>
#include <hemp/tag.h>
#include <hemp/tagset.h>
#include <hemp/template.h>
#include <hemp/uri.h>
#include <hemp/value.h>
#include <hemp/viewer.h>
#include <hemp/macros.h>
#include <hemp/scheme/text.h>
#include <hemp/scheme/file.h>
#include <hemp/type/text.h>
#include <hemp/type/list.h>
#include <hemp/type/hash.h>
#include <hemp/type/code.h>
#include <hemp/type/params.h>
#include <hemp/language/hemp.h>
#include <hemp/language/tt3.h>
#include <hemp/language/test.h>

//#include <hemp/module.h>



/* hemp initialisation and cleanup functions */
hemp_hemp       hemp_new();
void            hemp_init_errors(hemp_hemp);
void            hemp_init_factories(hemp_hemp);
void            hemp_init_schemes(hemp_hemp);
void            hemp_init_languages(hemp_hemp);
void            hemp_init_templates(hemp_hemp);
void            hemp_init_viewers(hemp_hemp);

void            hemp_free(hemp_hemp);
void            hemp_free_templates(hemp_hemp);
void            hemp_free_factories(hemp_hemp);
void            hemp_free_errors(hemp_hemp);

hemp_bool       hemp_free_codec   ( hemp_hash, hemp_pos, hemp_slot );
hemp_bool       hemp_free_dialect ( hemp_hash, hemp_pos, hemp_slot );
hemp_bool       hemp_free_element ( hemp_hash, hemp_pos, hemp_slot );
hemp_bool       hemp_free_grammar ( hemp_hash, hemp_pos, hemp_slot );
hemp_bool       hemp_free_language( hemp_hash, hemp_pos, hemp_slot );
hemp_bool       hemp_free_scheme  ( hemp_hash, hemp_pos, hemp_slot );
hemp_bool       hemp_free_tag     ( hemp_hash, hemp_pos, hemp_slot );
hemp_bool       hemp_free_template( hemp_hash, hemp_pos, hemp_slot );
hemp_bool       hemp_free_viewer  ( hemp_hash, hemp_pos, hemp_slot );

hemp_context    hemp_context_instance(hemp_hemp);
hemp_template   hemp_template_instance(hemp_hemp, hemp_string, hemp_string, hemp_string);

void            hemp_register_dialects(hemp_hemp, hemp_dialects);
void            hemp_register_tags(hemp_hemp, hemp_tags);
void            hemp_register_grammars(hemp_hemp, hemp_grammars);

void            hemp_scan_error(hemp_hemp, HEMP_SCAN_ARGS, hemp_errno, ...);

hemp_string     hemp_version();

void hemp_register_elements(hemp_hemp, hemp_elements);

HEMP_INLINE hemp_text
hemp_encode(
    hemp_hemp       hemp,
    hemp_string     name,
    hemp_value      input,
    hemp_context    context
);


HEMP_INLINE hemp_value
hemp_decode(
    hemp_hemp       hemp,
    hemp_string     name,
    hemp_text       input,
    hemp_context    context
);




#if defined(__cplusplus)
}  /* extern "C" { */
#endif

#endif /* HEMP_H */

