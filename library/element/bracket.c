#include <hemp/element.h>
#include <hemp/params.h>

// TODO: move this
void 
hemp_element_parens_compile_params(
    hemp_element_p  element,
    hemp_scope_p    scope
);

void
hemp_element_parens_clean(
    hemp_element_p element
);


/*--------------------------------------------------------------------------
 * generic functions for all bracketed constructs
 *--------------------------------------------------------------------------*/

HEMP_SYMBOL_FUNC(hemp_element_brackets_symbol) {
    hemp_element_literal_symbol(hemp, symbol);
    symbol->cleanup = &hemp_element_brackets_clean;
    symbol->prefix  = &hemp_element_brackets_prefix;
//    symbol->postfix = &hemp_element_brackets_postfix;
    symbol->text    = &hemp_element_value_text;
    symbol->number  = &hemp_element_value_number;
    symbol->integer = &hemp_element_value_integer;
    symbol->boolean = &hemp_element_value_boolean;
    symbol->compare = &hemp_element_value_compare;
    return symbol;
}



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
    symbol->cleanup = &hemp_element_parens_clean;
    symbol->value   = &hemp_element_parens_value;
    symbol->values  = &hemp_element_parens_values;
    symbol->parse_params = &hemp_element_parens_parse_params;
    return symbol;
}


HEMP_POSTFIX_FUNC(hemp_element_parens_parse_params) {
    hemp_debug("hemp_element_parens_parse_params()\n");
    
    hemp_element_p  self = *elemptr;
    hemp_symbol_p   type = self->type;
    hemp_list_p     exprs;

    /* skip opening parenthesis */
    hemp_go_next(elemptr);

    exprs = hemp_element_parse_exprs(elemptr, scope, 0, 1);

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
    hemp_set_rhs(self, hemp_list_val(exprs));
    hemp_set_flag(self, HEMP_BE_POSTFIX);               // TODO: quick hack

    hemp_element_parens_compile_params(self, scope);

    return self;
}


void 
hemp_element_parens_compile_params(
    hemp_element_p  element,
    hemp_scope_p    scope
) {
    hemp_debug("hemp_element_parens_params()\n");
    hemp_list_p     exprs  = hemp_val_list( hemp_rhs(element) );
    hemp_params_p   params = hemp_params_new();
    hemp_element_p  expr;
    hemp_value_t    item;
    hemp_size_t     n;

    hemp_debug("%d expressions in parens params\n", exprs->length);
    
    for (n = 0; n < exprs->length; n++) {
        item = hemp_list_item(exprs, n);
        expr = hemp_val_elem(item);
        if (expr->type->lvalue_param) {
            hemp_debug("has an lvalue_param() method\n");
            expr->type->lvalue_param(expr, scope, hemp_ptr_val(params));
        }
        else {
            hemp_debug("does not have an lvalue_param() method\n");
            hemp_element_not_lvalue_param(expr, scope, hemp_ptr_val(params));
        }
    }
    hemp_debug("new params at %p\n", params);
    hemp_set_lhs(element, hemp_ptr_val(params));
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



void
hemp_element_parens_clean(
    hemp_element_p element
) {
    hemp_debug_call("hemp_element_parens_clean(%p)\n", element);

    if (hemp_has_flag(element, HEMP_BE_POSTFIX)) {
// YIKES!  We've already added the args to the block which will free it...
//        hemp_params_p params = (hemp_params_p) hemp_val_ptr(
//            hemp_lhs(element)
//        );
//        if (params)
//            hemp_params_free(params);

        hemp_list_p exprs = hemp_val_list( hemp_rhs(element) );
        hemp_list_free(exprs);
    }
    else {
        hemp_debug("cleaning parens block\n");
        hemp_element_block_clean(
            hemp_expr_element(element)
        );
    }
    hemp_debug_call("END hemp_element_parens_clean(%p)\n", element);

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
