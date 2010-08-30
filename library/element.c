#include <hemp/element.h>


/*--------------------------------------------------------------------------
 * initialisation and cleanup functions
 *--------------------------------------------------------------------------*/

hemp_element_p
hemp_element_new() {
    hemp_element_p element = (hemp_element_p) hemp_mem_alloc(
        sizeof(struct hemp_element_s)
    );

    if (! element)
        hemp_mem_fail("element");

    return element;
}


hemp_element_p
hemp_element_init(
    hemp_element_p  element,
    hemp_symbol_p   type, 
    hemp_str_p      token, 
    hemp_pos_t      position, 
    hemp_size_t     length
) {
    if (! element)
        element = hemp_element_new();
        
    element->type     = type;
    element->token    = token;
    element->position = position;
    element->length   = length;
    element->flags    = type->flags;
    element->next     = NULL;
//  element->value    = NULL;

    return element;
}


void
hemp_element_free(
    hemp_element_p element
) {
    hemp_mem_free(element);
}


/*--------------------------------------------------------------------------
 * front-end parsing pseudo-method
 *--------------------------------------------------------------------------*/

hemp_element_p
hemp_element_parse(
    hemp_element_p element
) {
    hemp_debug_call("hemp_element_parse()\n");

    hemp_element_p block = hemp_element_parse_block(
        &element,
        NULL,
        0, 
        HEMP_FALSE
    );
/*    
    if (block) {
        hemp_text_t text = block->type->text(block, NULL);
        hemp_debug("OUTPUT: %s\n", text->string);
        hemp_text_free(text);
    }
    else {
        hemp_debug_red("did not parse a block\n");
    }
*/
    
    return block;
}


/*--------------------------------------------------------------------------
 * generic element parsing functions
 *--------------------------------------------------------------------------*/

HEMP_PREFIX_FUNC(hemp_element_parse_block) {
    hemp_element_p element = *elemptr;
    hemp_list_p list       = hemp_element_parse_exprs(HEMP_PREFIX_ARG_NAMES);
    hemp_element_p block   = NULL;

    if (list) {
        // hemp_debug("got list of %d exprs\n", list->length);
        block = hemp_element_init(
            NULL,
            HempSymbolBlock, 
            element->token,  
            element->position,
            element->length
        );
        block->args.block.exprs = list;
    }

    return block;
}


hemp_list_p
hemp_element_parse_exprs(
    HEMP_PREFIX_ARGS
) {
    hemp_debug_call("hemp_element_parse_exprs()\n");

    hemp_element_p expr;
    hemp_list_p    exprs = hemp_list_init();

#if HEMP_DEBUG & HEMP_DEBUG_PARSE
    hemp_debug("\n-- EXPRS --\n");
#endif

    while (1) {
        // TODO: skip whitespace/delimiter all in one
        hemp_skip_whitespace(elemptr);
        hemp_skip_terminator(elemptr);
        hemp_skip_whitespace(elemptr);

//      hemp_debug_parse("about to parse expr:\n");

        /* tmp hack to catch TODO stuff while developing */
        if (! (*elemptr)->type->prefix) {
            hemp_symbol_dump((*elemptr)->type);
            hemp_fatal(
                "%s does not define a parse_expr() method",
                (*elemptr)->type->name
            );
        }

        expr = hemp_parse_prefix(elemptr, scope, precedence, HEMP_FALSE);
        
        if (! expr)
            break;

//      hemp_debug_parse("parsed %s expression:\n", expr->type->name);
#if HEMP_DEBUG & HEMP_DEBUG_PARSE
        hemp_element_dump(expr);
#endif
        hemp_list_push(exprs, hemp_ptr_val(expr));
    }

    /* element should be EOF or we hit a duff token */
    if (hemp_at_eof(elemptr)) {
        hemp_debug_parse("%sReached EOF\n%s\n", HEMP_ANSI_GREEN, HEMP_ANSI_RESET);
    }
    else {
        hemp_debug_parse("%sNot an expression: %s:%s\n", HEMP_ANSI_RED, (*elemptr)->type->name, HEMP_ANSI_RESET);
        hemp_element_dump(*elemptr);
    }
    
    // hemp_debug("n expressions: %d\n", exprs->length);
    
    if (! exprs->length && ! force) {
        // hemp_debug("clearing empty list\n");
        hemp_list_free(exprs);
        exprs = NULL;
    }
    
    return exprs;
}


/*--------------------------------------------------------------------------
 * decline functions
 *--------------------------------------------------------------------------*/

HEMP_PREFIX_FUNC(hemp_element_not_prefix) {
    return NULL;
}


HEMP_POSTFIX_FUNC(hemp_element_not_postfix) {
    hemp_debug_call("hemp_element_not_postfix()\n");
    return lhs;
}


HEMP_ETEXT_FUNC(hemp_element_not_token) {
    hemp_fatal("%s element does not yield token\n", element->type->name);
    return output;
}


HEMP_ETEXT_FUNC(hemp_element_not_source) {
    hemp_fatal("%s element does not yield source\n", element->type->name);
    return output;
}


HEMP_ETEXT_FUNC(hemp_element_not_text) {
    hemp_fatal("%s element does not yield text\n", element->type->name);
    return output;
}


HEMP_EVAL_FUNC(hemp_element_not_value) {
    hemp_fatal("%s element does not yield value\n", element->type->name);
    return HempNothing;
}


HEMP_EVAL_FUNC(hemp_element_not_number) {
    hemp_fatal("%s element does not yield number\n", element->type->name);
    return HempNothing;
}


HEMP_EVAL_FUNC(hemp_element_not_integer) {
    hemp_fatal("%s element does not yield integer\n", element->type->name);
    return HempNothing;
}


HEMP_EVAL_FUNC(hemp_element_not_boolean) {
    hemp_fatal("%s element does not yield boolean\n", element->type->name);
    return HempNothing;
}


HEMP_EVAL_FUNC(hemp_element_not_compare) {
    hemp_fatal("%s element does not yield comparison\n", element->type->name);
    return HempNothing;
}


/*--------------------------------------------------------------------------
 * delegation functions, forwarding the request to the next element
 *--------------------------------------------------------------------------*/

HEMP_PREFIX_FUNC(hemp_element_next_prefix) {
    hemp_debug_call("hemp_element_next_prefix()\n");

    if (hemp_has_next(elemptr)) {
        hemp_go_next(elemptr);
        return hemp_parse_prefix(elemptr, scope, precedence, force);
//      return (*elemptr)->type->prefix(HEMP_PREFIX_ARG_NAMES);
    }

    return NULL;
}


HEMP_POSTFIX_FUNC(hemp_element_next_postfix) {
    hemp_debug_call("hemp_element_next_postfix()\n");

    if (hemp_has_next(elemptr)) {
        hemp_go_next(elemptr);
        return hemp_parse_postfix(elemptr, scope, precedence, force, lhs);
//      return (*elemptr)->type->infix(HEMP_POSTFIX_ARG_NAMES);
    }

    return NULL;
}


/*--------------------------------------------------------------------------
 * expression parsing methods
 *--------------------------------------------------------------------------*/

HEMP_PREFIX_FUNC(hemp_element_parse_prefix) {
    hemp_element_p self = *elemptr;
    hemp_symbol_p  type = self->type;

    hemp_debug_call("hemp_element_parse_prefix()\n");

    hemp_set_flag(self, HEMP_BE_PREFIX);
    hemp_go_next(elemptr);

    self->args.unary.expr = hemp_parse_prefix(elemptr, scope, type->rprec, 1);

    if (! self->args.unary.expr)
        hemp_fatal("missing expression on rhs of %s\n", type->start);
    
    hemp_skip_whitespace(elemptr);

    return hemp_parse_postfix(
        elemptr, scope, precedence, 0,
        self
    );
}


HEMP_POSTFIX_FUNC(hemp_element_parse_postfix) {
    hemp_element_p self = *elemptr;
    hemp_symbol_p  type = self->type;

    hemp_debug_call("hemp_element_parse_postfix()\n");

    HEMP_INFIX_LEFT_PRECEDENCE;              // is that right?
    hemp_set_flag(self, HEMP_BE_POSTFIX);

    self->args.unary.expr = lhs;
    hemp_go_next(elemptr);
    hemp_skip_whitespace(elemptr);

    return hemp_parse_postfix(
        elemptr, scope, precedence, 0,
        self
    );
}


HEMP_POSTFIX_FUNC(hemp_element_parse_infix_left) {
    hemp_element_p self = *elemptr;
    hemp_symbol_p  type = self->type;

    hemp_debug_call("hemp_element_parse_infix_left()\n");

    HEMP_INFIX_LEFT_PRECEDENCE;
    hemp_set_flag(self, HEMP_BE_INFIX);

    self->args.binary.lhs = lhs;
    hemp_go_next(elemptr);
    self->args.binary.rhs = hemp_parse_prefix(elemptr, scope, type->lprec, 1);

    if (! self->args.binary.rhs)
        hemp_fatal("missing expression on rhs of %s\n", type->start);
    
    hemp_skip_whitespace(elemptr);

    return hemp_parse_postfix(
        elemptr, scope, precedence, 0,
        self
    );
}


HEMP_POSTFIX_FUNC(hemp_element_parse_infix_right) {
    hemp_element_p self = *elemptr;
    hemp_symbol_p  type = self->type;

    hemp_debug_call("hemp_element_parse_infix_right()\n");

    HEMP_INFIX_RIGHT_PRECEDENCE;
    hemp_set_flag(self, HEMP_BE_INFIX);

    self->args.binary.lhs = lhs;
    hemp_go_next(elemptr);
    self->args.binary.rhs = hemp_parse_prefix(elemptr, scope, type->lprec, 1);

    if (! self->args.binary.rhs)
        hemp_fatal("missing expression on rhs of %s\n", type->start);
        
    hemp_skip_whitespace(elemptr);

    return hemp_parse_postfix(
        elemptr, scope, precedence, 0,
        self
    );
}


/*--------------------------------------------------------------------------
 * evaluation and output functions
 *
 * Elements can provide custom functions for evaluation as a text fragment,
 * number, integer, boolean truth, or tri-state comparison (before, equal,
 * after) value.  In the usual case they "inherit" the default "methods" 
 * below that call the element's generic value() method and then coerce the 
 * returned value to the appropriate type.  That way an element is only 
 * required to implement a single value() method and everything else Just
 * Works[tm].
 *
 * NOTE: don't be tempted to write something like this:
 *   return hemp_to_XXX( element->type->value(element, context) );
 *
 * hemp_to_XXX are macros and the above will cause the element to be 
 * evaluated multiple times.
 *--------------------------------------------------------------------------*/

HEMP_ETEXT_FUNC(hemp_element_value_text) {
    hemp_debug_call("hemp_element_value_text()\n");
    hemp_value_t value = element->type->value(element, context);
    return hemp_onto_text(value, context, output);
}


HEMP_EVAL_FUNC(hemp_element_value_number) {
    hemp_debug_call("hemp_element_value_number()\n");
    hemp_value_t value = element->type->value(element, context);
    return hemp_is_numeric(value)
        ? value
        : hemp_call(value, context, number);

// forced coersion is not the way forward
//    return hemp_to_num(v);
}


HEMP_EVAL_FUNC(hemp_element_value_integer) {
    hemp_debug_call("hemp_element_value_integer()\n");
    hemp_value_t value = element->type->value(element, context);
    return hemp_to_integer(value, context);
}


HEMP_EVAL_FUNC(hemp_element_value_boolean) {
    hemp_debug_call("hemp_element_value_boolean()\n");
    hemp_value_t value = element->type->value(element, context);
    return hemp_to_boolean(value, context);
}


HEMP_EVAL_FUNC(hemp_element_value_compare) {
    hemp_debug_call("hemp_element_value_compare()\n");
    hemp_value_t value = element->type->value(element, context);
    return hemp_to_compare(value, context);

}


/*--------------------------------------------------------------------------
 * debugging functions
 *--------------------------------------------------------------------------*/

hemp_bool_t
hemp_element_dump(
    hemp_element_p e
) {
    if (! e->type->text)
        hemp_fatal("%s type does not define a text() method", e->type->name);

//    hemp_debug("calling text() method for %s\n", e->type->name);

    hemp_value_t output = e->type->token
        ? e->type->token(e, NULL, HempNothing)
        : e->type->text(e, NULL, HempNothing);

    hemp_text_p text  = hemp_val_text(output);
    hemp_str_p string = text ? text->string : "-- NO OUTPUT --";
    
    hemp_debug(
        "%p %03d:%02d %-20s %s[%s%s%s]%s\n", e,
        (int) e->position, (int) e->length, e->type->name, 
        HEMP_ANSI_BLUE, HEMP_ANSI_YELLOW, string, HEMP_ANSI_BLUE, HEMP_ANSI_RESET
    );

    if (text) {
        hemp_text_free(text);
    }
    
    return hemp_string_eq(e->type->name, "EOF")
        ? HEMP_FALSE
        : HEMP_TRUE;
}



