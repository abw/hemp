#include <hemp/language/tt3.h>

/* see comment in language/hemp.c */
#define DONT_OPTIMISE_ME_AWAY  asm("");

hemp_template hemp_dialect_tt3_prepare(hemp_template template);
hemp_template hemp_dialect_tt3_unplugged_prepare(hemp_template template);
void hemp_dialect_tt3_cleanup(hemp_template template);


static struct hemp_symbols hemp_symbols_tt3_command[] = {
    { "tt3.command.if",   &hemp_element_tt3_if_symbol   },
    { "tt3.command.sub",  &hemp_element_sub_symbol      },
    { NULL, NULL },
};


HEMP_LANGUAGE(hemp_language_tt3) {
    hemp_debug_call("hemp_language_tt3_new(%p, %s)\n", hemp, name);

    hemp_language language = hemp_language_new(
        hemp, name, HEMP_LANGUAGE_TT3_VERSION
    );
    
    /* register handlers for command symbols */
    HEMP_ELEMENT("tt3.command.*", &hemp_element_tt3_command_symbols);

    /* register grammars */
    hemp_register_grammar(hemp, "tt3.core",     &hemp_grammar_tt3_core);
    hemp_register_grammar(hemp, "tt3.command",  &hemp_grammar_tt3_command);
    hemp_register_grammar(hemp, "tt3.control",  &hemp_grammar_tt3_control);

    /* register dialects */
    hemp_register_dialect(hemp, "tt3",           &hemp_dialect_tt3);
    hemp_register_dialect(hemp, "tt3.unplugged", &hemp_dialect_tt3_unplugged);

    return language;
}


/*--------------------------------------------------------------------------
 * dialects
 *--------------------------------------------------------------------------*/

HEMP_DIALECT(hemp_dialect_tt3) {
    hemp_dialect dialect = hemp_dialect_new(hemp, name);
    
    dialect->prepare = &hemp_dialect_tt3_prepare;
    dialect->scanner = &hemp_scan_text;
    dialect->cleanup = NULL;
    
    return dialect;
}


hemp_template
hemp_dialect_tt3_prepare(
    hemp_template template
    // TODO: options
) {
    hemp_debug_call("hemp_dialect_tt3_prepare(%p)\n", template);

    hemp_hemp    hemp    = template->dialect->hemp;
    hemp_tagset  tagset  = template->tagset;
    hemp_grammar command = hemp_grammar_instance(hemp, "tt3.command");
    hemp_grammar control = hemp_grammar_instance(hemp, "tt3.control");

    hemp_tagset_new_tag(tagset, "hemp.comment",  "comment",  "[#",   "#]", NULL);
    hemp_tagset_new_tag(tagset, "hemp.control",  "control",  "[?",   "?]", control);
    hemp_tagset_new_tag(tagset, "hemp.outline",  "outline",  "%%",   NULL, command);
    hemp_tagset_new_tag(tagset, "hemp.inline",   "inline",   "[%",   "%]", command);

//    hemp_tagset_add_inline_tag(tagset, HempTagVariable);
//    hemp_tagset_add_inline_tag(tagset, HempTagEmbed);

    return template;
}




HEMP_DIALECT(hemp_dialect_tt3_unplugged) {
    hemp_dialect dialect = hemp_dialect_new(hemp, name);
    
    dialect->prepare = &hemp_dialect_tt3_unplugged_prepare;
    dialect->scanner = &hemp_scan_unplugged;
    dialect->cleanup = NULL;
    
    return dialect;
}


hemp_template
hemp_dialect_tt3_unplugged_prepare(
    hemp_template template
) {
    hemp_debug_msg("hemp_dialect_tt3_unplugged_prepare(%p)\n", template);

    hemp_hemp    hemp    = template->dialect->hemp;
    hemp_tagset  tagset  = template->tagset;
    hemp_grammar command = hemp_grammar_instance(hemp, "tt3.command");

    hemp_tagset_new_tag(
        tagset, "hemp.unplugged", "unplugged",  NULL, NULL, command
    );

    return template;
}



/*--------------------------------------------------------------------------
 * tags
 *--------------------------------------------------------------------------*/



/*--------------------------------------------------------------------------
 * grammars
 *--------------------------------------------------------------------------*/

HEMP_GRAMMAR(hemp_grammar_tt3_core) {
    hemp_debug_call("hemp_grammar_tt3_core(%p, %s)\n", hemp, name);
    hemp_grammar grammar = hemp_grammar_hemp_charlie(hemp, name);
    HEMP_SYMBOL1("hemp.identity.true", "True");
    HEMP_SYMBOL1("hemp.identity.false", "False");
    HEMP_SYMBOL2("hemp.squote", "q<<", ">>");
    HEMP_OPERATOR1("hemp.terminator", "end", 0, 0);

    return grammar;
}


HEMP_GRAMMAR(hemp_grammar_tt3_command) {
    hemp_debug_call("hemp_grammar_tt3_command(%p, %s)\n", hemp, name);
    hemp_grammar grammar = hemp_grammar_tt3_core(hemp, name);
    HEMP_OPERATOR1("tt3.command.if", "if", 100, 100);
    HEMP_OPERATOR2("tt3.command.sub", "sub", "end", 100, 100);
    return grammar;
}


HEMP_GRAMMAR(hemp_grammar_tt3_control) {
    hemp_debug_call("hemp_grammar_tt3_control(%p, %s)\n", hemp, name);
    hemp_grammar grammar = hemp_grammar_tt3_core(hemp, name);
    return grammar;
}



/*--------------------------------------------------------------------------
 * elements
 *--------------------------------------------------------------------------*/

hemp_action
hemp_element_tt3_command_symbols(
    hemp_hemp     hemp,
    hemp_string name
) {
    hemp_debug_init("** Initialising tt3 command symbols (%s requested)\n", name);

    /* we should detect if we've done this already and skip it */
    hemp_register_elements(
        hemp, hemp_symbols_tt3_command
    );

    /* now try again */
    return (hemp_action) hemp_hash_fetch_pointer(
        hemp->element->constructors, name
    );
}


hemp_symbol
hemp_element_tt3_TODO_symbol(
    hemp_hemp        hemp,
    hemp_symbol symbol
) {
    hemp_todo("tt3 constructor for %s symbol", symbol->name);
    DONT_OPTIMISE_ME_AWAY;
    return symbol;
}


hemp_symbol
hemp_element_tt3_if_symbol(
    hemp_hemp        hemp,
    hemp_symbol symbol
) {
    return hemp_element_tt3_TODO_symbol(hemp, symbol);
}


