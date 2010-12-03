#include <hemp/test.h>

void test_unplugged();


int
main(
    int  argc, 
    char **argv, 
    char **env
) {
    plan(8);
    test_unplugged();
    return done();
}


void test_unplugged() {
    hemp_hemp       hemp = hemp_new();
    hemp_template   template;
    hemp_context    context;
    hemp_value      output;
    ok( hemp, "created hemp object" );

    HEMP_TRY;
        hemp_language_instance(hemp, "json");
        pass("got json langauge"); 

        template = hemp_template_instance(
            hemp, "json", HEMP_TEXT, "[10, 20, {w:10,x:null,y:false,z:true}]"
        );
        ok( template , "created template" );

        context = hemp_context_init(hemp);
        output  = hemp_template_data(template, context);

        ok( hemp_is_list(output), "got data list returned" );
        hemp_list list = hemp_val_list(output);
        eq( list->length, 3, "three items in json list" );
        
        hemp_value  item = hemp_list_item(list, 2);
        ok( hemp_is_hash(item), "third item is hash" );
        hemp_hash   hash = hemp_val_hash(item);

        eq( hash->size, 4, "four items in hash in json list" );
        hemp_value y = hemp_hash_fetch(hash, "y");
        ok( hemp_is_false(y), "y is false" );
    
        hemp_context_free(context);

    HEMP_CATCH_ALL;
        fail( "Caught error: %s", hemp->error->message );

    HEMP_END;

    hemp_free(hemp);
}


