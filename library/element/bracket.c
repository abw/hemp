#include <hemp/element.h>
#include <hemp/proto.h>

HEMP_PREFIX_FUNC(hemp_element_brackets_parse);
HEMP_PREFIX_FUNC(hemp_element_brackets_prefix);
HEMP_CLEAN_FUNC(hemp_element_brackets_clean);

HEMP_POSTFIX_FUNC(hemp_element_parens_postfix);
HEMP_FIXUP_FUNC(hemp_element_parens_proto);
HEMP_VALUE_FUNC(hemp_element_parens_value);



/*--------------------------------------------------------------------------
 * generic functions for all bracketed constructs
 *--------------------------------------------------------------------------*/

HEMP_SYMBOL(hemp_element_brackets_symbol) {
    hemp_element_block_symbol(hemp, symbol);
    symbol->token           = &hemp_element_literal_token;
    symbol->parse_prefix    = &hemp_element_brackets_prefix;
    /* each subtype must define its own value() method */
    symbol->values          = &hemp_element_value_values;
    symbol->text            = &hemp_element_value_text;
    symbol->number          = &hemp_element_value_number;
    symbol->integer         = &hemp_element_value_integer;
    symbol->boolean         = &hemp_element_value_boolean;
    symbol->compare         = &hemp_element_value_compare;
    symbol->cleanup         = &hemp_element_brackets_clean;
    return symbol;
}


HEMP_PREFIX_FUNC(hemp_element_brackets_parse) {
    hemp_debug_call("hemp_element_brackets_parse()\n");
    hemp_element  self = *elemptr;
    hemp_symbol   type = self->type;
    hemp_list     exprs;

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
    hemp_set_block_exprs_list(self, exprs);
    hemp_set_flag(self, HEMP_BE_ALLOCATED);

    return self;
}


HEMP_PREFIX_FUNC(hemp_element_brackets_prefix) {
    hemp_debug_call("hemp_element_brackets_prefix()\n");

    /* parse the bracketed expressions */
    hemp_element self = hemp_element_brackets_parse(
        elemptr, scope, precedence, force
    );

    /* parse onwards in case there's a postfix operator following */
    /* TODO: should we mute the force flag? */
    return hemp_parse_postfix(
        elemptr, scope, precedence, force, self
    );
}


HEMP_CLEAN_FUNC(hemp_element_brackets_clean) {
    hemp_debug_call("hemp_element_brackets_clean(%p)\n", element);

    if (hemp_has_flag(element, HEMP_BE_ALLOCATED)) {
        hemp_list_free(
            hemp_block_exprs_list(element)
        );
    }
}


/*--------------------------------------------------------------------------
 * parenthesis: ( )
 *--------------------------------------------------------------------------*/

HEMP_SYMBOL(hemp_element_parens_symbol) {
    hemp_element_brackets_symbol(hemp, symbol);
    symbol->parse_params    = &hemp_element_brackets_parse;
    symbol->parse_postfix   = &hemp_element_parens_postfix;
    symbol->parse_proto     = &hemp_element_parens_proto;
    symbol->value           = &hemp_element_parens_value;
    symbol->values          = &hemp_element_block_values;
    symbol->params          = &hemp_element_block_params;
    symbol->flags  |= HEMP_BE_POSTBOUND;
    return symbol;
}


HEMP_POSTFIX_FUNC(hemp_element_parens_postfix) {
    hemp_debug_call("hemp_element_parens_postfix()\n");
    
    hemp_element  self = *elemptr;
    hemp_symbol   type = self->type;

    HEMP_INFIX_LEFT_PRECEDENCE;

    /* parse the bracketed expressions */
    self = hemp_element_brackets_parse(
        elemptr, scope, precedence, force
    );

    hemp_set_flag(self, HEMP_BE_INFIX);

    hemp_element apply = hemp_element_create(
        self, "hemp.apply"
    );

    hemp_set_lhs_element(apply, lhs);
    hemp_set_rhs_element(apply, self);

    return hemp_parse_postfix(
        elemptr, scope, precedence, 0,
        apply
    );
}


HEMP_FIXUP_FUNC(hemp_element_parens_proto) {
    hemp_debug_call("hemp_element_parens_proto(%p)\n", element);
    hemp_list     exprs = hemp_block_exprs_list(element);
    hemp_value    item;
    hemp_element  expr;
    hemp_size     n;

    for (n = 0; n < exprs->length; n++) {
        item = hemp_list_item(exprs, n);
        expr = hemp_val_elem(item);
        expr->type->parse_proto(expr, scope, fixative);
    }

    return element;
}


HEMP_VALUE_FUNC(hemp_element_parens_value) {
    hemp_debug_call("hemp_element_parens_value()\n");
    hemp_value    values  = hemp_obcall(value, values, context, HempNothing);
    hemp_list     list    = hemp_val_list(values);

//  hemp_debug_msg("got %d values returned by parens\n", list->length);

    if (list->length > 1) {
//      hemp_debug_msg("squishing list of %d items to text\n", list->length);
        return hemp_vtext(values, context, HempNothing);
    }
    else if (list->length == 1) {
//      hemp_debug_msg("got one value returned by parens\n"); 
        return hemp_list_item(list, 0);
    }
    else {
//      hemp_debug_msg("nothing in list\n");
        return HempEmpty;
    }
}



/*--------------------------------------------------------------------------
 * square brackets: [ ]
 *--------------------------------------------------------------------------*/

HEMP_SYMBOL(hemp_element_list_symbol) {
    hemp_element_brackets_symbol(hemp, symbol);
    symbol->value = &hemp_element_list_value;
    return symbol;
}


HEMP_VALUE_FUNC(hemp_element_list_value) {
    hemp_debug_call("hemp_element_list_value()\n");
    return hemp_element_block_values(
        value, context, HempNothing
    );
}


/*--------------------------------------------------------------------------
 * hash: might want to rename this to 'scope' to better reflect it's dual
 * nature behaviour both as a hash constructor when used in an expression, 
 * e.g. hash = { a => 10 }, and as a code block: if a { b }
 *--------------------------------------------------------------------------*/

HEMP_SYMBOL(hemp_element_hash_symbol) {
    hemp_element_brackets_symbol(hemp, symbol);
    symbol->parse_prefix    = &hemp_element_hash_prefix;
    symbol->parse_body      = &hemp_element_hash_body;
    symbol->value           = &hemp_element_hash_value;
    hemp_set_flag(symbol, HEMP_BE_POSTBOUND);
    return symbol;
}


HEMP_PREFIX_FUNC(hemp_element_hash_prefix) {
    hemp_debug_call("hemp_element_hash_prefix()\n");
    hemp_element element = hemp_element_brackets_parse(HEMP_PREFIX_ARG_NAMES);
    hemp_list    exprs   = hemp_block_exprs_list(element);
    hemp_value   item;
    hemp_element expr;
    hemp_size    n;
    
    for (n = 0; n < exprs->length; n++) {
        item = hemp_list_item(exprs, n);
        expr = hemp_val_elem(item);
        // should we also support a parse_pairs handler in case the 
        // expression wants to make a decision about what it supports
        if (hemp_not_flag(expr, HEMP_BE_PAIRS))
            hemp_fatal(     // TODO: proper parse error
                "Invalid expression inside hash definition: %s\n", 
                expr->type->name
            );
    }

    return element;
}


HEMP_PREFIX_FUNC(hemp_element_hash_body) {
    hemp_debug_call("hemp_element_hash_body()\n");
    hemp_element element = hemp_element_brackets_parse(HEMP_PREFIX_ARG_NAMES);
    hemp_set_flag(element, HEMP_BE_BODY|HEMP_BE_TERMINATED);   // better to retype?
    return element;
}


HEMP_VALUE_FUNC(hemp_element_hash_value) {
    hemp_debug_msg("hemp_element_hash_value()\n");
    hemp_element element = hemp_val_elem(value);

    if (hemp_has_flag(element, HEMP_BE_BODY))
        return hemp_element_block_value(value, context);
    
    hemp_list   exprs = hemp_block_exprs_list(element);
    hemp_hash   hash  = hemp_context_tmp_hash(context);
    hemp_value  hashv = hemp_hash_val(hash);
    hemp_value  item;
    hemp_size   n;

    for (n = 0; n < exprs->length; n++) {
        item = hemp_list_item(exprs, n);
        hemp_call(item, pairs, context, hashv);
    }

    return hashv;

    // pairs/slots = block->type->pairs(block, context, HempNothing);
    // add pairs/slots to a hash, return.
    // or better still, pass hash to pairs()... but what if we want a list
    // of pairs/slots instead?
    // maybe call ->pairs to return a list of pairs and then hashify them
}
