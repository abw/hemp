#include <hemp/element.h>


HEMP_VALUE(hemp_element_identity_value);


/*--------------------------------------------------------------------------
 * Macro hack to generate element and value functions for various identities
 *--------------------------------------------------------------------------*/

#define HEMP_IDENTITY_ELEMENT(name, result)                     \
    HEMP_VALUE(hemp_element_identity_##name##_value) {          \
        hemp_fragment fragment = hemp_val_frag(value);          \
        return hemp_has_flag(fragment, HEMP_BE_FIXED)           \
            ? hemp_expr(fragment)                               \
            : result;                                           \
    }                                                           \
    HEMP_ELEMENT(hemp_element_identity_##name) {                \
        hemp_element_word(hemp, element);                       \
        element->value = &hemp_element_identity_##name##_value; \
        return element;                                         \
    }                                                           \

HEMP_IDENTITY_ELEMENT(true,      HempTrue);
HEMP_IDENTITY_ELEMENT(false,     HempFalse);
HEMP_IDENTITY_ELEMENT(empty,     HempEmpty);
HEMP_IDENTITY_ELEMENT(nothing,   HempNothing);
HEMP_IDENTITY_ELEMENT(missing,   HempMissing);
HEMP_IDENTITY_ELEMENT(before,    HempBefore);
HEMP_IDENTITY_ELEMENT(after,     HempAfter);
HEMP_IDENTITY_ELEMENT(equal,     HempEqual);


