#include <hemp/element.h>


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
 * Simple functions for skipping over tokens.  I know, it looks tempting to 
 * define these as macros, but they have to be real functions so we can store
 * pointers to them in the element vtables.
 *--------------------------------------------------------------------------*/

hemp_element_p
hemp_element_dont_skip(
    HEMP_SKIP_PROTO
) {
    /* no-op to return the current element */
    return *elemptr;
}


hemp_element_p
hemp_element_next_skip_space(
    HEMP_SKIP_PROTO
) {
    // TODO: need to consider BRANCH for tag start, strings, etc.
    if (hemp_has_next(elemptr)) {
        hemp_go_next(elemptr);
        return hemp_skip_space(elemptr);
    }
    return NULL;
}


hemp_element_p
hemp_element_next_skip_delimiter(
    HEMP_SKIP_PROTO
) {
    // TODO: need to consider BRANCH for tag start, strings, etc.
    if (hemp_has_next(elemptr)) {
        hemp_go_next(elemptr);
        return hemp_skip_delimiter(elemptr);
    }
    return NULL;
}


hemp_element_p
hemp_element_next_skip_separator(
    HEMP_SKIP_PROTO
) {
    // TODO: need to consider BRANCH for tag start, strings, etc.
    if (hemp_has_next(elemptr)) {
        hemp_go_next(elemptr);
        return hemp_skip_separator(elemptr);
    }
    return NULL;
}



/*--------------------------------------------------------------------------
 * generic element parsing functions
 *--------------------------------------------------------------------------*/

hemp_element_p
hemp_element_dont_parse(
    HEMP_PARSE_PROTO
) {
    return NULL;
}


hemp_element_p
hemp_element_parse_block(
    HEMP_PARSE_PROTO
) {
    hemp_element_p element = *elemptr;
    hemp_list_p list       = hemp_element_parse_exprs(HEMP_PARSE_ARGS);
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
        block->value.block.exprs = list;
    }

    return block;
}


hemp_list_p
hemp_element_parse_exprs(
    HEMP_PARSE_PROTO
) {
    debug_call("hemp_element_parse_exprs()\n");

    hemp_element_p expr;
    hemp_list_p    exprs = hemp_list_init();

#if DEBUG
    debug("\n-- EXPRS --\n");
#endif

    while (1) {
        // todo: skip delimiter
        //debug_parse("about to skip delimiter:\n");
        hemp_skip_delimiter(elemptr);

//      debug_parse("about to parse expr:\n");

        /* tmp hack to catch TODO stuff while developing */
        if (! (*elemptr)->type->parse_expr) {
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
#if DEBUG
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


hemp_element_p
hemp_element_parse_expr(
    HEMP_PARSE_PROTO
) {
    debug_todo("hemp_element_parse_expr()\n");
    return NULL;
}


hemp_element_p
hemp_element_parse_binary(       // infix left
    HEMP_PARSE_PROTO, 
    hemp_element_p lhs
) {
    debug_blue("hemp_element_parse_binary()\n");
    hemp_element_p self = *elemptr;
    hemp_symbol_p  type = self->type;

    if (precedence && type->lprec <= precedence) {
        debug(
            "precedence of %s (%d) is lower than %s (%d), returning preceding element\n",
            type->name, type->lprec, lhs->type->name, precedence
        );
        return lhs;
    }
    else {
        debug(
            "precedence of %s (%d) is higher than %s (%d), continuing...\n",
            type->name, type->lprec, lhs->type->name, precedence
        );
    }

    self->value.binary.lhs = lhs;
//    debug_cyan("set lhs to %p / %p\n", self->value.binary.lhs, lhs);
    hemp_go_next(elemptr);

    hemp_element_p rhs = hemp_parse_expr(
        elemptr, scope, type->lprec, 1
    );

    if (! rhs)
        hemp_fatal("missing expression on rhs of %s\n", type->token);
        
    self->value.binary.rhs = rhs;
//    debug_cyan("set rhs to %p / %p\n", self->value.binary.rhs, rhs);
    
    hemp_skip_space(elemptr);

    return hemp_parse_infix(
        elemptr, scope, precedence, 0,
        self
    );
}



hemp_text_p
hemp_element_token(
    hemp_element_p  element,
    hemp_text_p     text
) {
    debug_call("hemp_element_token()\n");

    if (! text)
        text = hemp_text_init(element->length);
    
    hemp_text_append_cstrn(text, element->token, element->length);

    return text;
}


hemp_text_p
hemp_element_binary_text(
    hemp_element_p  element,
    hemp_text_p     text
) {
    debug_call("hemp_element_binary_text()\n");

    if (! text)
        text = hemp_text_init(element->length);

    struct hemp_binary_s exprs = element->value.binary;
    hemp_element_p lhs = exprs.lhs;
    hemp_element_p rhs = exprs.rhs;

    // debugging
    hemp_text_append_cstr(text, "(");

    if (lhs && lhs->type->text) {
        lhs->type->text(lhs, text);
    }
    else 
        hemp_text_append_cstr(text, "[LHS]");
        
    hemp_text_append_cstr(text, " ");
    hemp_text_append_cstrn(text, element->token, element->length);
    hemp_text_append_cstr(text, " ");

    if (rhs && rhs->type->text)
        rhs->type->text(rhs, text);
    else 
        hemp_text_append_cstr(text, "[RHS]");

    hemp_text_append_cstr(text, ")");

    return text;
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

    hemp_text_p text = e->type->source
        ? e->type->source(e, NULL)
        : e->type->text(e, NULL);

    hemp_cstr_p cstr = text ? text->string : "-- NO OUTPUT --";
    
    debug(
        "%03d:%02d %-20s %s[%s%s%s]%s\n", 
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
