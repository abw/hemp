#include <hemp/dialect.h>
#include <hemp/scanner.h>
#include <hemp/grammar.h>


/* TODO: move all this out into a separate tt3 framework/system/setup which
 * defines all elements, grammars, etc
 */
 

hemp_dialect_p
OLD_hemp_dialect_tt3(
    hemp_p      hemp,
    hemp_cstr_p name
) {
    hemp_dialect_p dialect = hemp_dialect_init(hemp, name);
    hemp_fatal("old dialect/tt3.c is no longer in use");
}

