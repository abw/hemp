#include <hemp/test.h>

void test_grammar();
void test_hemp_grammar();
void test_hemp_grammar_hemp();


int main(
    int argc, char **argv, char **env
) {
    plan(11);

    test_grammar();
    test_hemp_grammar();
    test_hemp_grammar_hemp();

    return done();
}


void test_grammar() {
    hemp_p hemp = hemp_init();
    ok( hemp, "created hemp object" );
    
    hemp_language_p language = hemp_language(hemp, HEMP_TT3);
    hemp_grammar_p grammar;

    /* calling this multiple times has no effect */
    hemp_language(hemp, HEMP_TT3);
    hemp_language(hemp, HEMP_TT3);
    hemp_language(hemp, HEMP_TT3);
    hemp_language(hemp, HEMP_TT3);
    hemp_language(hemp, HEMP_TT3);

    HEMP_TRY;
        hemp_grammar_p grammar = hemp_grammar(hemp, "tt3.core");
        ok( grammar, "got grammar" );
        is( grammar->name, "tt3.core", "grammar name is tt3.core" );
    HEMP_CATCH_ALL;
        fail("caught error: %s", hemp->error->message);
    HEMP_END;
        
    HEMP_TRY;
        hemp_grammar(hemp, "dud");
        fail("there should be no grammar called 'dud'");
    HEMP_CATCH_ALL;
        pass("caught error for dud grammar");
        ok( 
            hemp_cstr_eq(hemp->error->message, "Invalid grammar specified: dud"),
            "dud grammar message"
        );
    HEMP_END;
        
    hemp_free(hemp);
}



hemp_grammar_p
hemp_grammar_test(
    hemp_p      hemp,
    hemp_cstr_p name
) {
    hemp_debug("constructing test grammar\n");
    hemp_grammar_p grammar = (hemp_grammar_p) hemp_grammar_init(hemp, name);

    HEMP_TRY;
        hemp_grammar_add_symbol(
            grammar, "hemp.text", "_TEXT", NULL, 0, 0
        );
        hemp_grammar_add_symbol(
            grammar, "hemp.space", "_SPACE", NULL, 0, 0
        );
        hemp_grammar_add_symbol(
            grammar, "hemp.numop.plus", "+", NULL, 42, 420
        );
        hemp_grammar_add_symbol(
            grammar, "hemp.boolop.and", "and", NULL, 42, 420
        );
        hemp_grammar_add_symbol(
            grammar, "hemp.textop.concat", "~", NULL, 42, 420
        );
        hemp_grammar_add_symbol(
            grammar, "hemp.numop.dud", "--", NULL, 55, 99
        );
//        hemp_grammar_add_symbol(
//            grammar, "hemp.space", "++", 69, 690
//        );
    HEMP_CATCH_ALL;
        pass( "caught error: %s", hemp->error->message );
    HEMP_END;

//    hemp_grammar_add_symbol(grammar, "++", HempElementText, 42, 69);
    return grammar;
}



void test_hemp_grammar() {
    hemp_p hemp = hemp_init();
    ok( hemp, "created hemp object at %p", hemp );

//    hemp_language(hemp, HEMP_TT3);
    hemp_register_grammar(
        hemp, "test", (hemp_actor_f) &hemp_grammar_test
    );
    
    hemp_grammar_p grammar = hemp_grammar(hemp, "test");
    ok( grammar, "got grammar" );

    hemp_free(hemp);
}

void test_hemp_grammar_hemp() {
    hemp_p hemp = hemp_init();
    ok( hemp, "created hemp object at %p", hemp );

    HEMP_TRY;
        hemp_grammar_p grammar_a = hemp_grammar(hemp, "hemp.alpha");
        ok( grammar_a, "got hemp.alpha grammar" );

        hemp_grammar_p grammar_b = hemp_grammar(hemp, "hemp.bravo");
        ok( grammar_b, "got hemp.bravo grammar" );

    HEMP_CATCH_ALL;
        fail( "caught error: %s", hemp->error->message );

    HEMP_END;
    
//    hemp_ptree_dump(grammar->operators);

    hemp_free(hemp);
}