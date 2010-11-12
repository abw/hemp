#include <hemp/element.h>


/*--------------------------------------------------------------------------
 * generic functions for all bracketed constructs
 *--------------------------------------------------------------------------*/

HEMP_PREFIX_FUNC(hemp_element_brackets_prefix) {
    hemp_debug("hemp_element_brackets_prefix()\n");
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
        hemp_debug("found matching terminator for %s => %s\n", type->start, type->end);
        hemp_go_next(elemptr);
    }
    else {
        hemp_fatal("missing terminator to match %s => %s\n", type->start, type->end);
    }

    return hemp_parse_postfix(elemptr, scope, precedence, force, self);
}

HEMP_POSTFIX_FUNC(hemp_element_brackets_postfix) {
    hemp_debug("TODO: hemp_element_brackets_prefix()\n");
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
    hemp_debug("hemp_element_brackets_clean(%p)\n", element);

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
    return symbol;
}


HEMP_POSTFIX_FUNC(hemp_element_parens_postfix) {
    hemp_debug("TODO: hemp_element_parens_postfix()\n");
    return NULL;
}


HEMP_EVAL_FUNC(hemp_element_parens_value) {
    hemp_debug("hemp_element_parens_value()\n");
    hemp_element_p block  = hemp_expr_element(element);
    hemp_value_t   values = block->type->values(block, context, HempNothing);
    hemp_list_p    list   = hemp_val_list(values);
    hemp_debug("got %d values returned by parens\n", list->length);
    if (list->length > 1) {
        hemp_debug("TODO: squish list (%d items) to text\n", list->length);
    }
    else if (list->length == 1) {
        hemp_debug("got one value returned by parens\n");
        return hemp_list_item(list, 0);
    }
    else {
        hemp_debug("nothing in list\n");
        return HempNothing;
    }
    return HempNothing;
}


HEMP_EVALS_FUNC(hemp_element_parens_values) {
    hemp_debug("hemp_element_parens_values()\n");
//    hemp_element_p block  = hemp_expr_element(element);
//    hemp_value_t   values = block->type->values(block, context, HempNothing);
//    hemp_list_p    list   = hemp_val_list(values);
//    hemp_debug("got %s values returned by parens\n", hemp_val_list(values)->length);
//    return hemp_num_val(42);
    return HempNothing;
}



/*--------------------------------------------------------------------------
 * square brackets: [ ]
 *--------------------------------------------------------------------------*/

HEMP_SYMBOL_FUNC(hemp_element_list_symbol) {
    hemp_element_literal_symbol(hemp, symbol);
/*
    TODO
    symbol->cleanup = &hemp_element_text_clean;
    symbol->prefix  = &hemp_element_list_prefix;
    symbol->postfix = &hemp_element_list_postfix;
    symbol->text    = &hemp_element_value_text;
    symbol->value   = &hemp_element_list_value;
    symbol->number  = &hemp_element_value_number;
    symbol->integer = &hemp_element_value_integer;
    symbol->boolean = &hemp_element_value_boolean;
    symbol->compare = &hemp_element_value_compare;
*/
    return symbol;
}



HEMP_EVAL_FUNC(hemp_element_list_value) {
    hemp_debug("hemp_element_list_value()\n");
    return HempNothing;
}


/*--------------------------------------------------------------------------
 * hash
 *--------------------------------------------------------------------------*/

HEMP_SYMBOL_FUNC(hemp_element_hash_symbol) {
    hemp_element_literal_symbol(hemp, symbol);
/*
    TODO
*/
    return symbol;
}
