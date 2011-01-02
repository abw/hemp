#include <hemp/test.h>

void test_grammar();
void test_hemp_grammar_instance();
void test_hemp_grammar_hemp();


int main(
    int argc, char **argv, char **env
) {
    plan(12);

    test_grammar();
    test_hemp_grammar_instance();
    test_hemp_grammar_hemp();

    return done();
}


void test_grammar() {
    Hemp hemp = hemp_new();
    ok( hemp, "created hemp object" );
    
    hemp_language_instance(hemp, HEMP_TT3);
    HempGrammar grammar;

    /* calling this multiple times has no effect */
    hemp_language_instance(hemp, HEMP_TT3);
    hemp_language_instance(hemp, HEMP_TT3);
    hemp_language_instance(hemp, HEMP_TT3);
    hemp_language_instance(hemp, HEMP_TT3);
    hemp_language_instance(hemp, HEMP_TT3);

    HEMP_TRY;
        grammar = hemp_grammar_instance(hemp, "tt3.core");
        ok( grammar, "got grammar" );
        is( grammar->name, "tt3.core", "grammar name is tt3.core" );
        HempElement space = hemp_grammar_element(grammar, "hemp.space");
        ok( space, "got space symbol" );
    HEMP_CATCH_ALL;
        fail("eek! caught error: %s", hemp->error->message);
    HEMP_END;
        
    HEMP_TRY;
        hemp_grammar_instance(hemp, "dud");
        fail("there should be no grammar called 'dud'");
    HEMP_CATCH_ALL;
        pass("caught error for dud grammar");
        ok( 
            hemp_string_eq(hemp->error->message, "Invalid grammar specified: dud"),
            "dud grammar message"
        );
    HEMP_END;
        
    hemp_free(hemp);
}



HempGrammar
hemp_grammar_test1(
    Hemp     hemp,
    HempString name
) {
    hemp_debug("constructing test grammar\n");
    HempGrammar grammar = (HempGrammar) hemp_grammar_new(hemp, name);

    HEMP_TRY;
        hemp_grammar_add_element(
            grammar, "hemp.text", "_TEXT", NULL, 0, 0
        );
        hemp_grammar_add_element(
            grammar, "hemp.space", "_SPACE", NULL, 0, 0
        );
        hemp_grammar_add_element(
            grammar, "hemp.numop.plus", "+", NULL, 42, 420
        );
        hemp_grammar_add_element(
            grammar, "hemp.boolop.and", "and", NULL, 42, 420
        );
        hemp_grammar_add_element(
            grammar, "hemp.textop.concat", "~", NULL, 42, 420
        );
        hemp_grammar_add_element(
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



void test_hemp_grammar_instance() {
    Hemp hemp = hemp_new();
    ok( hemp, "created hemp object at %p", hemp );

//    hemp_language_instance(hemp, HEMP_TT3);
    hemp_register_grammar(
        hemp, "test", (HempActor) &hemp_grammar_test1
    );
    
    HempGrammar grammar = hemp_grammar_instance(hemp, "test");
    ok( grammar, "got grammar" );

    hemp_free(hemp);
}

void test_hemp_grammar_hemp() {
    Hemp hemp = hemp_new();
    ok( hemp, "created hemp object at %p", hemp );

    HEMP_TRY;
        HempGrammar grammar_a = hemp_grammar_instance(hemp, "hemp.alpha");
        ok( grammar_a, "got hemp.alpha grammar" );

        HempGrammar grammar_b = hemp_grammar_instance(hemp, "hemp.bravo");
        ok( grammar_b, "got hemp.bravo grammar" );

    HEMP_CATCH_ALL;
        fail( "caught error: %s", hemp->error->message );

    HEMP_END;
    
//    hemp_ptree_dump(grammar->operators);

    hemp_free(hemp);
}