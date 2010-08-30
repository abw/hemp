#include <hemp/test.h>


int main(
    int argc, char **argv, char **env
) {
    hemp_p          hemp    = hemp_init();
    hemp_context_p  context = hemp_context(hemp);
    int             result;

    hemp_text_p message = hemp_text_from_string("Hello World!");
    
    hemp_context_set_number(  context, "e",         2.718    );
    hemp_context_set_number(  context, "pi",        3.14159  );
    hemp_context_set_integer( context, "answer",    42       );
    hemp_context_set_string(  context, "name",      "Badger" );
    hemp_context_set_text(    context, "message",   message );
    
    result = hemp_test_expect_script(
        HEMP_TT3, HEMP_TT3, 
        HEMP_TESTDIR, "words", 
        context
    );

    hemp_text_free(message);
    hemp_context_free(context);
    hemp_free(hemp);

    hemp_mem_trace_ok();

    return result;
}
