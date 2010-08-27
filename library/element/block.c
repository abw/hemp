#include <hemp/element.h>


static struct hemp_symbol_s
    hemp_symbol_block = { 
        "block",                                /* name                     */
        NULL,                                   /* start token              */
        NULL,                                   /* end token                */
        0,                                      /* flags                    */
        0, 0,                                   /* l/r precedence           */
        NULL,                                   /* scanner callback         */
        &hemp_element_block_clean,              /* cleanup callback         */
        &hemp_element_not_prefix,               /* prefix expr parser       */      // CHECK ME
        &hemp_element_not_postfix,              /* parse postfix expr       */
        &hemp_element_block_token,              /* source token(s)          */
        &hemp_element_block_source,             /* source code              */
        &hemp_element_block_text,               /* output text              */
        &hemp_element_block_value,              /* output value             */
        &hemp_element_not_number,               /* numeric conversion       */
        &hemp_element_not_integer,              /* integer conversion       */
        &hemp_element_not_boolean,              /* boolean conversion       */
        &hemp_element_not_compare,              /* comparison conversion    */
    };

hemp_symbol_p HempSymbolBlock = &hemp_symbol_block;


HEMP_SYMBOL_FUNC(hemp_element_block_symbol) {
    symbol->source     = &hemp_element_block_source;
    symbol->text       = &hemp_element_block_text;
//   symbol->cleanup    = &hemp_element_block_clean;       // FIXME
    return symbol;
}


HEMP_ETEXT_FUNC(hemp_element_block_token) {
    hemp_debug_call("hemp_element_block_token()\n");
    hemp_text_p text;
    hemp_prepare_output(output, text, 0);
    hemp_todo("hemp_element_block_token()");
    return output;
}


HEMP_ETEXT_FUNC(hemp_element_block_source) {
    hemp_debug_call("hemp_element_block_source()\n");

    hemp_text_p text;
    hemp_prepare_output(output, text, element->length);

    hemp_todo("hemp_element_block_source()");

    return output;
}


HEMP_ETEXT_FUNC(hemp_element_block_text) {
    hemp_debug_call("hemp_element_block_text()\n");
    hemp_list_p     exprs = element->args.block.exprs;
    hemp_element_p  expr;
    hemp_size_t     n;

    hemp_text_p text;
    hemp_prepare_output(output, text, 0);
    
    for (n = 0; n < exprs->length; n++) {
        expr = (hemp_element_p) hemp_val_ptr( hemp_list_item(exprs, n) );
        expr->type->text(expr, context, output);
    }

//  hemp_debug("returning block text (%d bytes): %s\n", text->length, text->string);

    return output;
}


HEMP_EVAL_FUNC(hemp_element_block_value) {
    return hemp_element_block_text(HEMP_EVAL_ARG_NAMES, HempNothing);
}


void
hemp_element_block_clean(
    hemp_element_p element
) {
    hemp_debug_call("hemp_element_block_clean(%p)\n", element);

    hemp_list_p exprs = element->args.block.exprs;

    if (exprs)
        hemp_list_free(exprs);

    // hmmm... are block always synthesised and hence need freeing?
    hemp_element_free(element);
}

