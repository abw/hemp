#include <hemp/element.h>


hemp_symbol_p HempSymbolBlock = NULL;


hemp_symbol_p
hemp_symbol_block() {
    hemp_debug_call("hemp_symbol_block()\n");
    return hemp_element_block_symbol(
        NULL,
        hemp_symbol_init("hemp.block", NULL, NULL)
    );
}


HEMP_SYMBOL_FUNC(hemp_element_block_symbol) {
    symbol->source     = &hemp_element_block_source;
    symbol->text       = &hemp_element_block_text;
    symbol->values     = &hemp_element_block_values;
    symbol->cleanup    = &hemp_element_block_clean;
    return symbol;
}


HEMP_OUTPUT_FUNC(hemp_element_block_token) {
    hemp_debug_call("hemp_element_block_token()\n");
    hemp_text_p text;
    hemp_prepare_text(context, output, text);
    hemp_todo("hemp_element_block_token()");
    return output;
}


HEMP_OUTPUT_FUNC(hemp_element_block_source) {
    hemp_debug_call("hemp_element_block_source()\n");

    hemp_element_p element = hemp_val_elem(value);
    hemp_text_p text;
    hemp_prepare_text_size(context, output, text, element->length);

    hemp_todo("hemp_element_block_source()");

    return output;
}


HEMP_OUTPUT_FUNC(hemp_element_block_text) {
    hemp_debug_call("hemp_element_block_text()\n");
    hemp_element_p  element = hemp_val_elem(value);
    hemp_list_p     exprs   = hemp_block_exprs(element);
    hemp_value_t    item;
    hemp_size_t     n;

    hemp_text_p text;
    hemp_prepare_text(context, output, text);
    
    for (n = 0; n < exprs->length; n++) {
        item = hemp_list_item(exprs, n);
        hemp_obcall(item, text, context, output);
    }

//  hemp_debug("returning block text (%d bytes): %s\n", text->length, text->string);

    return output;
}


HEMP_VALUE_FUNC(hemp_element_block_value) {
    return hemp_element_block_text(
        value, context,
        HempNothing
    );
}


HEMP_OUTPUT_FUNC(hemp_element_block_values) {
    hemp_debug_call("hemp_element_block_text()\n");
    hemp_element_p  element = hemp_val_elem(value);
    hemp_list_p     exprs   = element->args.block.exprs;
    hemp_value_t    item;
    hemp_size_t     n;
    hemp_list_p     values;
    hemp_prepare_values(context, output, values);
    
    for (n = 0; n < exprs->length; n++) {
        item = hemp_list_item(exprs, n);
//      hemp_debug("calling values() on %s\n", expr->type->name);
        hemp_obcall(item, values, context, output);
    }

//    hemp_debug("returning block values (%d items)\n", values->length);

    return output;
}


void
hemp_element_block_clean(
    hemp_element_p element
) {
    hemp_debug_call("hemp_element_block_clean(%p)\n", element);

    hemp_list_p exprs = hemp_block_exprs(element);

    if (exprs)
        hemp_list_free(exprs);

    // hmmm... are block always synthesised and hence need freeing?
    hemp_element_free(element);
}

