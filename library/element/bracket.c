#include <hemp/element.h>
#include <hemp/proto.h>


/*--------------------------------------------------------------------------
 * generic functions for all bracketed constructs
 *--------------------------------------------------------------------------*/

HEMP_ELEMENT(hemp_element_brackets) {
    hemp_element_block(hemp, element);
    element->token           = &hemp_element_literal_text;
    element->parse_prefix    = &hemp_element_brackets_prefix;
    /* each subtype must define its own value() method */
    element->values          = &hemp_element_value_values;
    element->text            = &hemp_element_value_text;
    element->number          = &hemp_element_value_number;
    element->integer         = &hemp_element_value_integer;
    element->boolean         = &hemp_element_value_boolean;
    element->compare         = &hemp_element_value_compare;
    element->cleanup         = &hemp_element_brackets_clean;
    return element;
}


HEMP_PREFIX(hemp_element_brackets_parse) {
    hemp_debug_call("hemp_element_brackets_parse()\n");
    HempFragment self = *fragptr;
    HempElement  type = self->type;
    HempList     exprs;

    /* skip opening bracket */
    hemp_advance(fragptr);

    /* parse a list of none or more expressions - first number (0) indicates
     * no forwarded precedence level, second number (1) forces an empty list 
     * to be returned if no expressions are found
     */
    exprs = hemp_parse_exprs(fragptr, scope, 0, 1);

    /* skip any whitespace then check for closing parenthesis */
    hemp_skip_whitespace(fragptr);

    if (! hemp_element_terminator_matches(*fragptr, type->end))
        hemp_fatal("missing terminator to match %s => %s\n", type->start, type->end);

    /* skip closing bracket */
    hemp_advance(fragptr);

    /* stash the list of expressions in the element */
    hemp_set_block_exprs_list(self, exprs);
    hemp_set_flag(self, HEMP_BE_ALLOCATED);

    return self;
}


HEMP_PREFIX(hemp_element_brackets_prefix) {
    hemp_debug_call("hemp_element_brackets_prefix()\n");

    /* parse the bracketed expressions */
    HempFragment self = hemp_element_brackets_parse(
        fragptr, scope, precedence, force
    );

    /* parse onwards in case there's a postfix operator following */
    /* TODO: should we mute the force flag? */
    return hemp_parse_postfix(
        fragptr, scope, precedence, force, self
    );
}


HEMP_CLEANUP(hemp_element_brackets_clean) {
    hemp_debug_call("hemp_element_brackets_clean(%p)\n", fragment);

    if (hemp_has_flag(fragment, HEMP_BE_ALLOCATED)) {
        hemp_list_free(
            hemp_block_exprs_list(fragment)
        );
    }
}



/*--------------------------------------------------------------------------
 * parenthesis: ( )
 *--------------------------------------------------------------------------*/

HEMP_ELEMENT(hemp_element_parens) {
    hemp_element_brackets(hemp, element);
    element->parse_params    = &hemp_element_brackets_parse;
    element->parse_pair      = &hemp_element_brackets_parse;
    element->parse_postfix   = &hemp_element_parens_postfix;
    element->parse_proto     = &hemp_element_parens_proto;
    element->value           = &hemp_element_parens_value;
    element->values          = &hemp_element_block_values;
    element->params          = &hemp_element_block_params;
    hemp_set_flag(element, HEMP_BE_POSTBOUND);
    return element;
}


HEMP_POSTFIX(hemp_element_parens_postfix) {
    hemp_debug_call("hemp_element_parens_postfix()\n");
    
    HempFragment fragment = *fragptr;

    HEMP_INFIX_LEFT_PRECEDENCE;

    /* parse the bracketed expressions */
    fragment = hemp_element_brackets_parse(
        fragptr, scope, precedence, force
    );

    hemp_set_flag(fragment, HEMP_BE_INFIX);

    HempFragment apply = hemp_fragment_new_fragment(
        fragment, "hemp.apply"
    );

    hemp_set_lhs_fragment(apply, lhs);
    hemp_set_rhs_fragment(apply, fragment);

    return hemp_parse_postfix(
        fragptr, scope, precedence, 0,
        apply
    );
}


HEMP_FIXUP(hemp_element_parens_proto) {
    hemp_debug_call("hemp_element_parens_proto(%p)\n", fragment);
    HempList     exprs = hemp_block_exprs_list(fragment);
    HempFragment expr;
    HempValue    item;
    HempSize     n;

    for (n = 0; n < exprs->length; n++) {
        item = hemp_list_item(exprs, n);
        expr = hemp_val_frag(item);
        expr->type->parse_proto(expr, scope, fixative);
    }

    return fragment;
}


HEMP_VALUE(hemp_element_parens_value) {
    hemp_debug_call("hemp_element_parens_value()\n");
    HempValue values  = hemp_obcall(value, values, context, HempNothing);
    HempList  list    = hemp_val_list(values);

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

HEMP_ELEMENT(hemp_element_list) {
    hemp_element_brackets(hemp, element);
    element->value = &hemp_element_list_value;
    return element;
}


HEMP_VALUE(hemp_element_list_value) {
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

HEMP_ELEMENT(hemp_element_hash) {
    hemp_element_brackets(hemp, element);
    element->parse_prefix    = &hemp_element_hash_prefix;
    element->parse_body      = &hemp_element_hash_body;
    element->value           = &hemp_element_hash_value;
    hemp_set_flag(element, HEMP_BE_POSTBOUND);
    return element;
}


HEMP_PREFIX(hemp_element_hash_prefix) {
    hemp_debug_msg("hemp_element_hash_prefix()  precedence: %d  force: %s\n", precedence, force ? "true" : "false");

//  HEMP_PREFIX_PRECEDENCE;

    HempFragment fragment = hemp_element_brackets_parse(HEMP_PREFIX_ARG_NAMES);
    HempList     exprs    = hemp_block_exprs_list(fragment);
    HempFragment expr;
    HempValue    item;
    HempSize     n;
    
    /* TODO: this should be merged in with newer code in hemp_parse_pairs() */
    for (n = 0; n < exprs->length; n++) {
        item = hemp_list_item(exprs, n);
        expr = hemp_val_frag(item);
        // should we also support a parse_pairs handler in case the 
        // expression wants to make a decision about what it supports
        if (hemp_not_flag(expr, HEMP_BE_PAIRS))
            hemp_fatal(     // TODO: proper parse error
                "Invalid expression inside hash definition: %s\n", 
                expr->type->name
            );
    }

    return fragment;
}


HEMP_PREFIX(hemp_element_hash_body) {
    hemp_debug_call("hemp_element_hash_body()\n");
    HempFragment fragment = hemp_element_brackets_parse(HEMP_PREFIX_ARG_NAMES);
    hemp_set_flag(fragment, HEMP_BE_BODY|HEMP_BE_TERMINATED);
    return fragment;
}


HEMP_VALUE(hemp_element_hash_value) {
    hemp_debug_call("hemp_element_hash_value()\n");
    HempFragment fragment = hemp_val_frag(value);

    if (hemp_has_flag(fragment, HEMP_BE_BODY))
        return hemp_element_block_value(value, context);
    
    HempList   exprs = hemp_block_exprs_list(fragment);
    HempHash   hash  = hemp_context_tmp_hash(context);
    HempValue  hashv = hemp_hash_val(hash);
    HempValue  item;
    HempSize   n;

    for (n = 0; n < exprs->length; n++) {
        item = hemp_list_item(exprs, n);
        hemp_call(item, pairs, context, hashv);
    }

    return hashv;
}
