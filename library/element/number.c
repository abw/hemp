#include <hemp/element.h>


static struct hemp_etype_s
    hemp_element_number = { 
        "number",
        HEMP_IS_STATIC,
        NULL,
        &hemp_element_dont_skip,
        &hemp_element_dont_skip,
        &hemp_element_dont_skip,
        &hemp_element_literal_parse_expr,
        &hemp_element_literal_text
    };

hemp_etype_p HempElementNumber = &hemp_element_number;


hemp_mem_p
hemp_element_number_constructor(
    hemp_p      hemp,
    hemp_cstr_p name
) {
    debug_call("hemp_element_number_constructor(%p, %s)\n", hemp, name);
    return "TODO: hemp_element_number_constructor()";
}

