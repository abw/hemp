#include <hemp/parser.h>


hemp_list
hemp_parse_exprs(
    HEMP_PREFIX_ARGS
) {
    hemp_debug_call("hemp_parse_exprs( precedence => %d )\n", precedence);

    hemp_fragment   expr;
    hemp_list       exprs = hemp_list_new();
//  hemp_debug_msg("hemp_parse_exprs() LIST: %p\n", exprs);

    while (1) {
        /* skip whitespace, delimiters (commas) and separators (semi-colons) */
        hemp_skip_separator(fragptr);

        /* ask the next token to return an expression */
        hemp_debug_parse("%s parse_prefix: %p\n", (*fragptr)->type->name, (*fragptr)->type->parse_prefix);
        expr = hemp_parse_prefix(fragptr, scope, precedence, HEMP_FALSE);

        /* if it's not an expression (e.g. a terminator) then we're done */
        if (! expr)
            break;

        hemp_debug_parse("expr: %s\n", expr->type->name);
        hemp_list_push(exprs, hemp_frag_val(expr));
    }

    /* element should be EOF or we hit a duff token */
    if (hemp_at_eof(fragptr)) {
        hemp_debug_parse("%sReached EOF\n%s\n", HEMP_ANSI_GREEN, HEMP_ANSI_RESET);
    }
    else {
        hemp_debug_parse("%sNot an expression: %s:%s\n", HEMP_ANSI_RED, (*fragptr)->type->name, HEMP_ANSI_RESET);
    }

    // hemp_debug("n expressions: %d\n", exprs->length);
    
    if (! exprs->length && ! force) {
        hemp_list_free(exprs);
        exprs = NULL;
    }

#if HEMP_DEBUG & HEMP_DEBUG_PARSE
    hemp_fragment_dump_exprs(exprs);
#endif
    
    return exprs;
}


HEMP_PREFIX(hemp_parse_block) {
    hemp_debug_call("hemp_parse_block()\n");
    hemp_fragment   fragment    = *fragptr;
    hemp_list       list        = hemp_parse_exprs(HEMP_PREFIX_ARG_NAMES);
    hemp_fragment   block       = NULL;

    if (list) {
        block = hemp_fragments_add_fragment(
            hemp_fragment_fragments(fragment),
            HempElementBlock,
            fragment->token, fragment->position, 0
        );
        hemp_set_block_exprs_list(block, list);
    }

    return block;
}



