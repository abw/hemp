#include <hemp/language/tt3.h>


static struct hemp_elements hemp_elements_tt3_command[] = {
    { "tt3.command.if",     &hemp_element_command_if      },
    { "tt3.command.else",   &hemp_element_command_else    },
    { "tt3.command.elsif",  &hemp_element_command_elsif   },
    { "tt3.command.sub",    &hemp_element_sub             },
    { "tt3.command.just",   &hemp_element_command_just    },
    { "tt3.command.with",   &hemp_element_command_with    },
    { NULL, NULL },
};


HEMP_LANGUAGE(hemp_language_tt3) {
    hemp_debug_call("hemp_language_tt3_new(%p, %s)\n", hemp, name);

    HempLanguage language = hemp_language_new(
        hemp, name, HEMP_LANGUAGE_TT3_VERSION
    );
    
    /* register handlers for command symbols */
    hemp_register_element(hemp, "tt3.command.*", &hemp_elements_tt3_command_registrar);

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
    HempDialect dialect = hemp_dialect_new(hemp, name);
    dialect->scanner = &hemp_dialect_tt3_scanner;
    return dialect;
}

HEMP_DOC_SCAN(hemp_dialect_tt3_scanner) {
    hemp_debug_call("hemp_dialect_tt3_scanner(%p)\n", document);

    Hemp    hemp    = document->hemp;
    HempTagset  tagset  = hemp_tagset_new(document);
    HempGrammar command = hemp_grammar(hemp, "tt3.command");
    HempGrammar control = hemp_grammar(hemp, "tt3.control");
    HempBool    result;

    hemp_tagset_new_tag(tagset, "hemp.comment",  "comment",  "[#",   "#]", NULL);
    hemp_tagset_new_tag(tagset, "hemp.control",  "control",  "[?",   "?]", control);
    hemp_tagset_new_tag(tagset, "hemp.outline",  "outline",  "%%",   NULL, command);
    hemp_tagset_new_tag(tagset, "hemp.inline",   "inline",   "[%",   "%]", command);

    HEMP_TRY;
        result = hemp_tagset_scanner(
            tagset, document
        );
    HEMP_CATCH_ALL;
        hemp_tagset_free(tagset);
        HEMP_RETHROW;
    HEMP_END;

    hemp_tagset_free(tagset);

    return result;
}

/*--------------------------------------------------------------------------
 * Unplugged
 *--------------------------------------------------------------------------*/

HEMP_DIALECT(hemp_dialect_tt3_unplugged) {
    HempDialect dialect = hemp_dialect_new(hemp, name);
    dialect->scanner = &hemp_dialect_tt3_unplugged_scanner;
    return dialect;
}


HEMP_DOC_SCAN(hemp_dialect_tt3_unplugged_scanner) {
    hemp_debug_msg("hemp_dialect_tt3_unplugged_scanner(%p)\n", document);

    return hemp_grammar_scanner(
        hemp_grammar(document->hemp, "tt3.command"),
        document
    );
}


/*--------------------------------------------------------------------------
 * grammars
 *--------------------------------------------------------------------------*/

HEMP_GRAMMAR(hemp_grammar_tt3_core) {
    hemp_debug_call("hemp_grammar_tt3_core(%p, %s)\n", hemp, name);
    HempGrammar grammar = hemp_grammar_hemp_charlie(hemp, name);

    HEMP_USE_ELEMENT1("hemp.identity.true",  "True");
    HEMP_USE_ELEMENT1("hemp.identity.false", "False");
    HEMP_USE_ELEMENT2("hemp.squote", "q<<", ">>");
    HEMP_USE_OPERATOR1("hemp.terminator", "end", 0, 0);

/*  just testing
 *  hemp_grammar_feature(hemp, grammar, "ansi");
 */

    return grammar;
}


HEMP_GRAMMAR(hemp_grammar_tt3_command) {
    hemp_debug_call("hemp_grammar_tt3_command(%p, %s)\n", hemp, name);
    HempGrammar grammar = hemp_grammar_tt3_core(hemp, name);

    HEMP_USE_COMMAND2("tt3.command.if",    "if",   "end");
    HEMP_USE_COMMAND1("tt3.command.elsif", "elsif");
    HEMP_USE_COMMAND1("tt3.command.else",  "else");
    HEMP_USE_COMMAND2("tt3.command.sub",   "sub",  "end");
    HEMP_USE_COMMAND2("tt3.command.just",  "just", "end");
    HEMP_USE_COMMAND2("tt3.command.with",  "with", "end");

//    HempElement intag = HEMP_USE_ELEMENT2("hemp.tag.inline", "[%", "%]");
//  hemp_debug_msg("embedded tag: %p => %s\n", intag, intag->name); 
    return grammar;
}


HEMP_GRAMMAR(hemp_grammar_tt3_control) {
    hemp_debug_call("hemp_grammar_tt3_control(%p, %s)\n", hemp, name);
    HempGrammar grammar = hemp_grammar_tt3_core(hemp, name);
    return grammar;
}



/*--------------------------------------------------------------------------
 * elements
 *--------------------------------------------------------------------------*/


HEMP_ELEMENTS(hemp_elements_tt3_command_registrar) {
    hemp_debug_init("** Initialising tt3 command symbols (%s requested)\n", name);

    /* we should detect if we've done this already and skip it */
    hemp_register_elements(hemp, hemp_elements_tt3_command);

    /* now try again */
    return (HempAction) hemp_hash_fetch_pointer(
        hemp->element->constructors, name
    );
}




