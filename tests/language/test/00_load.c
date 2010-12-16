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
    hemp_hemp       hemp    = hemp_new();
    hemp_context    context = hemp_context_instance(hemp);
    hemp_string     name    = "test1";
    hemp_document   document;
    hemp_text       output;

    hemp_string  dir    = hemp_filesystem_join_path(HEMP_TEST_DIR, "scripts");
    hemp_string  file   = hemp_filesystem_join_path(dir, name);
    hemp_string  text   = hemp_filesystem_read_file(file);

    hemp_debug("text: %s\n", text);
    hemp_language_instance(hemp, "test");

    hemp_test_plan_p plan = hemp_test_plan(5);

    HEMP_TRY;
        document = hemp_document_instance(
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
        hemp_mem_trace_ok();

    HEMP_CATCH_ALL;
        fprintf(stderr, "Hemp error: %s", hemp->error->message);

    HEMP_END;

    return hemp_test_done(plan);
//    return 0;
}
