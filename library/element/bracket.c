#include <hemp/element.h>
#include <hemp/params.h>

// TODO: move these
void 
hemp_element_params_compile(
    hemp_element_p  element,
    hemp_scope_p    scope
);

void
hemp_element_parens_clean(
    hemp_element_p element
);

void
hemp_element_params_clean(
    hemp_element_p element
);


/*--------------------------------------------------------------------------
 * generic functions for all bracketed constructs
 *--------------------------------------------------------------------------*/

HEMP_SYMBOL_FUNC(hemp_element_brackets_symbol) {
    hemp_element_block_symbol(hemp, symbol);
    symbol->token   = &hemp_element_literal_token;
    symbol->prefix  = &hemp_element_brackets_prefix;
    /* each subtype must define its own value() method */
    symbol->values  = &hemp_element_value_values;
    symbol->text    = &hemp_element_value_text;
    symbol->number  = &hemp_element_value_number;
    symbol->integer = &hemp_element_value_integer;
    symbol->boolean = &hemp_element_value_boolean;
    symbol->compare = &hemp_element_value_compare;
    symbol->cleanup = &hemp_element_brackets_clean;
    return symbol;
}


HEMP_PREFIX_FUNC(hemp_element_brackets_parse) {
    hemp_debug("hemp_element_brackets_parse()\n");
    hemp_element_p  self = *elemptr;
    hemp_symbol_p   type = self->type;
    hemp_list_p     exprs;

    /* skip opening bracket */
    hemp_go_next(elemptr);

    /* parse a list of none or more expressions - first number (0) indicates
     * no forwarded precedence level, second number (1) forces an empty list 
     * to be returned if no expressions are found
     */
    exprs = hemp_element_parse_exprs(elemptr, scope, 0, 1);

    /* skip any whitespace then check for closing parenthesis */
    hemp_skip_whitespace(elemptr);

    if (! hemp_element_terminator_matches(*elemptr, type->end))
        hemp_fatal("missing terminator to match %s => %s\n", type->start, type->end);

    /* skip closing bracket */
    hemp_go_next(elemptr);

    /* stash the list of expressions in the element */
    hemp_set_block_exprs(self, exprs);

    return self;
}


HEMP_PREFIX_FUNC(hemp_element_brackets_prefix) {
    hemp_debug("hemp_element_brackets_prefix()\n");

    /* parse the bracketed expressions */
    hemp_element_p self = hemp_element_brackets_parse(
        elemptr, scope, precedence, force
    );

    /* parse onwards in case there's a postfix operator following */
    /* TODO: should we mute the force flag? */
    return hemp_parse_postfix(
        elemptr, scope, precedence, force, self
    );
}


void
hemp_element_brackets_clean(
    hemp_element_p element
) {
    hemp_debug("hemp_element_brackets_clean(%p)\n", element);
    hemp_list_free(
        hemp_block_exprs(element)
    );
}


/*--------------------------------------------------------------------------
 * parenthesis: ( )
 *--------------------------------------------------------------------------*/

HEMP_SYMBOL_FUNC(hemp_element_parens_symbol) {
    hemp_element_brackets_symbol(hemp, symbol);
    symbol->postfix = &hemp_element_parens_postfix;
    symbol->value   = &hemp_element_parens_value;
    symbol->values  = &hemp_element_block_values;
    symbol->parse_params = &hemp_element_parens_postfix;
    return symbol;
}


HEMP_POSTFIX_FUNC(hemp_element_parens_postfix) {
    hemp_debug("hemp_element_parens_postfix()\n");
    
    hemp_element_p  self = *elemptr;
    hemp_symbol_p   type = self->type;

    HEMP_INFIX_LEFT_PRECEDENCE;

    /* parse the bracketed expressions */
    self = hemp_element_brackets_parse(
        elemptr, scope, precedence, force
    );

    hemp_set_flag(self, HEMP_BE_INFIX);

    hemp_debug("*** creating params ***\n");

    hemp_element_p params = hemp_element_create(
        self, "hemp.params"
    );

    hemp_set_lhs_element(params, lhs);
    hemp_set_rhs_element(params, self);
    hemp_debug("*** created params %p : %s ***\n", params, params->type->name);

    // TODO: more postfix

    return params;
}


HEMP_VALUE_FUNC(hemp_element_parens_value) {
    hemp_debug_call("hemp_element_parens_value()\n");
    hemp_value_t    values  = hemp_obcall(value, values, context, HempNothing);
    hemp_list_p     list    = hemp_val_list(values);

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



/*--------------------------------------------------------------------------
 * parameters, a special case of parenthesis following a function name or 
 * expression yielding a function, e.g. foo(), foo(10, 20)
 * NOTE: was a temp hack... not in use at the moment.
 *--------------------------------------------------------------------------*/

HEMP_SYMBOL_FUNC(hemp_element_params_symbol) {
    hemp_element_parens_symbol(hemp, symbol);
    symbol->value   = &hemp_element_params_value;
//    symbol->values  = &hemp_element_params_values;
    symbol->values  = &hemp_element_value_values;
    symbol->cleanup = &hemp_element_params_clean;
    return symbol;
}

HEMP_VALUE_FUNC(hemp_element_params_value) {
    hemp_debug("hemp_element_params_value()\n");
    
    hemp_element_p element  = hemp_val_elem(value);
//    hemp_value_t   lhs      = hemp_lhs_element(element);
    hemp_value_t   parens   = hemp_rhs(element);
    hemp_value_t   values   = hemp_obcall(parens, values, context, HempNothing);
    hemp_list_p    list     = hemp_val_list(values);

    hemp_debug("got %d parameters in parens\n", list->length);

}


//HEMP_OUTPUT_FUNC(hemp_element_params_values) {
//    hemp_debug("hemp_element_params_values()\n");
//    hemp_element_p element  = hemp_val_elem(value);
//    hemp_value_t   parens   = hemp_rhs(element);
//    return hemp_obcall(parens, values, context, output);
//}



void 
hemp_element_params_compile(
    hemp_element_p  element,
    hemp_scope_p    scope
) {
    hemp_debug("hemp_element_parens_params()\n");
    hemp_list_p     exprs  = hemp_val_list( hemp_expr(element) );
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




void
hemp_element_params_clean(
    hemp_element_p element
) {
    hemp_debug("*** hemp_element_params_clean(%p)\n", element);

// YIKES!  We've already added the args to the block which will free it...
//        hemp_params_p params = (hemp_params_p) hemp_val_ptr(
//            hemp_lhs(element)
//        );
//        if (params)
//            hemp_params_free(params);

//    hemp_list_p exprs = hemp_val_list( hemp_expr(element) );
//    hemp_list_free(exprs);
//    hemp_debug_call("END hemp_element_params_clean(%p)\n", element);
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
