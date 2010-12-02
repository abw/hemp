#include <math.h>
#include <hemp/element.h>


/*--------------------------------------------------------------------------
 * assignment operators
 *--------------------------------------------------------------------------*/

HEMP_SYMBOL(hemp_element_assign_symbol) {
    hemp_element_infix_right_symbol(hemp, symbol);
    symbol->parse_postfix   = &hemp_element_assign_postfix;
    symbol->text            = &hemp_element_assign_text;
    symbol->value           = &hemp_element_assign_value;
    symbol->params          = &hemp_element_assign_params;
    symbol->pairs           = &hemp_element_assign_pairs;
    hemp_set_flag(symbol, HEMP_BE_PAIRS);
    return symbol;
}


HEMP_POSTFIX_FUNC(hemp_element_assign_postfix) {
    hemp_element self    = *elemptr;
    hemp_symbol  type    = self->type;
    hemp_fixup   handler = lhs->type->parse_lvalue;
    hemp_element rhs;

    hemp_debug_call("hemp_element_assign_postfix()\n");

    HEMP_INFIX_RIGHT_PRECEDENCE;

    /* only proceed if the LHS has a parse_lvalue() handler or sets the 
     * HEMP_BE_LVALUE flag.
     */
    if (handler || hemp_has_flag(lhs, HEMP_BE_LVALUE)) {
        hemp_debug_parse(
            handler 
                ? "%s element has parse_lvalue() method\n" 
                : "%s element has HEMP_BE_LVALUE flag set\n", 
            lhs->type->name
        );
    }
    else {
        return lhs;
    }

    hemp_set_flag(self, HEMP_BE_INFIX);

    hemp_set_lhs_element(self, lhs);
    hemp_go_next(elemptr);
    rhs = hemp_parse_prefix(elemptr, scope, type->lprec, 1);

    if (! rhs)
        hemp_fatal("missing expression on rhs of %s\n", type->start);
    
    hemp_set_rhs_element(self, rhs);

    /* call the lvalue handler to do any jiggling */
    if (handler)
        self = handler(lhs, scope, hemp_elem_val(self));
        
//  hemp_skip_whitespace(elemptr);

    return hemp_parse_postfix(
        elemptr, scope, precedence, 0,
        self
    );
}


HEMP_OUTPUT_FUNC(hemp_element_assign_text) {
    hemp_debug_call("hemp_element_assign_text()\n");
    hemp_element_assign_value(value, context);
    hemp_text text;
    hemp_prepare_text(context, output, text);
    return output;
}


HEMP_VALUE_FUNC(hemp_element_assign_value) {
    hemp_debug_call("hemp_element_assign_value()\n");
    hemp_element  element = hemp_val_elem(value);
    hemp_value    lhs     = hemp_lhs(element);
    hemp_value    rhs     = hemp_rhs(element);
    return hemp_obcall(lhs, assign, context, rhs);
}


HEMP_OUTPUT_FUNC(hemp_element_assign_params) {
    hemp_debug_call("hemp_element_assign_params()\n");
    hemp_params params;
    hemp_prepare_params(context, output, params);
    hemp_element_assign_value(value, context);
    return output;
}


HEMP_OUTPUT_FUNC(hemp_element_assign_pairs) {
    hemp_debug_msg("hemp_element_assign_pairs()\n");
    hemp_element    element = hemp_val_elem(value);
    hemp_value      lhs     = hemp_lhs(element);
    hemp_value      rhs     = hemp_rhs(element);
    hemp_value      key     = hemp_call(lhs, value, context);
    hemp_value      rvalue  = hemp_call(rhs, value, context);
    hemp_hash       pairs;
    hemp_prepare_pairs(context, output, pairs);

    if (! hemp_is_string(key)) {
        hemp_fatal("Non-string key for assignment pairs: %s\n", hemp_type_name(key));
    }
    
    hemp_debug_msg(
        "assign pairs %s => %s\n", 
        hemp_val_str(key), 
        hemp_type_name(rvalue)
    );

    hemp_hash_store(pairs, hemp_val_str(key), rvalue);

    return output;
}
