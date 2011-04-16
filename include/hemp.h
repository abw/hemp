#ifndef HEMP_H
#define HEMP_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <hemp/core.h>
#include <hemp/action.h>
#include <hemp/cache.h>
#include <hemp/codec.h>
#include <hemp/context.h>
#include <hemp/dialect.h>
#include <hemp/document.h>
#include <hemp/documents.h>
#include <hemp/element.h>
#include <hemp/factory.h>
#include <hemp/filesystem.h>
#include <hemp/fragment.h>
#include <hemp/fragments.h>
#include <hemp/grammar.h>
#include <hemp/language.h>
#include <hemp/module.h>
#include <hemp/namespace.h>
#include <hemp/parser.h>
#include <hemp/proto.h>
#include <hemp/pool.h>
#include <hemp/ptree.h>
#include <hemp/scanner.h>
#include <hemp/scheme.h>
#include <hemp/scope.h>
#include <hemp/source.h>
#include <hemp/stack.h>
#include <hemp/tag.h>
#include <hemp/tagset.h>
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

//#include <hemp/module.h>



/* hemp initialisation and cleanup functions */
Hemp            hemp_new();
void            hemp_init_errors(Hemp);
void            hemp_init_config(Hemp);
void            hemp_init_factories(Hemp);
void            hemp_init_schemes(Hemp);
void            hemp_init_languages(Hemp);
void            hemp_init_viewers(Hemp);
void            hemp_init_filesystem(Hemp);

void            hemp_free(Hemp);
void            hemp_free_documents(Hemp);
void            hemp_free_factories(Hemp);
void            hemp_free_config(Hemp);
void            hemp_free_errors(Hemp);
void            hemp_free_filesystem(Hemp);

HempBool        hemp_free_dialect (HempHash, HempPos, HempSlot);
HempBool        hemp_free_document(HempHash, HempPos, HempSlot);

HempContext     hemp_context(Hemp);
HempDocuments   hemp_documents(Hemp);
HempDocument    hemp_document(Hemp, HempString, HempString, HempString);
HempFilesystem  hemp_filesystem_instance(Hemp);

void            hemp_register_tags(Hemp, HempTags);
void            hemp_register_grammars(Hemp, HempGrammars);
void            hemp_register_elements(Hemp, HempElements);

void            hemp_configure(Hemp, HempValue);
void            hemp_configure_from(Hemp, HempString, HempString, HempString);
void            hemp_ready(Hemp);
HempString      hemp_version();


HEMP_INLINE HempText
hemp_encode(
    Hemp            hemp,
    HempString      name,
    HempValue       input,
    HempContext     context
);


HEMP_INLINE HempValue
hemp_decode(
    Hemp            hemp,
    HempString      name,
    HempText        input,
    HempContext     context
);


#if defined(__cplusplus)
}  /* extern "C" { */
#endif

#endif /* HEMP_H */

