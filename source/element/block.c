#include "hemp/elements.h"
#include "hemp/debug.h"


static struct hemp_etype
    hemp_element_block = { 
        "block",
        &hemp_element_dont_skip,
        &hemp_element_dont_skip,
        &hemp_element_dont_skip,
        &hemp_element_parse_expr,
        &hemp_element_block_text
    };


hemp_etype_t HempElementBlock = &hemp_element_block;


hemp_text_t
hemp_element_block_text(
    hemp_element_t  element,
    hemp_text_t     text
) {
    debug_call("hemp_element_block_text()\n");
    hemp_list_t     exprs = element->value.block.exprs;
    hemp_element_t  expr;
    hemp_size_t     n;
    
    if (! text)
        text = hemp_text_init(0);

    for (n = 0; n < exprs->length; n++) {
        expr = hemp_list_item(exprs, n);
        expr->type->text(expr, text);
    }

    return text;
}


