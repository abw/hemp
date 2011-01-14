#include <math.h>
#include <hemp/element.h>


/*--------------------------------------------------------------------------
 * assignment operators
 *--------------------------------------------------------------------------*/

HEMP_ELEMENT(hemp_element_assign) {
    hemp_element_infix_right(hemp, element);
    element->parse_postfix   = &hemp_element_assign_postfix;
    element->text            = &hemp_element_assign_text;
    element->value           = &hemp_element_assign_value;
    element->params          = &hemp_element_assign_params;
    element->pairs           = &hemp_element_assign_pairs;
    hemp_set_flag(element, HEMP_BE_PAIRS);
    return element;
}


HEMP_POSTFIX(hemp_element_assign_postfix) {
    HempFragment self    = *fragptr;
    HempElement  type    = self->type;
    HempFixup    handler = lhs->type->parse_lvalue;
    HempFragment rhs;

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

    hemp_set_lhs_fragment(self, lhs);
    hemp_advance(fragptr);
    rhs = hemp_parse_prefix(fragptr, scope, type->lprec, 1);

    if (! rhs)
        hemp_fatal("missing expression on rhs of %s\n", type->start);
    
    hemp_set_rhs_fragment(self, rhs);

    /* call the lvalue handler to do any jiggling */
    if (handler)
        self = handler(lhs, scope, hemp_frag_val(self));
        
//  hemp_skip_whitespace(elemptr);

    return hemp_parse_postfix(
        fragptr, scope, precedence, 0,
        self
    );
}


HEMP_OUTPUT(hemp_element_assign_text) {
    hemp_debug_call("hemp_element_assign_text()\n");
    hemp_element_assign_value(value, context);
    HempText text;
    hemp_prepare_text(context, output, text);
    return output;
}


HEMP_VALUE(hemp_element_assign_value) {
    hemp_debug_call("hemp_element_assign_value()\n");
    HempFragment fragment = hemp_val_frag(value);
    HempValue    lhs      = hemp_lhs(fragment);
    HempValue    rhs      = hemp_rhs(fragment);
    return hemp_obcall(lhs, assign, context, rhs);
}


HEMP_OUTPUT(hemp_element_assign_params) {
    hemp_debug_call("hemp_element_assign_params()\n");
    HempParams params;
    hemp_prepare_params(context, output, params);
    hemp_element_assign_value(value, context);
    return output;
}


HEMP_OUTPUT(hemp_element_assign_pairs) {
    hemp_debug_call("hemp_element_assign_pairs()\n");
    HempFragment   fragment = hemp_val_frag(value);
    HempValue      lhs      = hemp_lhs(fragment);
    HempValue      rhs      = hemp_rhs(fragment);
    HempValue      key      = hemp_call(lhs, value, context);
    HempValue      rvalue   = hemp_call(rhs, value, context);
    HempHash       pairs;
    hemp_prepare_pairs(context, output, pairs);

    if (! hemp_is_string(key)) {
        hemp_fatal("Non-string key for assignment pairs: %s\n", hemp_type_name(key));
    }
    
//    hemp_debug_msg(
//        "assign pairs %s => %s\n", 
//        hemp_val_str(key), 
//        hemp_type_name(rvalue)
//    );

    hemp_hash_store(pairs, hemp_val_str(key), rvalue);

    return output;
}

