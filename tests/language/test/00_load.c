#include <hemp/test.h>


int test_language();

int main(
    int argc, char **argv, char **env
) {
//    plan(8);
    return test_language();
//    return done();
}


int test_language() {
    Hemp       hemp    = hemp_new();
    HempContext    context = hemp_context(hemp);
    HempString     name    = "test1";
    HempDocument   document;
    HempText       output;

    HempString  dir    = hemp_filesystem_join_path(HEMP_TEST_DIR, "scripts");
    HempString  file   = hemp_filesystem_join_path(dir, name);
    HempString  text   = hemp_filesystem_read_file(file);

//  hemp_debug_msg("HempGlobal is %p (%p)\n", &HempGlobal, HempGlobal.namespace);

//    hemp_debug("text: %s\n", text);

    hemp_language(hemp, "test");

    HempTestPlan plan = hemp_test_plan(5);

    HEMP_TRY;
        document = hemp_document(
            hemp, "test", HEMP_TEXT, text
        );
        output = hemp_document_render(document, context);
        ok( output, "%s rendered", name);
        fprintf(stderr, "OUTPUT:\n%s\n/OUTPUT\n", output->string);

        hemp_text_free(output);
        hemp_mem_free(text);
        hemp_mem_free(file);
        hemp_mem_free(dir);
        hemp_context_free(context);
        hemp_free(hemp);
//        hemp_mem_trace_ok();

    HEMP_CATCH_ALL;
        fprintf(stderr, "Hemp error: %s", hemp->error->message);

    HEMP_END;

    return hemp_test_all_done(plan);
}
