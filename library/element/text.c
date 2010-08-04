#include <hemp/elements.h>


static struct hemp_etype_s
    hemp_element_text = { 
        "text",
        HEMP_IS_FIXED | HEMP_IS_STATIC,
        NULL,
        &hemp_element_dont_skip,                 /* not skippable */
        &hemp_element_dont_skip,
        &hemp_element_dont_skip,
        &hemp_element_literal_parse_expr,
        &hemp_element_literal_text
    };

hemp_etype_p HempElementText = &hemp_element_text;


/* TODO: perhaps move this as it only applies to FIXED literal text
 * like squote and dquote 
 */

void
hemp_element_text_clean(
    hemp_element_p element
) {
//  debug_call("hemp_element_text_clean(%p)\n", element);

    /* squote and dquote elements may have allocated memory to accomodate
     * translated escape sequences, e.g. \n \\, etc
     */
    if (hemp_not_flag(element, HEMP_IS_FIXED)) {
        hemp_mem_free((hemp_mem_p) element->value.text);
    }
}

