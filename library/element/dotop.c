#include <hemp/element.h>
//#include <hemp/scope.h>


HEMP_ELEMENT(hemp_element_dotop) {
    hemp_element_infix_left(hemp, element);
    element->parse_postfix = &hemp_element_dotop_postfix;
    element->value         = &hemp_element_dotop_value;
    element->assign        = &hemp_element_dotop_assign;
    element->cleanup       = &hemp_element_dotop_cleanup;
    hemp_set_flag(element, HEMP_BE_LVALUE);
    return element;
}


HEMP_POSTFIX(hemp_element_dotop_postfix) {
    HempFragment self = *fragptr;
    HempElement  type = self->type;

    hemp_debug_call("hemp_element_dotop_postfix()\n");

    HEMP_INFIX_LEFT_PRECEDENCE;
    
    hemp_set_flag(self, HEMP_BE_INFIX);
    hemp_set_lhs_fragment(self, lhs);
    hemp_advance(fragptr);
    hemp_skip_whitespace(fragptr);

    // FIXME: temporary hack to get something working
    HempFragment rhs = hemp_parse_fixed(fragptr, scope, type->lprec, 1);

    if (! rhs)
        hemp_fatal("missing expression on rhs of %s\n", type->start);

    // make rhs yield static token name
    hemp_set_rhs(
        self,
        /* fuckola!  we need to call token() on words, but value() on numbers */
        /* TODO: make the word be a plain word and only upgrade to variable
         * when used in prefix/infix position.  If used as fixed then it 
         * returns the token text (which is always a text copy...)
         */
        rhs->type->value(
            hemp_frag_val(rhs), scope->context
        )
    );

    hemp_skip_whitespace(fragptr);
    
    return hemp_parse_postfix(
        fragptr, scope, precedence, 0,
        self
    );
}


HEMP_VALUE(hemp_element_dotop_value) {
    hemp_debug_call("hemp_element_dotop_value()\n");

    /* TODO: Note the temporary hack: rhs is always a pre-evaluated value */
    HempFragment fragment = hemp_val_frag(value);
    HempValue    lhs      = hemp_lhs(fragment);
    HempValue    lval     = hemp_obcall(lhs, value, context);
    HempValue    rval     = hemp_rhs(fragment);

    if (hemp_is_undefined(lval)) {
        hemp_fatal("TODO: cannot call dotop on undefined value");
    }
    
    if (hemp_is_undefined(rval)) {
        hemp_fatal("TODO: cannot call dotop with undefined RHS value");
    }

    return hemp_dot(lval, context, rval);
}


HEMP_INPUT(hemp_element_dotop_assign) {
    hemp_debug_call("hemp_element_dotop_assign()\n");

    /* Note the temporary hack: rhs is always a pre-evaluated value */
    HempFragment fragment = hemp_val_frag(value);
    HempValue    lhs      = hemp_lhs(fragment);
    HempValue    lval     = hemp_obcall(lhs, value, context);
    HempValue    rval     = hemp_rhs(fragment);

    return hemp_store(
        lval, context, rval, 
        /* evaluate operand element tree to yield an immediate value */
        hemp_call(input, value, context)
    );
}


HEMP_CLEANUP(hemp_element_dotop_cleanup) {
    hemp_debug_call("hemp_element_dotop_cleanup(%p)\n", fragment);

    if (hemp_has_flag(fragment, HEMP_BE_FIXED)) {
//      hemp_debug("cleaning static dotop element...");
//      TODO: more comprehensive check, probably via type->cleanup

        HempValue rhs = hemp_rhs(fragment);
        if (hemp_is_text(rhs)) {
            // TODO: It appears to be the case that the context is managing
            // the memory allocated by the fixed RHS so we don't need to free
            // it explicitly.  However, I haven't been able to convince 
            // myself where and when this is happening, so it is to be taken
            // with a pinch of salt.
            
            // hemp_debug("rhs is text, freeing\n");
            // hemp_text_free( hemp_val_text(rhs) );
        }
        else {
//          hemp_debug("not freeing %s RHS\n", hemp_type_name(rhs));
        }
    }
}

