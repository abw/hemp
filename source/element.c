#include "hemp/element.h"
#include "hemp/debug.h"


hemp_element_t
hemp_element_init() {
    hemp_element_t element;

    if ((element = (hemp_element_t) hemp_mem_init(sizeof(struct hemp_element)))) {
        debug_mem("Allocated new element at %p\n", element);
    }
    
    return element;
}


void
hemp_element_free(hemp_element_t element) {
    if (element) {
        debug_mem("Releasing element at %p\n", element);
        hemp_mem_free(element);
    }
}


/*--------------------------------------------------------------------------
 * front-end parsing pseudo-method
 *--------------------------------------------------------------------------*/

hemp_element_t
hemp_element_parse(
    hemp_element_t element
) {
    debug_call("hemp_element_parse()\n");

    hemp_element_t block = hemp_element_parse_block(
        &element,
        NULL,
        0, 
        HEMP_FALSE
    );
    
    if (block) {
        hemp_text_t text = block->type->text(block, NULL);
        debug("OUTPUT: %s\n", text->string);
        hemp_text_free(text);
    }
    else {
        debug_red("did not parse a block\n");
    }
    
    return block;
        
}



/*--------------------------------------------------------------------------
 * Simple functions for skipping over tokens.  I know, it looks tempting to 
 * define these as macros, but they have to be real functions so we can store
 * pointers to them in the element vtables.
 *--------------------------------------------------------------------------*/

hemp_element_t
hemp_element_dont_skip(
    HEMP_SKIP_PROTO
) {
    /* no-op to return the current element */
    return *elemptr;
}


hemp_element_t
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


hemp_element_t
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


hemp_element_t
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

hemp_element_t
hemp_element_dont_parse(
    HEMP_PARSE_PROTO
) {
    return NULL;
}



hemp_element_t
hemp_element_parse_block(
    HEMP_PARSE_PROTO
) {
    hemp_element_t element = *elemptr;
    hemp_list_t list       = hemp_element_parse_exprs(HEMP_PARSE_ARGS);
    hemp_element_t block   = NULL;

    if (list) {
        // debug("got list of %d exprs\n", list->length);
        block           = hemp_element_init();
        block->type     = HempElementBlock;
        block->token    = element->token;
        block->position = element->position;
        block->length   = element->length;
        block->value.block.exprs = list;
    }

    return block;
}


hemp_list_t
hemp_element_parse_exprs(
    HEMP_PARSE_PROTO
) {
    debug_call("hemp_element_parse_exprs()\n");

    hemp_element_t expr;
    hemp_list_t    exprs = hemp_list_init();
    
    debug("\n-- EXPRS --\n");

    while (1) {
        // todo: skip delimiter
        //debug_parse("about to skip delimiter:\n");
        hemp_skip_delimiter(elemptr);
        //debug_parse("about to parse expr:\n");

        expr = hemp_parse_expr(elemptr, scope, precedence, HEMP_FALSE);
        
        if (! expr)
            break;

        // debug_parse("parsed %s expression:\n", expr->type->name);
        hemp_element_dump(expr);
        hemp_list_push(exprs, expr);
    }

    /* element should be EOF or we hit a duff token */
    if (hemp_at_eof(elemptr)) {
        debug_parse("%sReached EOF\n%s\n", ANSI_GREEN, ANSI_RESET);
    }
    else {
        debug_parse(stderr, "%sNot an expression: %s:%s\n", ANSI_RED, (*elemptr)->type->name, ANSI_RESET);
        hemp_element_dump(*elemptr);
    }
    
    // debug("n expressions: %d\n", exprs->length);
    
    if (! exprs->length && ! force) {
        // debug("clearing empty list\n");
        hemp_list_null(exprs);
    }
    
    return exprs;
}


hemp_element_t
hemp_element_parse_expr(
    HEMP_PARSE_PROTO
) {
    debug_call("hemp_element_parse_expr()\n");
    return NULL;
}



/*--------------------------------------------------------------------------
 * debugging functions
 *--------------------------------------------------------------------------*/

hemp_bool_t
hemp_element_dump(
    hemp_element_t e
) {
    hemp_text_t text = e->type->text(e, NULL);
    hemp_cstr_t cstr = text ? text->string : "-- NO OUTPUT --";
    
    debug("%03d:%02d %-12s |%s%s%s|\n", (int) e->position, (int) e->length, e->type->name, ANSI_YELLOW, cstr, ANSI_RESET);

    if (text) {
        hemp_text_free(text);
    }
    
    return hemp_cstr_eq(e->type->name, "EOF")
        ? HEMP_FALSE
        : HEMP_TRUE;
}
