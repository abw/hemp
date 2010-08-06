#include <hemp/element.h>

struct hemp_symbol_s
    hemp_symbol_number = { 
        "number",
        "number:",
        HEMP_IS_STATIC, 0, 0,
        NULL, NULL,
        &hemp_skip_none_vtable,
        NULL, NULL,
        &hemp_element_literal_text,
        &hemp_element_number_parse_expr
    };

hemp_symbol_p HempSymbolNumber = &hemp_symbol_number;

hemp_element_p
hemp_element_number_parse_expr(
    HEMP_PARSE_PROTO
) {
    debug_blue("hemp_element_number_parse_expr() precedence is %d, parg: %d\n", (*elemptr)->type->lprec, precedence);
    
    hemp_element_p element = *elemptr;
  
    if (hemp_has_next(elemptr)) {
        hemp_go_next(elemptr);
        hemp_skip_space(elemptr);
        hemp_symbol_p sym = (*elemptr)->type;

        if (sym->parse && sym->parse->infix) {
//          return hemp_parse_infix(HEMP_PARSE_ARGS, element);

            return hemp_parse_infix(elemptr, scope, precedence, force, element);
        }
    }

    return element;
}


