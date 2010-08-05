#include <hemp.h>

#define HEMP_TT3_LANGUAGE_VERSION 0.2

HEMP_DIALECT_PROTO(hemp_dialect_tt3);
HEMP_GRAMMAR_PROTO(hemp_grammar_tt3_core);
HEMP_GRAMMAR_PROTO(hemp_grammar_tt3_command);
HEMP_GRAMMAR_PROTO(hemp_grammar_tt3_control);
HEMP_SYMBOLS_PROTO(hemp_element_tt3_command_symbols);
HEMP_ELEMENT_PROTO(hemp_element_tt3_if_symbol);
HEMP_ELEMENT_PROTO(hemp_element_tt3_TODO_symbol);

hemp_template_p hemp_dialect_tt3_prepare(hemp_template_p tmpl);
void hemp_dialect_tt3_cleanup(hemp_template_p tmpl);


static struct hemp_symbols_s hemp_symbols_tt3_command[] = {
    { "tt3.if",  &hemp_element_tt3_if_symbol },
};


hemp_language_p
hemp_language_tt3_init(
    hemp_p      hemp,
    hemp_cstr_p name
) {
    debug("initialising %s language\n", name);
    hemp_language_p language = hemp_language_init(hemp, name, HEMP_TT3_LANGUAGE_VERSION);
    
    /* register handlers for command symbols */
    HEMP_ELEMENT("tt3.command.*", &hemp_element_tt3_command_symbols);

    /* register grammars */
    HEMP_GRAMMAR("tt3.core",    &hemp_grammar_tt3_core);
    HEMP_GRAMMAR("tt3.command", &hemp_grammar_tt3_command);
    HEMP_GRAMMAR("tt3.control", &hemp_grammar_tt3_control);

    /* register dialects */
    HEMP_DIALECT(HEMP_TT3, &hemp_dialect_tt3);

//    hemp_register_grammar(
//        hemp, HEMP_TT3, (hemp_actor_f) &hemp_grammar_tt3
//    );

    return language;
}


/*--------------------------------------------------------------------------
 * dialects
 *--------------------------------------------------------------------------*/

hemp_dialect_p
hemp_dialect_tt3(
    hemp_p      hemp,
    hemp_cstr_p name
) {
    hemp_dialect_p dialect = hemp_dialect_init(hemp, name);
    
    dialect->prepare = &hemp_dialect_tt3_prepare;
    dialect->scanner = &hemp_scan_text;
    dialect->cleanup = NULL; // &hemp_dialect_tt3_cleanup;
    
    return dialect;
}


hemp_template_p
hemp_dialect_tt3_prepare(
    hemp_template_p tmpl
    // TODO: options
) {
    debug_yellow("** Running TT3 dialect template prepare **\n");
    hemp_tagset_p tagset = tmpl->tagset;
    
    hemp_tagset_add_tag(
        tagset, 
        hemp_tag_init(
            "comment", 
            HEMP_INLINE,
            &hemp_scan_comment_tag,
            "[#", "#]"
        )
    );

    hemp_tagset_add_tag(
        tagset, 
        hemp_tag_init(
            "control", 
            HEMP_INLINE,
            &hemp_scan_control_tag,
            "[?", "?]"
        )
    );

    hemp_tagset_add_tag(
        tagset, 
        hemp_tag_init(
            "outline", 
            HEMP_OUTLINE,
            &hemp_scan_outline_tag,
            "%%", NULL
        )
    );

    hemp_tagset_add_tag(
        tagset, 
        hemp_tag_init(
            "inline", 
            HEMP_INLINE,
            &hemp_scan_inline_tag,
            "[%", "%]"
        )
    );

//    hemp_tagset_add_inline_tag(tagset, HempTagVariable);
//    hemp_tagset_add_inline_tag(tagset, HempTagEmbed);

    return tmpl;
}


void
hemp_dialect_tt3_cleanup(
    hemp_template_p tmpl
) {
    debug_yellow("** Running TT3 dialect template cleanup **\n");
}



/*--------------------------------------------------------------------------
 * grammars
 *--------------------------------------------------------------------------*/

hemp_grammar_p
hemp_grammar_tt3_core(
    hemp_p      hemp,
    hemp_cstr_p name
) {
    debug("constructing tt3 core grammar\n");
    hemp_grammar_p grammar = (hemp_grammar_p) hemp_grammar_init(hemp, name);
    return grammar;
}


hemp_grammar_p
hemp_grammar_tt3_command(
    hemp_p      hemp,
    hemp_cstr_p name
) {
    debug("constructing tt3 command grammar\n");
    hemp_grammar_p grammar = hemp_grammar_tt3_core(hemp, name);
    return grammar;
}


hemp_grammar_p
hemp_grammar_tt3_control(
    hemp_p      hemp,
    hemp_cstr_p name
) {
    debug("constructing tt3 control grammar\n");
    hemp_grammar_p grammar = hemp_grammar_tt3_core(hemp, name);
    return grammar;
}



/*--------------------------------------------------------------------------
 * elements
 *--------------------------------------------------------------------------*/

hemp_action_p
hemp_element_tt3_command_symbols(
    hemp_p      hemp,
    hemp_cstr_p name
) {
    debug("initialising tt3 command symbols (%s requested)\n", name);

    /* we should detect if we've done this already and skip it */
    HEMP_ELEMENTS(hemp_symbols_tt3_command);

    /* now try again */
    return (hemp_action_p) hemp_hash_fetch(
        hemp->elements->constructors, name
    );
}


hemp_symbol_p
hemp_element_tt3_if_symbol(
    hemp_p        hemp,
    hemp_symbol_p symbol
) {
    hemp_element_tt3_TODO_symbol(hemp, symbol);
}


hemp_symbol_p
hemp_element_tt3_TODO_symbol(
    hemp_p        hemp,
    hemp_symbol_p symbol
) {
    debug("TODO: tt3 constructor for %s symbol\n", symbol->name);
    return symbol;
}
