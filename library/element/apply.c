#include <hemp/element.h>
#include <hemp/proto.h>
#include <hemp/type/code.h>

HEMP_FIXUP(hemp_element_apply_lvalue);
HEMP_VALUE(hemp_element_apply_value);
HEMP_INPUT(hemp_element_apply_assign);



HEMP_ELEMENT(hemp_element_apply) {
    element->parse_lvalue    = &hemp_element_apply_lvalue;
    element->value           = &hemp_element_apply_value;
    element->assign          = &hemp_element_apply_assign;
    element->text            = &hemp_element_value_text;
    return element;
}


HEMP_FIXUP(hemp_element_apply_lvalue) {
    hemp_debug_call("hemp_element_apply_lvalue()\n");
    hemp_set_flag(fragment, HEMP_BE_LVALUE);

    /* This function application element has the function name (or some other
     * expression) in the LHS slot and the list of parenthesised expressions
     * in the RHS slot. 
     *   e.g. foo(a,b)  
     *    =>  [apply, foo, (a,b)].            # [type, lhs, rhs]
     *
     * The fixative value passed as the extra argument is a pointer to
     * the assignment element which has this element on the LHS and the 
     * rvalue on the RHS.  
     *   e.g. foo(a,b) = a ~ b
     *    =>  [assign, [apply,foo,(a,b)], [concat,a,b]]
     *
     * We need to wrap the RHS up in a code value complete with a 
     * prototype compiled from the parenthesised parameters.  We then
     * poke this new value into the RHS of the assignment.  Then when
     * the assignment happens at runtime and hemp_element_apply_assign()
     * is called, the input value passed to it will be a pre-compiled
     * code value which will automatically validate the arguments against
     * the prototype.
     *   e.g. foo(a,b) = a ~ b  
     *    =>  [assign, [apply,foo,(a,b)], [code,(a,b),[concat,a,b]]
     */
    HempFragment parens = hemp_rhs_fragment(fragment);
    HempFragment assign = hemp_val_frag(fixative);
    HempCode     code   = hemp_code_new();
    HempProto    proto  = hemp_code_proto(code);
    code->body          = hemp_rhs(assign);

    parens->type->parse_proto(parens, scope, hemp_ptr_val(proto));
    hemp_set_rhs(assign, hemp_code_val(code));

    return assign;
}


HEMP_VALUE(hemp_element_apply_value) {
    HempFragment fragment = hemp_val_frag(value);
    hemp_debug_call("hemp_element_apply_value(%s)\n", hemp_type_name(hemp_lhs(fragment)));
    
    if (hemp_has_flag(fragment, HEMP_BE_LVALUE)) {
        // TODO
        hemp_fatal("Attempt to directly apply lvalue function declaration\n");
    }

    HempValue    lhs     = hemp_lhs(fragment);
    HempValue    parens  = hemp_rhs(fragment);
    HempFrame    frame   = hemp_context_enter(context, fragment);
    HempValue    params  = hemp_params_val(frame->params);
    HempValue    result;

    /* Evaluate the parenthesised arguments in the new frame */
    hemp_context_focus_params(context);
    hemp_obcall(parens, params, context, params);
    hemp_context_blur_params(context);

    /* Now call the value() method on the LHS element to yield a value 
     * and then call the apply() method on that value.  If the value isn't
     * a code reference, element tree, or some other value that implements
     * a dedicated apply() method, then the default apply() method will 
     * simply short-circuit and return the original value.
     */
    result = hemp_call(lhs, value, context);
//  hemp_debug_msg("apply result: %s\n", hemp_type_name(result));
    result = hemp_call(result, apply, context);

    hemp_context_leave(context);

//  hemp_debug_msg("apply result after application: %s\n", hemp_type_name(result));
    return result;
}


HEMP_INPUT(hemp_element_apply_assign) {
    hemp_debug_call("hemp_element_apply_assign()\n");
    HempFragment fragment = hemp_val_frag(value);
    HempValue    lhs      = hemp_lhs(fragment);

    /* TODO: there's an iffy cross-dependence with hemp_element_apply_lvalue()
     * that could do with being cleaned up
     */
    if (hemp_not_flag(fragment, HEMP_BE_LVALUE))
        hemp_fatal("hemp_element_apply_assign() called when HEMP_BE_LVALUE is not set\n");

//  hemp_debug_msg("apply assign %s\n", hemp_type_name(input));

    return hemp_call(lhs, assign, context, input);
}



