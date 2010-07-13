#include "hemp/elements.h"
#include "hemp/debug.h"


hemp_element_t
hemp_literal_parse_expr(
    HEMP_PARSE_PROTO
) {
    /* advance the pointer to the next element after this one and return the 
     * current element as the yielded expression 
     */
    debug("hemp_literal_parse_expr()");
    *elemptr = element->next;
    return element;
}

