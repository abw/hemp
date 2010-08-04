#include <hemp/element.h>

void
    hemp_element_block_clean(
        hemp_element_p element
    );


static struct hemp_etype_s
    hemp_element_block = { 
        "block",
        0,
        &hemp_element_block_clean,
        &hemp_element_dont_skip,
        &hemp_element_dont_skip,
        &hemp_element_dont_skip,
        &hemp_element_parse_expr,
        &hemp_element_block_text
    };


hemp_etype_p HempElementBlock = &hemp_element_block;


hemp_text_p
hemp_element_block_text(
    hemp_element_p  element,
    hemp_text_p     text
) {
    debug_call("hemp_element_block_text()\n");
//  debug("*** hemp_element_block_text()\n");
    hemp_list_p     exprs = element->value.block.exprs;
    hemp_element_p  expr;
    hemp_size_t     n;
    
    if (! text) {
//      debug("new text item\n");
        text = hemp_text_init(0);
    }

    for (n = 0; n < exprs->length; n++) {
        expr = hemp_list_item(exprs, n);
//      debug("calling %s text method\n", expr->type->name);
        expr->type->text(expr, text);
    }

//  debug("returning block text (%d bytes): %p\n", text->length, text);

    return text;
}


void
hemp_element_block_clean(
    hemp_element_p element
) {
    debug_call("hemp_element_block_clean(%p)\n", element);
    hemp_list_p exprs = element->value.block.exprs;
    if (exprs)
        hemp_list_free(exprs);

    // hmmm... are block always synthesised and hence need freeing?
    hemp_element_free(element);
}

