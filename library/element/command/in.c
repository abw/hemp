#include <hemp/element.h>


HEMP_ELEMENT(hemp_element_in);
HEMP_POSTFIX(hemp_element_in_postfix);
HEMP_CLEANUP(hemp_element_in_cleanup);
HEMP_VALUE(hemp_method_element_in_each);


/*--------------------------------------------------------------------------
 * <name> in <value>
 *
 * When used in conjunction with the 'each' command, the 'in' element 
 * behaves rather like an anonymous subroutine constructor.
 *
 *   each a in b { c }
 *
 * This example iterates through the items in 'b', invoking the content of the
 * body block, 'c', for each.  The iterated value is aliased to 'a'.
 *
 * This is equivalent to:
 *
 *   b.each( sub(a) { c } )
 *
 * At compile time we parse the LHS of the 'in' operator ('a' in the above
 * example) to create a subroutine (code) shell with the appropriate prototype
 * defined for the argument, e.g. "sub(a) { ... }".  The content of the code
 * block (denoted here by "...") is initially undefined.
 *
 * When the document is rendered, the 'each' command invokes the 'in' element 
 * via a call to the hemp_method_element_in_each() function, install in the
 * elements type vtable as the each() method.  The body of the each block 
 * ("c") is passed as an argument.  That looks something like this in symbolic 
 * pseudo-code: 
 *
 *   in(a,b)->each(\c)
 *
 * Here we mean "\in(a,b)" to indicate the element tree that represents the 
 * "a in b" construction, and "\c" to denote a reference to the content of
 * the "each" body block.
 *
 * The each() method attaches the content of the body block ("c") to the 
 * subroutine shell created at parse time.  This gives us a construction
 * equivalent to:
 *
 *   sub(a) { c }
 *
 * It then calls the each() method on the RHS data source passing this 
 * subroutine as the argument
 *
 *   b->each( sub(a) { c } ).
 *
 * The iterator for the RHS will then invoke the apply() method against the 
 * subroutine for each item in the data source (e.g. each item in a list, 
 * slot in a hash, record in a database, or whatever, etc).
 *
 * I'm also considering providing a custom boolean() method so that 'in'
 * can be used for existential tests.
 *
 *   if a in b { c }
 *
 *--------------------------------------------------------------------------*/

HEMP_ELEMENT(hemp_element_in) {
    hemp_element_infix_right(hemp, element);
    element->parse_postfix  = &hemp_element_in_postfix;
    element->cleanup        = &hemp_element_in_cleanup;

    /* define a custom each() method */
    hemp_type_extend(
        (HempType) element, 
        "each", &hemp_method_element_in_each
    );

    return element;
}


HEMP_POSTFIX(hemp_element_in_postfix) {
    hemp_debug_call("hemp_element_in_postfix()\n");

    HEMP_INFIX_RIGHT_PRECEDENCE;

    HempFragment fragment   = *fragptr;
    HempCode     code       = hemp_code_new();
    HempProto    proto      = hemp_code_proto(code);

    hemp_set_flag(fragment, HEMP_BE_INFIX);
    hemp_advance(fragptr);

    /* parse the LHS as a prototype lvalue */
    lhs->type->parse_proto(lhs, scope, hemp_ptr_val(proto));
    hemp_set_lhs(fragment, hemp_code_val(code));

    /* parse the RHS expr */
    hemp_parse_rhs_expr_lprec(fragment);

    return hemp_parse_postfix(
        fragptr, scope, precedence, 0,
        fragment
    );
}


HEMP_CLEANUP(hemp_element_in_cleanup) {
    hemp_debug_call("hemp_element_in_clean(%p)\n", fragment);
    HempValue    lhs    = hemp_lhs(fragment);
    HempCode     code   = hemp_val_code(lhs);
    hemp_code_free(code);
}


/*--------------------------------------------------------------------------
 * Method(s)
 *--------------------------------------------------------------------------*/

HEMP_VALUE(hemp_method_element_in_each) {
    hemp_debug_call("hemp_method_element_in_each()\n");

    HempFragment fragment   = hemp_val_frag(value);
    HempValue    lhs        = hemp_lhs(fragment);
    HempValue    rhs        = hemp_rhs(fragment);
    HempCode     code       = hemp_val_code(lhs);
    HempParams   params     = context->frame->params;
    HempValue    saved      = code->body;
    HempFrame    frame;
    HempValue    result;

/*
    hemp_debug_msg(
        "[%s] IN [%s]\n", 
        hemp_type_name(lhs), 
        hemp_type_name(rhs)
    );
*/

    /* attach the body passed as an ordinal parameter to the code block */
    code->body = hemp_list_item(params->ordinals, 0);

    /* create a new call frame and push the code value onto the params list */
    frame = hemp_context_enter(context, NULL);
    hemp_params_push(frame->params, lhs);

    /* call the rhs->each() method */
    result = hemp_send(rhs, "each", context);

    /* release with the call frame */
    hemp_context_leave(context);

    /* restore the original code body to preserve re-entrancy */
    code->body = saved;

    return result;
}

