#include <hemp/element.h>

struct hemp_symbol_s
    hemp_symbol_text = { 
        "text",
        "text:",
        HEMP_IS_FIXED | HEMP_IS_STATIC, 0, 0,
        NULL, NULL,
        &hemp_skip_none_vtable,
        NULL, NULL,
        &hemp_element_literal_text,
        &hemp_element_literal_parse_expr
    };

hemp_symbol_p HempSymbolText = &hemp_symbol_text;


/* TODO: perhaps move this as it only applies to FIXED literal text
 * like squote and dquote 
 */

void
hemp_element_text_clean(
    hemp_element_p element
) {
    debug_call("hemp_element_text_clean(%p)\n", element);

    /* squote and dquote elements may have allocated memory to accomodate
     * translated escape sequences, e.g. \n \\, etc
     */
    if (hemp_not_flag(element, HEMP_IS_FIXED)) {
        hemp_mem_free((hemp_mem_p) element->value.text);
    }
}

