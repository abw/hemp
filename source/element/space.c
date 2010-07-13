#include "hemp/elements.h"
#include "hemp/debug.h"


static struct hemp_etype
    hemp_element_space = { 
        "space",
        /* asking whitespace to skip_space forwards to next token */
        &hemp_element_next_skip_space,
        &hemp_space_parse_expr,
        &hemp_element_text_text
    };

static struct hemp_etype
    hemp_element_tag_start = { 
        "tag_start",
        &hemp_element_next_skip_space,
        &hemp_space_parse_expr,
        &hemp_element_text_text
    };

static struct hemp_etype
    hemp_element_tag_end = { 
        "tag_end",
        &hemp_element_next_skip_space,
        &hemp_space_parse_expr,
        &hemp_element_text_text
    };


hemp_etype_t HempElementSpace     = &hemp_element_space;
hemp_etype_t HempElementTagStart  = &hemp_element_tag_start;
hemp_etype_t HempElementTagEnd    = &hemp_element_tag_end;


hemp_element_t
hemp_space_parse_expr(
    HEMP_PARSE_PROTO
) {
    /* advance the pointer to the next element after this one and return the 
     * current element as the yielded expression 
     */
    debug("hemp_space_parse_expr()\n");
    hemp_element_next_skip_space(element, elemptr);
    debug("next element after space: ");
    hemp_element_dump(*elemptr);

    return hemp_parse_expr(elemptr, scope, precedence, force);
}

