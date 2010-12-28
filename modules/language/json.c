#include <hemp.h>

#define HEMP_LANGUAGE_JSON_VERSION 0.1

HEMP_LANGUAGE(hemp_language_json);
HEMP_DIALECT(hemp_dialect_json);
HEMP_GRAMMAR(hemp_grammar_json);
HEMP_DOC_SCAN(hemp_dialect_json_scanner);
//HEMP_DOC_PREP(hemp_dialect_json_prepare);
//HEMP_DOC_CLEAN(hemp_dialect_json_cleanup);


/*--------------------------------------------------------------------------
 * Binder function registers the json dialect with the hemp instance
 *--------------------------------------------------------------------------*/

hemp_bool
hemp_module_binder(
    hemp_module module,
    hemp_hemp   hemp
) {
    hemp_register_language( hemp, "json", &hemp_language_json );
    hemp_register_grammar ( hemp, "json", &hemp_grammar_json  );
    hemp_register_dialect ( hemp, "json", &hemp_dialect_json  );
    return HEMP_TRUE;
}


/*--------------------------------------------------------------------------
 * Language constructor
 *--------------------------------------------------------------------------*/

HEMP_LANGUAGE(hemp_language_json) {
    return hemp_language_new(
        hemp, name, HEMP_LANGUAGE_JSON_VERSION
    );
}


HEMP_DIALECT(hemp_dialect_json) {
    hemp_dialect dialect = hemp_dialect_new(hemp, name);
    dialect->scanner = &hemp_dialect_json_scanner;
//  dialect->prepare = &hemp_dialect_json_prepare;
//  dialect->cleanup = &hemp_dialect_json_cleanup;
    return dialect;
}


HEMP_GRAMMAR(hemp_grammar_json) {
    hemp_grammar grammar = (hemp_grammar) hemp_grammar_new(hemp, name);

    HEMP_USE_ELEMENT0("hemp.text");
    HEMP_USE_ELEMENT0("hemp.space");
    HEMP_USE_ELEMENT0("hemp.block");
    HEMP_USE_ELEMENT1("hemp.identity.true",            "true"     );
    HEMP_USE_ELEMENT1("hemp.identity.false",           "false"    );
    HEMP_USE_ELEMENT1("hemp.identity.nothing",         "null"     );
    HEMP_USE_ELEMENT1("hemp.terminator",               "]"        );
    HEMP_USE_ELEMENT1("hemp.terminator",               "}"        );
    HEMP_USE_ELEMENT1("hemp.delimiter",                ","        );
    HEMP_USE_ELEMENT1("hemp.operator.assign.equals",   ":"        )
    HEMP_USE_ELEMENT2("hemp.bracket.list",             "[", "]"   );
    HEMP_USE_ELEMENT2("hemp.bracket.hash",             "{", "}"   );
    HEMP_USE_ELEMENT2("hemp.dquote",                   "\"", "\"" );
    HEMP_USE_ELEMENT2("hemp.squote",                   "'",  "'"  );

    return grammar;
}


HEMP_DOC_SCAN(hemp_dialect_json_scanner) {
    hemp_hemp    hemp    = document->dialect->hemp;
    hemp_grammar grammar = hemp_grammar_instance(hemp, "json");

    return hemp_grammar_scanner(
        grammar, document
    );
}

