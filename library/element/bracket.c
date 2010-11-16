#include <hemp/element.h>


/*--------------------------------------------------------------------------
 * generic functions for all bracketed constructs
 *--------------------------------------------------------------------------*/

HEMP_PREFIX_FUNC(hemp_element_brackets_prefix) {
    hemp_debug_call("hemp_element_brackets_prefix()\n");
    hemp_element_p self = *elemptr;
    hemp_symbol_p  type = self->type;
    hemp_element_p block;

//    HEMP_PREFIX_PRECEDENCE;
    hemp_go_next(elemptr);

    block = hemp_element_parse_block(elemptr, scope, type->rprec, 1);

    if (! block)
        hemp_fatal("missing block for %s\n", type->start);

    hemp_set_expr_element(self, block);
    hemp_skip_whitespace(elemptr);

    if (hemp_element_terminator_matches(*elemptr, type->end)) {
//      hemp_debug("found matching terminator for %s => %s\n", type->start, type->end);
        hemp_go_next(elemptr);
    }
    else {
        hemp_fatal("missing terminator to match %s => %s\n", type->start, type->end);
    }

    return hemp_parse_postfix(elemptr, scope, precedence, force, self);
}


HEMP_POSTFIX_FUNC(hemp_element_brackets_postfix) {
    hemp_debug("TODO: hemp_element_brackets_postfix(), skipping for now\n");
    return lhs;
}


HEMP_SYMBOL_FUNC(hemp_element_brackets_symbol) {
    hemp_element_literal_symbol(hemp, symbol);
    symbol->cleanup = &hemp_element_brackets_clean;
    symbol->prefix  = &hemp_element_brackets_prefix;
    symbol->postfix = &hemp_element_brackets_postfix;
    symbol->text    = &hemp_element_value_text;
    symbol->number  = &hemp_element_value_number;
    symbol->integer = &hemp_element_value_integer;
    symbol->boolean = &hemp_element_value_boolean;
    symbol->compare = &hemp_element_value_compare;
    return symbol;
}


void
hemp_element_brackets_clean(
    hemp_element_p element
) {
    hemp_debug_call("hemp_element_brackets_clean(%p)\n", element);

    hemp_element_block_clean(
        hemp_expr_element(element)
    );
}


/*--------------------------------------------------------------------------
 * parenthesis: ( )
 *--------------------------------------------------------------------------*/

HEMP_SYMBOL_FUNC(hemp_element_parens_symbol) {
    hemp_element_brackets_symbol(hemp, symbol);
    symbol->value   = &hemp_element_parens_value;
    symbol->values  = &hemp_element_parens_values;
    symbol->parse_params = &hemp_element_parens_parse_params;
    return symbol;
}


HEMP_POSTFIX_FUNC(hemp_element_parens_parse_params) {
    hemp_debug("hemp_element_parens_parse_params()\n");
    
    hemp_element_p self = *elemptr;
    hemp_symbol_p  type = self->type;
    hemp_element_p block;

    /* skip opening parenthesis */
    hemp_go_next(elemptr);

    block = hemp_element_parse_block(elemptr, scope, type->rprec, 1);

    if (! block)
        hemp_fatal("missing block for %s\n", type->start);

    /* skip any whitespace before closing parenthesis */
    hemp_skip_whitespace(elemptr);

    if (! hemp_element_terminator_matches(*elemptr, type->end))
        hemp_fatal("missing terminator to match %s => %s\n", type->start, type->end);

    /* skip closing parenthesis */
    hemp_go_next(elemptr);

    /* IMPORTANT: must set parens expr to NULL so that destructor doesn't
     * get confused and segfault.  This is because we're returning the block
     * we just parsed.... maybe we should return this parens element
     * instead?
     */
    hemp_set_expr_element(self, block);

    return self;
}


HEMP_POSTFIX_FUNC(hemp_element_parens_postfix) {
    hemp_debug("TODO: hemp_element_parens_postfix()\n");
    return lhs;
}


HEMP_VALUE_FUNC(hemp_element_parens_value) {
    hemp_debug_call("hemp_element_parens_value()\n");
    hemp_element_p element  = hemp_val_elem(value);
    hemp_value_t   block    = hemp_expr(element);
    hemp_value_t   values   = hemp_obcall(block, values, context, HempNothing);
    hemp_list_p    list     = hemp_val_list(values);

    hemp_debug("got %d values returned by parens\n", list->length);

    if (list->length > 1) {
//      hemp_debug("squishing list of %d items to text\n", list->length);
        return hemp_text(values, context, HempNothing);
    }
    else if (list->length == 1) {
//      hemp_debug("got one value returned by parens\n");
        return hemp_list_item(list, 0);
    }
    else {
        hemp_debug("nothing in list\n");
        return HempEmpty;
    }
}


HEMP_OUTPUT_FUNC(hemp_element_parens_values) {
    hemp_debug_call("hemp_element_parens_values()\n");
    hemp_element_p element  = hemp_val_elem(value);
    hemp_value_t   block    = hemp_expr(element);
    return hemp_obcall(block, values, context, output);
}



/*--------------------------------------------------------------------------
 * square brackets: [ ]
 *--------------------------------------------------------------------------*/

HEMP_SYMBOL_FUNC(hemp_element_list_symbol) {
    hemp_element_brackets_symbol(hemp, symbol);
    symbol->value = &hemp_element_list_value;
    return symbol;
}



HEMP_VALUE_FUNC(hemp_element_list_value) {
    hemp_debug("hemp_element_list_value()\n");
    hemp_element_p element  = hemp_val_elem(value);
    hemp_value_t   block    = hemp_expr(element);
    return hemp_obcall(block, values, context, HempNothing);
}


/*--------------------------------------------------------------------------
 * hash
 *--------------------------------------------------------------------------*/

HEMP_SYMBOL_FUNC(hemp_element_hash_symbol) {
    hemp_element_brackets_symbol(hemp, symbol);
    symbol->value = &hemp_element_hash_value;
    return symbol;
}


HEMP_VALUE_FUNC(hemp_element_hash_value) {
    hemp_debug("hemp_element_hash_value()\n");
    hemp_element_p element  = hemp_val_elem(value);
    hemp_element_p block    = hemp_expr_element(element);
    // pairs/slots = block->type->pairs(block, context, HempNothing);
    // add pairs/slots to a hash, return.
    // or better still, pass hash to pairs()... but what if we want a list
    // of pairs/slots instead?
    // maybe call ->pairs to return a list of pairs and then hashify them
}
