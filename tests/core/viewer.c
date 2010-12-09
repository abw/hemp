#include <hemp/test.h>


void test_viewer();

int main(
    int argc, char **argv, char **env
) {
    plan(9);
    test_viewer();
    return done();
}



void test_viewer() {
    hemp_hemp hemp = hemp_new();
    ok( hemp, "created hemp object" );

    hemp_language_instance(hemp, "tt3");

    hemp_document document = hemp_document_instance(
        hemp, HEMP_TT3, HEMP_TEXT, "Hello [% name %]!"
    );
    ok( document , "created document" );
    
    hemp_context context = hemp_context_instance(hemp);
    ok( context, "created hemp context" );
    ok( hemp_context_set_string(context, "name", "World"), "set name" );

    hemp_viewer text = hemp_viewer_instance(hemp, "text");
    ok( text, "created text view" );
    is( text->name, "text", "name is set: text" );

    text = hemp_viewer_instance(hemp, "text");
    ok( text, "fetched text view again" );

    hemp_text output;

    HEMP_TRY;
        output = hemp_document_render(document, context);
        ok( output, "rendered document");
    HEMP_CATCH_ALL;
        output = hemp_error_text(hemp->error);
    HEMP_END;

    printf("** OUTPUT ** : %s\n", output->string);
    hemp_text_free(output);

    hemp_fragment root = hemp_document_tree(document);
    ok( root, "got root element: %p", root);

    hemp_value result = hemp_viewer_fragment(text, root, context, HempNothing);
    HEMP_UNUSED(result);

    hemp_context_free(context);
    hemp_free(hemp);
}


