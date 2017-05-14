#include <hemp/fragment.h>


/*--------------------------------------------------------------------------
 * initialisation and cleanup functions
 *--------------------------------------------------------------------------*/

HempFragment
hemp_fragment_init(
    HempFragment   fragment,
    HempElement    type,
    HempString     token,
    HempPos        position,
    HempSize       length
) {
    HEMP_INSTANCE(fragment);

    /* NOTE: this function isn't used in the general case because
     * fragments are slab-allocated by the fragments module - see
     * hemp_fragments_new_fragment() in fragments.c
     */
    fragment->fragments  = NULL;
    fragment->next       = NULL;
    fragment->branch     = NULL;
    fragment->type       = type;
    fragment->token      = token;
    fragment->position   = position;
    fragment->length     = length;
    fragment->flags      = type->flags;
    fragment->op.value   = HempMissing;

    return fragment;
}


void
hemp_fragment_free(
    HempFragment fragment
) {
    hemp_mem_free(fragment);
}


HempGrammar
hemp_fragment_grammar(
    HempFragment fragment
) {
    if (! fragment->type->grammar)
        hemp_fatal(
            "No grammar defined for %s fragment",
            fragment->type->name
        );

    return fragment->type->grammar;
}


HempFragments
hemp_fragment_fragments(
    HempFragment fragment
) {
    if (! fragment->fragments)
        hemp_fatal(
            "No fragments defined for %s fragment",
            fragment->type->name
    );

    return fragment->fragments;
}


HempDocument
hemp_fragment_document(
    HempFragment fragment
) {
    return hemp_fragments_document(
        hemp_fragment_fragments(
            fragment
        )
    );
}


HempElement
hemp_fragment_grammar_element(
    HempFragment fragment,
    HempString   name
) {
    return hemp_grammar_element(
        hemp_fragment_grammar(fragment),
        name
    );
}

HempFragment
hemp_fragment_new_fragment(
    HempFragment fragment,
    HempString   typename
) {
    return hemp_fragments_add_fragment(
        hemp_fragment_fragments(fragment),
        hemp_fragment_grammar_element(fragment, typename),
        fragment->token, fragment->position, 0
    );
}


HempElement
hemp_fragment_retype(
    HempFragment fragment,
    HempString   typename
) {
    HempElement type    = fragment->type;
    HempGrammar grammar = type->grammar;

    if (grammar) {
        hemp_debug("found fragment grammar\n");
    }
    else {
        hemp_fatal("No grammar defined for fragment type: %s", type->name);
    }

    type = hemp_grammar_element(grammar, typename);

    if (! type)
        hemp_fatal(
            "Invalid fragment type specified (symbol not found in %s grammar): %s",
            grammar->name, typename
        );

    fragment->type = type;

    return type;
}



/*--------------------------------------------------------------------------
 * parsing functions
 *--------------------------------------------------------------------------*/

HempFragment
hemp_fragment_parse(
    HempFragment fragment,
    HempScope    scope
) {
    hemp_debug_call("hemp_fragment_parse()\n");
    HempFragment *current = &fragment;

    HempFragment block = hemp_parse_block(
        current,
        scope,
        0,
        HEMP_FALSE
    );

    HempFragment next_frag = *current;

    if (next_frag->type != HempElementEOF) {
        // TODO: should print token, not element type
        HEMP_PARSE_ERROR(next_frag, HEMP_ERROR_UNEXPECTED, next_frag->type->name);
    }

    //hemp_fatal("Unexpected token: %s\n", next_frag->type->name);

    return block;
}



/*--------------------------------------------------------------------------
 * debugging functions
 *--------------------------------------------------------------------------*/

HempBool
hemp_fragment_dump(
    HempFragment f
) {
    HempContext context = hemp_context_new(NULL);       // tmp ugly hack

    if (! f->type->text)
        hemp_fatal("%s type does not define a text() method", f->type->name);

    HempValue output = f->type->token
        ? f->type->token(hemp_frag_val(f), context, HempNothing)
        : f->type->text(hemp_frag_val(f), context, HempNothing);


    HempText text  = hemp_val_text(output);
    HempString string = text ? text->string : "-- NO OUTPUT --";

    hemp_debug(
        "%p %03d:%02d %-20s %s[%s%s%s]%s\n", f,
        (int) f->position, (int) f->length, f->type->name,
        HEMP_ANSI_BLUE, HEMP_ANSI_YELLOW, string, HEMP_ANSI_BLUE, HEMP_ANSI_RESET
    );

    hemp_context_free(context);

    return hemp_string_eq(f->type->name, "EOF")
        ? HEMP_FALSE
        : HEMP_TRUE;
}


void hemp_fragment_dump_exprs(
    HempList exprs
) {
    HempSize n;

    hemp_debug("\n-- exprs --\n");

    for (n = 0; n < exprs->length; n++) {
        HempValue    v = hemp_list_item(exprs, n);
        HempFragment f = hemp_val_frag(v);
        hemp_fragment_debug(f);
    }

    hemp_debug("-- /exprs --\n");
}


void
hemp_fragment_debug(
    HempFragment f
) {
    static char buffer[80];
    HempPos len = f->length;
    if (len > 79) len = 79;
    strncpy(buffer, f->token, len);
    buffer[len] = '\0';

    hemp_debug(
        "%s%03d:%02d %s%-20s %s[%s%s%s]%s\n",
        HEMP_ANSI_CYAN, (int) f->position, (int) f->length,
        HEMP_ANSI_BLUE, f->type->name,
        HEMP_ANSI_BLUE, HEMP_ANSI_RESET,
        buffer,
        HEMP_ANSI_BLUE, HEMP_ANSI_RESET
    );
}
