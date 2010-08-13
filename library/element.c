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
    hemp_cstr_p     token, 
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
    debug_call("hemp_element_parse()\n");

    hemp_element_p block = hemp_element_parse_block(
        &element,
        NULL,
        0, 
        HEMP_FALSE
    );
/*    
    if (block) {
        hemp_text_t text = block->type->text(block, NULL);
        debug("OUTPUT: %s\n", text->string);
        hemp_text_free(text);
    }
    else {
        debug_red("did not parse a block\n");
    }
*/
    
    return block;
}


/*--------------------------------------------------------------------------
 * generic element parsing functions
 *--------------------------------------------------------------------------*/

HEMP_PARSE_FUNC(hemp_element_parse_block) {
    hemp_element_p element = *elemptr;
    hemp_list_p list       = hemp_element_parse_exprs(HEMP_PARSE_ARG_NAMES);
    hemp_element_p block   = NULL;

    if (list) {
        // debug("got list of %d exprs\n", list->length);
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
    HEMP_PARSE_ARGS
) {
    debug_call("hemp_element_parse_exprs()\n");

    hemp_element_p expr;
    hemp_list_p    exprs = hemp_list_init();

#if DEBUG & DEBUG_PARSE
    debug("\n-- EXPRS --\n");
#endif

    while (1) {
        // TODO: skip whitespace/delimiter all in one
        hemp_skip_whitespace(elemptr);
        hemp_skip_terminator(elemptr);
        hemp_skip_whitespace(elemptr);

//      debug_parse("about to parse expr:\n");

        /* tmp hack to catch TODO stuff while developing */
        if (! (*elemptr)->type->expr) {
            hemp_symbol_dump((*elemptr)->type);
            hemp_fatal(
                "%s does not define a parse_expr() method",
                (*elemptr)->type->name
            );
        }

        expr = hemp_parse_expr(elemptr, scope, precedence, HEMP_FALSE);
        
        if (! expr)
            break;

//      debug_parse("parsed %s expression:\n", expr->type->name);
#if DEBUG & DEBUG_PARSE
        hemp_element_dump(expr);
#endif
        hemp_list_push(exprs, expr);
    }

    /* element should be EOF or we hit a duff token */
    if (hemp_at_eof(elemptr)) {
        debug_parse("%sReached EOF\n%s\n", ANSI_GREEN, ANSI_RESET);
    }
    else {
        debug_parse("%sNot an expression: %s:%s\n", ANSI_RED, (*elemptr)->type->name, ANSI_RESET);
        hemp_element_dump(*elemptr);
    }
    
    // debug("n expressions: %d\n", exprs->length);
    
    if (! exprs->length && ! force) {
        // debug("clearing empty list\n");
        hemp_list_free(exprs);
        exprs = NULL;
    }
    
    return exprs;
}


HEMP_PARSE_FUNC(hemp_element_parse_expr) {
    debug_todo("hemp_element_parse_expr()\n");
    return NULL;
}


/*--------------------------------------------------------------------------
 * decline functions
 *--------------------------------------------------------------------------*/

HEMP_PARSE_FUNC(hemp_element_not_expr) {
    return NULL;
}


HEMP_INFIX_FUNC(hemp_element_not_infix) {
    debug_call("hemp_element_not_infix()\n");
    return lhs;
}


HEMP_OUTPUT_FUNC(hemp_element_not_source) {
    hemp_fatal("%s element does not yield source\n", element->type->name);
    return output;
}


HEMP_OUTPUT_FUNC(hemp_element_not_text) {
    hemp_fatal("%s element does not yield text\n", element->type->name);
    return output;
}


HEMP_VALUE_FUNC(hemp_element_not_number) {
    hemp_fatal("%s element does not yield number\n", element->type->name);
    return HempNothing;
}


HEMP_VALUE_FUNC(hemp_element_not_integer) {
    hemp_fatal("%s element does not yield integer\n", element->type->name);
    return HempNothing;
}


HEMP_VALUE_FUNC(hemp_element_not_boolean) {
    hemp_fatal("%s element does not yield boolean\n", element->type->name);
    return HempNothing;
}


HEMP_VALUE_FUNC(hemp_element_not_compare) {
    hemp_fatal("%s element does not yield comparison\n", element->type->name);
    return HempNothing;
}


/*--------------------------------------------------------------------------
 * delegation functions, forwarding the request to the next element
 *--------------------------------------------------------------------------*/

HEMP_PARSE_FUNC(hemp_element_next_expr) {
    debug_call("hemp_element_next_expr()\n");

    if (hemp_has_next(elemptr)) {
        hemp_go_next(elemptr);
        return (*elemptr)->type->expr(HEMP_PARSE_ARG_NAMES);
    }

    return NULL;
}


HEMP_INFIX_FUNC(hemp_element_next_infix) {
    debug_call("hemp_element_next_infix()\n");

    if (hemp_has_next(elemptr)) {
        hemp_go_next(elemptr);
        return (*elemptr)->type->infix(HEMP_INFIX_ARG_NAMES);
    }

    return NULL;
}


/*--------------------------------------------------------------------------
 * expression parsing methods
 *--------------------------------------------------------------------------*/

HEMP_PARSE_FUNC(hemp_element_parse_prefix) {
    hemp_element_p self = *elemptr;
    hemp_symbol_p  type = self->type;

    debug_call("hemp_element_parse_prefix()\n");

    hemp_set_flag(self, HEMP_BE_PREFIX);
    hemp_go_next(elemptr);

    self->args.unary.expr = hemp_parse_expr(elemptr, scope, type->rprec, 1);

    if (! self->args.unary.expr)
        hemp_fatal("missing expression on rhs of %s\n", type->start);
    
    hemp_skip_whitespace(elemptr);

    return hemp_parse_infix(
        elemptr, scope, precedence, 0,
        self
    );
}


HEMP_INFIX_FUNC(hemp_element_parse_postfix) {
    hemp_element_p self = *elemptr;
    hemp_symbol_p  type = self->type;

    debug_call("hemp_element_parse_infix_left()\n");

    HEMP_INFIX_LEFT_PRECEDENCE;
    hemp_set_flag(self, HEMP_BE_POSTFIX);

    self->args.unary.expr = lhs;
    hemp_go_next(elemptr);
    hemp_skip_whitespace(elemptr);

    return hemp_parse_infix(
        elemptr, scope, precedence, 0,
        self
    );
}


HEMP_INFIX_FUNC(hemp_element_parse_infix_left) {
    hemp_element_p self = *elemptr;
    hemp_symbol_p  type = self->type;

    debug_call("hemp_element_parse_infix_left()\n");

    HEMP_INFIX_LEFT_PRECEDENCE;
    hemp_set_flag(self, HEMP_BE_INFIX);

    self->args.binary.lhs = lhs;
    hemp_go_next(elemptr);
    self->args.binary.rhs = hemp_parse_expr(elemptr, scope, type->lprec, 1);

    if (! self->args.binary.rhs)
        hemp_fatal("missing expression on rhs of %s\n", type->start);
    
    hemp_skip_whitespace(elemptr);

    return hemp_parse_infix(
        elemptr, scope, precedence, 0,
        self
    );
}


HEMP_INFIX_FUNC(hemp_element_parse_infix_right) {
    hemp_element_p self = *elemptr;
    hemp_symbol_p  type = self->type;

    debug_call("hemp_element_parse_infix_right()\n");

    HEMP_INFIX_RIGHT_PRECEDENCE;
    hemp_set_flag(self, HEMP_BE_INFIX);

    self->args.binary.lhs = lhs;
    hemp_go_next(elemptr);
    self->args.binary.rhs = hemp_parse_expr(elemptr, scope, type->lprec, 1);

    if (! self->args.binary.rhs)
        hemp_fatal("missing expression on rhs of %s\n", type->start);
        
    hemp_skip_whitespace(elemptr);

    return hemp_parse_infix(
        elemptr, scope, precedence, 0,
        self
    );
}


/*--------------------------------------------------------------------------
 * output functions
 *--------------------------------------------------------------------------*/

HEMP_OUTPUT_FUNC(hemp_element_binary_source) {
    debug_call("hemp_element_binary_source()\n");

    /* ARSE!  I forgot, I'm using the source "method" to display token
     * list as part of the parser debug... will have to disable this for
     * now
     */
    return hemp_element_literal_source(HEMP_OUTPUT_ARG_NAMES);

    hemp_text_p text;


    hemp_prepare_output(output, text, 32);

    struct hemp_binary_s exprs = element->args.binary;
    hemp_element_p lhs = exprs.lhs;
    hemp_element_p rhs = exprs.rhs;

    lhs->type->source(lhs, context, output);
    hemp_text_append_cstr(text, HEMP_STR_SPACE);
    hemp_text_append_cstrn(text, element->token, element->length);
    hemp_text_append_cstr(text, HEMP_STR_SPACE);

    rhs->type->source(rhs, context, output);

    return output;
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

//    debug("calling text() method for %s\n", e->type->name);

    hemp_value_t output = e->type->source
        ? e->type->source(e, NULL, HempNothing)
        : e->type->text(e, NULL, HempNothing);

    hemp_text_p text = hemp_val_text(output);
    hemp_cstr_p cstr = text ? text->string : "-- NO OUTPUT --";
    
    debug(
        "%p %03d:%02d %-20s %s[%s%s%s]%s\n", e,
        (int) e->position, (int) e->length, e->type->name, 
        ANSI_BLUE, ANSI_YELLOW, cstr, ANSI_BLUE, ANSI_RESET
    );

    if (text) {
        hemp_text_free(text);
    }
    
    return hemp_cstr_eq(e->type->name, "EOF")
        ? HEMP_FALSE
        : HEMP_TRUE;
}



