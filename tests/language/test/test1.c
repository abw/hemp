#include <hemp/test.h>


int main(
    int argc, char **argv, char **env
) {
    hemp_p          hemp    = hemp_init();
    hemp_cntx_p     context = hemp_context(hemp);
    hemp_str_p      name    = "test1";
    hemp_template_p tmpl;
    hemp_text_p     output;

    hemp_str_p  dir    = hemp_filesystem_join_path(HEMP_TESTDIR, "scripts");
    hemp_str_p  file   = hemp_filesystem_join_path(dir, name);
    hemp_str_p  text   = hemp_filesystem_read_file(file);

    hemp_debug("text: %s\n", text);
    hemp_language(hemp, HEMP_TEST);

    hemp_test_plan(5);

    HEMP_TRY;
        tmpl = hemp_template(
            hemp,
            HEMP_TEST,
            HEMP_TEXT, 
            text
        );
        output = hemp_template_render(tmpl, context);
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

    return 0;
}
