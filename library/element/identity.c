#include <hemp/element.h>


HEMP_VALUE_FUNC(hemp_element_identity_value);

/*--------------------------------------------------------------------------
 * Macro hack to generate symbol and value functions for various identities
 *--------------------------------------------------------------------------*/

#define HEMP_IDENTITY_SYMBOL(name, result)                      \
    HEMP_VALUE_FUNC(hemp_element_identity_##name##_value) {     \
        hemp_element element = hemp_val_elem(value);            \
        return hemp_has_flag(element, HEMP_BE_FIXED)            \
            ? hemp_expr(element)                                \
            : result;                                           \
    }                                                           \
    HEMP_SYMBOL(hemp_element_identity_##name) {                 \
        hemp_element_word_symbol(hemp, symbol);                 \
        symbol->value = &hemp_element_identity_##name##_value;  \
        return symbol;                                          \
    }                                                           \

HEMP_IDENTITY_SYMBOL(true,      HempTrue);
HEMP_IDENTITY_SYMBOL(false,     HempFalse);
HEMP_IDENTITY_SYMBOL(empty,     HempEmpty);
HEMP_IDENTITY_SYMBOL(nothing,   HempNothing);
HEMP_IDENTITY_SYMBOL(missing,   HempMissing);
HEMP_IDENTITY_SYMBOL(before,    HempBefore);
HEMP_IDENTITY_SYMBOL(after,     HempAfter);
HEMP_IDENTITY_SYMBOL(equal,     HempEqual);



/*--------------------------------------------------------------------------
 * Generic identity element - no longer used (but I might change my mind)
 *--------------------------------------------------------------------------*/

//HEMP_SYMBOL(hemp_element_identity) {
//    hemp_element_word_symbol(hemp, symbol);
//    symbol->value = &hemp_element_identity_value;
//    return symbol;
//}
//
//
//HEMP_VALUE_FUNC(hemp_element_identity_value) {
//    hemp_debug_call("hemp_element_identity_value()\n");
//    hemp_element element = hemp_val_elem(value);
//
//    /* If the element was parsed as a fixed expression (e.g. lvalue for 
//     * assignment or following a dotop) then we return the fixed word in 
//     * the unary expression slot.  Otherwise we return the identity value 
//     * stored in the arguments slot.
//     *
//     * UPDATE: nah, maybe not... that means intercepting the parse_prefix()
//     * method to store the identity value in the args slot.  I think I'll
//     * go with hard-coded value methods for the identity elements for now.
//     */
//
//    return hemp_has_flag(element, HEMP_BE_FIXED)
//        ? hemp_expr(element)
//        : hemp_expr_args(element);
//}

