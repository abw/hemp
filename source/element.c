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
 * basic element functions
 *--------------------------------------------------------------------------*/

hemp_element_t
hemp_element_skip_space(
    HEMP_SKIP_PROTO
) {
    /* Most elements are NOT whitespace so we return the current element and
     * ensure that the element pointer is pointing at it
     */
    *elemptr = element;
    return element;
}


hemp_element_t
hemp_element_next_skip_space(
    HEMP_SKIP_PROTO
) {
    return element->next
        ? hemp_skip_space(element->next, elemptr)
        : NULL;
}


hemp_element_t
hemp_element_parse(
    hemp_element_t element
) {
    debug("hemp_element_parse()\n");
    return hemp_element_parse_block(
        element,
        &element,
        NULL,
        0, 
        HEMP_FALSE
    );
}


hemp_element_t
hemp_element_parse_block(
    HEMP_PARSE_PROTO
) {
    hemp_element_t list = hemp_element_parse_exprs(HEMP_PARSE_ARGS);
    debug("got list of exprs\n");

    // return $self->[META]->[ELEMS]->create(
    //    block => $self->[TOKEN], $self->[POS], $exprs
    // );

    return NULL;
}


hemp_element_t
hemp_element_parse_exprs(
    HEMP_PARSE_PROTO
) {
    hemp_element_t expr;
    debug("hemp_element_parse_exprs()\n");

    while (1) {
        debug_yellow("TOKEN: ");
        hemp_element_dump(*elemptr);
        
        expr = hemp_parse_expr(elemptr, scope, precedence, HEMP_FALSE);

        if (expr) {
            debug_green("parsed expression:\n");
            hemp_element_dump(expr);
        }
        else {
            debug_red("no parsed expression");
            break;
        }
    }
    
    return NULL;
}


hemp_element_t
hemp_element_parse_expr(
    HEMP_PARSE_PROTO
) {
    hemp_element_t expr;
    debug("hemp_element_parse_expr()");
    
    return NULL;
}




hemp_bool_t
hemp_element_dump(
    hemp_element_t e
) {
    debug("%03d:%02d %-12s |%s%s%s|\n", (int) e->position, (int) e->length, e->type->name, ANSI_YELLOW, e->type->text(e), ANSI_RESET);
    return hemp_cstr_eq(e->type->name, "EOF")
        ? HEMP_FALSE
        : HEMP_TRUE;
}
