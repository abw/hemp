#include <hemp.h>
#include <tap.h>

void test_language();


int
main(
    int  argc, 
    char **argv, 
    char **env
) {
    plan_tests(14);
    test_language();
    hemp_mem_trace_ok();
    hemp_mem_trace_reset();

    test_hemp_grammar();
    hemp_mem_trace_ok();
    hemp_mem_trace_reset();

    test_hemp_grammar_hemp();
    hemp_mem_trace_ok();
    hemp_mem_trace_reset();

    return exit_status();
}


void test_grammar() {
    hemp_p hemp = hemp_init();
    ok( hemp, "created hemp object" );
    
    hemp_grammar_p grammar = hemp_grammar(hemp, HEMP_TT3);
    ok( grammar, "got grammar" );
    ok( hemp_cstr_eq(grammar->name, "tt3"), "grammar name is tt3" );

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
            grammar, "hemp.text", "_TEXT", 0, 0
        );
        hemp_grammar_add_symbol(
            grammar, "hemp.space", "_SPACE", 0, 0
        );
        hemp_grammar_add_symbol(
            grammar, "hemp.numop.plus", "+", 42, 420
        );
        hemp_grammar_add_symbol(
            grammar, "hemp.boolop.and", "and", 42, 420
        );
        hemp_grammar_add_symbol(
            grammar, "hemp.textop.concat", "~", 42, 420
        );
        hemp_grammar_add_symbol(
            grammar, "hemp.numop.dud", "--", 55, 99
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