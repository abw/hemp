#include <stdio.h>
#include "hemp.h"
#include "tap.h"

void test_hub();


int
main(int argc, char **argv, char **env)
{
    plan_tests(4);
    test_hub();
    return exit_status();
}


void test_hub() {
    hemp_hub_t hub = hemp_hub_init();
    ok( hub, "created hub" );

    ok( hemp_hub_add_tag(hub, HempTagComment), "added comment tag" );
    
    hemp_tag_t tag = hemp_hub_tag(hub, HempTagComment->name);
    ok( tag, "fetched tag" );
    ok( hemp_string_eq(tag->name, HempTagComment->name), "tag name: %s", tag->name );

    ok( hemp_hub_add_element(hub, HempElementSpace), "added space element" );
    
    hemp_etype_t etype = hemp_hub_element(hub, HempElementSpace->name);
    ok( etype, "fetched element type" );
    ok( hemp_string_eq(etype->name, HempElementSpace->name), "element name: %s", etype->name );
    
    hemp_hub_free(hub);
}

