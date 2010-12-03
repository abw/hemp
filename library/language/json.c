#include <hemp/language/json.h>


HEMP_LANGUAGE(hemp_language_json) {
    hemp_debug_call("hemp_language_json(%p, %s)\n", hemp, name);

    hemp_language language = hemp_language_new(
        hemp, name, HEMP_LANGUAGE_JSON_VERSION
    );
    
    hemp_register_grammar(hemp, "json", &hemp_grammar_json);
    hemp_register_dialect(hemp, "json", &hemp_dialect_json);

    return language;
}


HEMP_DIALECT(hemp_dialect_json) {
    hemp_dialect dialect = hemp_dialect_new(hemp, name);
    
    dialect->prepare = &hemp_dialect_json_prepare;
    dialect->scanner = &hemp_scan_unplugged;
    dialect->cleanup = NULL;
    
    return dialect;
}


HEMP_GRAMMAR(hemp_grammar_json) {
    hemp_debug_call("hemp_grammar_json_core(%p, %s)\n", hemp, name);

    hemp_grammar grammar = (hemp_grammar) hemp_grammar_new(hemp, name);

    HEMP_SYMBOL0("hemp.text");
    HEMP_SYMBOL0("hemp.space");
    HEMP_SYMBOL0("hemp.block");
    HEMP_SYMBOL1("hemp.identity.true",            "true"     );
    HEMP_SYMBOL1("hemp.identity.false",           "false"    );
    HEMP_SYMBOL1("hemp.identity.nothing",         "null"     );
    HEMP_SYMBOL1("hemp.terminator",               "]"        );
    HEMP_SYMBOL1("hemp.terminator",               "}"        );
    HEMP_SYMBOL1("hemp.delimiter",                ","        );
    HEMP_SYMBOL1("hemp.operator.assign.equals",   ":"        )
    HEMP_SYMBOL2("hemp.bracket.list",             "[", "]"   );
    HEMP_SYMBOL2("hemp.bracket.hash",             "{", "}"   );
    HEMP_SYMBOL2("hemp.dquote",                   "\"", "\"" );

    return grammar;
}


HEMP_PREPARE(hemp_dialect_json_prepare) {
    hemp_debug_call("hemp_dialect_json_prepare(%p)\n", template);
    hemp_hemp    hemp    = template->dialect->hemp;
    hemp_tagset  tagset  = template->tagset;
    hemp_grammar grammar = hemp_grammar_instance(hemp, "json");
    hemp_tagset_new_tag(tagset, "hemp.unplugged", "json",  NULL, NULL, grammar);
    return template;
}

