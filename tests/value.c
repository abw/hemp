#include <stdio.h>
#include "hemp.h"
#include "tap.h"


void test_value();
void test_hub();

int
main(int argc, char **argv, char **env)
{
    plan_tests(8);
    test_value();
    test_hub();
#if DEBUG & DEBUG_MEM
    hemp_mem_debug_report(HEMP_TRUE);
    hemp_mem_debug_cleanup();
#endif
    return exit_status();
}


void test_value() {
    hemp_vtypes_t vtypes = hemp_vtypes_init("test");
    ok( vtypes, "created vtypes" );
    ok( hemp_cstr_eq(vtypes->name, "test"), "vtypes name is %s", vtypes->name );
    
    hemp_vtype_t vtype = hemp_vtypes_new_type(vtypes, "foo");
    ok( vtype, "created vtype" );
    ok( hemp_cstr_eq(vtype->name, "foo"), "vtype name is %s", vtype->name );

    hemp_vtypes_free(vtypes);
}


void test_hub() {
    hemp_hub_t hub = hemp_hub_init();
    ok( hub, "created a hub" );
    
    hemp_hub_prepare_vtypes_hemp(hub);
    
    hemp_vtypes_t vtypes = hemp_hub_vtypes(hub, "hemp");
    ok( vtypes, "got %s value typeset", vtypes->name );

    hemp_vtype_t vtype = vtypes->text;
    ok( vtype, "got %s value type", vtype->name );
    ok( 
        hemp_cstr_eq(vtype->vtypes->name, "hemp"), 
        "vtype %s is a member of %s", vtype->name, vtype->vtypes->name
    );

    /* get hash type */
    hemp_vtype_t Hash = vtypes->hash;
    ok( Hash, "got %s value type", Hash->name );
    
    /* create a hash and store a text value */
    hemp_hash_t hash = hemp_hash_init();
    ok( hash, "created hash array" );
    
    hemp_text_t text = hemp_text_from_cstr("hello world");
    ok( text, "created message text" );
    
    hemp_hash_store(hash, "message", text);

    hemp_value_t hval = hemp_value_init(Hash, "ROOT", (hemp_data_t) hash, NULL);
    ok( hval, "created hash value" );
    
    hemp_context_t context = hemp_context_init();
    ok( context, "created context" );
    
    hemp_value_t tval = hval->vtype->dot(hval, context, "message");
    ok( tval, "got text value" );

    hemp_context_free(context);
    hemp_value_free(hval);
    hemp_text_free(text);
    hemp_hash_free(hash);
    hemp_hub_free(hub);
}
