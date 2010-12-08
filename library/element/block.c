#include <hemp/element.h>


/*--------------------------------------------------------------------------
 * global element types
 *--------------------------------------------------------------------------*/

hemp_element HempElementBlock = NULL;


HEMP_GLOBAL_ELEMENT(hemp_global_element_block) {
    return hemp_element_block(
        NULL,
        hemp_element_new("hemp.block", NULL, NULL)
    );
}


/*--------------------------------------------------------------------------
 * block
 *--------------------------------------------------------------------------*/

HEMP_ELEMENT(hemp_element_block) {
    element->source  = &hemp_element_block_source;
    element->value   = &hemp_element_block_value;
    element->text    = &hemp_element_block_text;
    element->values  = &hemp_element_block_values;
    element->cleanup = &hemp_element_block_cleanup;
    return element;
}


HEMP_OUTPUT(hemp_element_block_token) {
    hemp_debug_call("hemp_element_block_token()\n");
    hemp_text text;
    hemp_prepare_text(context, output, text);
    hemp_todo("hemp_element_block_token()");
    return output;
}


HEMP_OUTPUT(hemp_element_block_source) {
    hemp_debug_call("hemp_element_block_source()\n");

    hemp_fragment fragment = hemp_val_frag(value);
    hemp_text text;
    hemp_prepare_text_size(context, output, text, fragment->length);

    hemp_todo("hemp_element_block_source()");

    return output;
}


HEMP_OUTPUT(hemp_element_block_text) {
    hemp_debug_call("hemp_element_block_text()\n");
    hemp_fragment fragment = hemp_val_frag(value);
    hemp_list     exprs    = hemp_block_exprs_list(fragment);
    hemp_value    item;
    hemp_size     n;

    hemp_text text;
    hemp_prepare_text(context, output, text);

    for (n = 0; n < exprs->length; n++) {
        item = hemp_list_item(exprs, n);
        hemp_obcall(item, text, context, output);
    }

    return output;
}


HEMP_VALUE(hemp_element_block_value) {
    return hemp_element_block_text(
        value, context,
        HempNothing
    );
}


HEMP_OUTPUT(hemp_element_block_values) {
    hemp_debug_call("hemp_element_block_values()\n");
    hemp_fragment fragment = hemp_val_frag(value);
    hemp_list     exprs    = hemp_block_exprs_list(fragment);
    hemp_value    item;
    hemp_size     n;
    hemp_list     values;
    hemp_prepare_values(context, output, values);
    
    for (n = 0; n < exprs->length; n++) {
        item = hemp_list_item(exprs, n);
        hemp_obcall(item, values, context, output);
    }

    return output;
}


HEMP_OUTPUT(hemp_element_block_params) {
    hemp_debug_call("hemp_element_block_params()\n");
    hemp_fragment fragment = hemp_val_frag(value);
    hemp_list     exprs    = hemp_block_exprs_list(fragment);
    hemp_value    item;
    hemp_size     n;
    hemp_list     values;
    hemp_prepare_values(context, output, values);
    
    for (n = 0; n < exprs->length; n++) {
        item = hemp_list_item(exprs, n);
        hemp_call(item, params, context, output);
    }

    return output;
}


HEMP_CLEANUP(hemp_element_block_cleanup) {
    hemp_debug_call("hemp_element_block_clean(%p)\n", fragment);

    hemp_list exprs = hemp_block_exprs_list(fragment);

    if (exprs)
        hemp_list_free(exprs);
}


