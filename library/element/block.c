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


/*--------------------------------------------------------------------------
 * block
 *--------------------------------------------------------------------------*/

HEMP_SYMBOL_FUNC(hemp_element_block_symbol) {
    symbol->source     = &hemp_element_block_source;
    symbol->value      = &hemp_element_block_value;
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
    hemp_list_p     exprs   = hemp_block_exprs_list(element);
    hemp_value_t    item;
    hemp_size_t     n;

    hemp_text_p text;
    hemp_prepare_text(context, output, text);

    if (hemp_has_flag(element, HEMP_BE_ARGS)) {
        hemp_element_p  args    = hemp_val_elem(hemp_block_args(element));
        hemp_params_p   params  = (hemp_params_p) hemp_val_ptr(hemp_lhs(args));
        hemp_list_p     list    = params->item;
        hemp_debug("TODO: handle block arguments, params are at %p\n", params);
        for (n = 0; n < list->length; n++) {
            hemp_str_p name = hemp_val_str( hemp_list_item(list, n) );
            hemp_debug("- %s\n", name);
        }
        // hemp_value_t args = hemp_block_args(element);
    }
    
    for (n = 0; n < exprs->length; n++) {
        item = hemp_list_item(exprs, n);
        hemp_obcall(item, text, context, output);
    }

//    hemp_debug("returning block text (%d bytes): %s\n", text->length, text->string);

    return output;
}


HEMP_VALUE_FUNC(hemp_element_block_value) {
    return hemp_element_block_text(
        value, context,
        HempNothing
    );
}


HEMP_OUTPUT_FUNC(hemp_element_block_values) {
    hemp_debug_call("hemp_element_block_values()\n");
    hemp_element_p  element = hemp_val_elem(value);
    hemp_list_p     exprs   = hemp_block_exprs_list(element);
    hemp_value_t    item;
    hemp_size_t     n;
    hemp_list_p     values;
    hemp_prepare_values(context, output, values);
    
    for (n = 0; n < exprs->length; n++) {
        item = hemp_list_item(exprs, n);
//      hemp_debug_msg("calling values() on item %d: %s\n", n, hemp_obtype(item)->name);
        hemp_obcall(item, values, context, output);
    }

//  hemp_debug_msg("returning block values (%d items)\n", values->length);

    return output;
}


HEMP_OUTPUT_FUNC(hemp_element_block_params) {
    hemp_debug_call("hemp_element_block_params()\n");
    hemp_element_p  element = hemp_val_elem(value);
    hemp_list_p     exprs   = hemp_block_exprs_list(element);
    hemp_value_t    item;
    hemp_size_t     n;
    hemp_list_p     values;
    hemp_prepare_values(context, output, values);
    
    for (n = 0; n < exprs->length; n++) {
        item = hemp_list_item(exprs, n);
//      hemp_debug_msg("calling params() on %s\n", hemp_val_elem(item)->type->name);
        hemp_obcall(item, params, context, output);
    }

    return output;
}


void
hemp_element_block_clean(
    hemp_element_p element
) {
    hemp_debug_call("hemp_element_block_clean(%p)\n", element);

    hemp_list_p exprs = hemp_block_exprs_list(element);

    if (exprs)
        hemp_list_free(exprs);

    if (hemp_has_flag(element, HEMP_BE_ARGS)) {
        hemp_element_p  args    = hemp_val_elem(hemp_block_args(element));
        hemp_params_p   params  = (hemp_params_p) hemp_val_ptr(hemp_lhs(args));
//      hemp_debug("cleaning block params at %p\n", params);
        hemp_params_free(params);
    }

    // hmmm... are block always synthesised and hence need freeing?
    // TODO: allocate block elements via the elements object so that it 
    // can memory manage them en masse.
    hemp_element_free(element);
}



/*--------------------------------------------------------------------------
 * function
 *--------------------------------------------------------------------------*/

HEMP_SYMBOL_FUNC(hemp_element_function_symbol) {
    hemp_debug_call("hemp_element_function_symbol()\n");
//  symbol->source      = &hemp_element_function_source;
    symbol->value       = &hemp_value_self;
//    symbol->text        = &hemp_element_function_text;
//    symbol->apply       = &hemp_element_function_apply;
//    symbol->values      = &hemp_element_value_values;
//    symbol->cleanup     = &hemp_element_function_clean;
//  symbol->flags       = HEMP_BE_SYNTHETIC;
    return symbol;
}

