#include <hemp/test.h>


void test_view();

int main(
    int argc, char **argv, char **env
) {
    plan(15);
    test_view();
    return done();
}



void test_view() {
    hemp_p hemp = hemp_init();
    ok( hemp, "created hemp object" );

    hemp_language(hemp, "tt3");

    hemp_template_p tmpl = hemp_template(
        hemp, HEMP_TT3, HEMP_TEXT, "Hello [% name %]!"
    );
    ok( tmpl , "created template" );
    
    hemp_context_p context = hemp_context(hemp);
    ok( context, "created hemp context" );
    ok( hemp_context_set_string(context, "name", "World"), "set name" );

    hemp_view_p text = hemp_view(hemp, "text");
    ok( text, "created text view" );
    is( text->name, "text", "name is set: text" );

    text = hemp_view(hemp, "text");
    ok( text, "fetched text view again" );

//    hemp_view_free(text);

    hemp_text_p output;

    HEMP_TRY;
        output = hemp_template_render(tmpl, context);
        ok( output, "rendered template");
    HEMP_CATCH_ALL;
        output = hemp_error_text(hemp->error);
    HEMP_END;

    printf("** OUTPUT ** : %s\n", output->string);
    hemp_text_free(output);

    hemp_element_p root = hemp_template_tree(tmpl);
    ok( root, "got root element: %p", root);

    hemp_value_t result = hemp_view_element(text, root, context, HempNothing);

    hemp_context_free(context);
    hemp_free(hemp);
}


