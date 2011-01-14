#include <hemp/element.h>


/*--------------------------------------------------------------------------
 * global element types
 *--------------------------------------------------------------------------*/

HempElement HempElementBlock = NULL;


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
    element->pairs   = &hemp_element_block_pairs;
    element->params  = &hemp_element_block_params;
    element->cleanup = &hemp_element_block_cleanup;
    return element;
}


HEMP_OUTPUT(hemp_element_block_token) {
    hemp_debug_call("hemp_element_block_token()\n");
    HempText text;
    hemp_prepare_text(context, output, text);
    hemp_todo("hemp_element_block_token()");
    return output;
}


HEMP_OUTPUT(hemp_element_block_source) {
    hemp_debug_call("hemp_element_block_source()\n");

    HempFragment fragment = hemp_val_frag(value);
    HempText text;
    hemp_prepare_text_size(context, output, text, fragment->length);

    hemp_todo("hemp_element_block_source()");

    return output;
}


HEMP_OUTPUT(hemp_element_block_text) {
    hemp_debug_call("hemp_element_block_text()\n");
    HempFragment fragment = hemp_val_frag(value);
    HempList     exprs    = hemp_block_exprs_list(fragment);
    HempValue    item;
    HempSize     n;

    HempText text;
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
    HempFragment fragment = hemp_val_frag(value);
    HempList     exprs    = hemp_block_exprs_list(fragment);
    HempValue    item;
    HempSize     n;
    HempList     values;
    hemp_prepare_values(context, output, values);
    
    for (n = 0; n < exprs->length; n++) {
        item = hemp_list_item(exprs, n);
        hemp_obcall(item, values, context, output);
    }

    return output;
}


HEMP_OUTPUT(hemp_element_block_params) {
    hemp_debug_call("hemp_element_block_params()\n");
    HempFragment fragment = hemp_val_frag(value);
    HempList     exprs    = hemp_block_exprs_list(fragment);
    HempValue    item;
    HempSize     n;
    HempList     values;
    hemp_prepare_values(context, output, values);
    
    for (n = 0; n < exprs->length; n++) {
        item = hemp_list_item(exprs, n);
        hemp_call(item, params, context, output);
    }

    return output;
}


HEMP_OUTPUT(hemp_element_block_pairs) {
    hemp_debug_call("hemp_element_block_pairs()\n");
    HempFragment fragment = hemp_val_frag(value);
    HempList     exprs    = hemp_block_exprs_list(fragment);
    HempHash     pairs;
    HempValue    item;
    HempSize     n;
    hemp_prepare_pairs(context, output, pairs);
    
    for (n = 0; n < exprs->length; n++) {
        item = hemp_list_item(exprs, n);
        hemp_call(item, pairs, context, output);
    }

    return output;
}


HEMP_CLEANUP(hemp_element_block_cleanup) {
    hemp_debug_call("hemp_element_block_clean(%p)\n", fragment);

    HempList exprs = hemp_block_exprs_list(fragment);

    if (exprs)
        hemp_list_free(exprs);
}


