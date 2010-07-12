#include <stdio.h>
#include "hemp.h"
#include "tap.h"

void test_hemp();
void test_cstr_split();

int
main(int argc, char **argv, char **env)
{
    plan_tests(11);
    test_hemp();
    test_cstr_split();
    return exit_status();
}

void test_hemp() {
    hemp_t hemp = hemp_init();
    ok( hemp, "created hemp object" );
    hemp_free(hemp);
    ok( hemp, "released hemp object" );
}


void test_cstr_split() {
    hemp_list_t list = hemp_cstr_split("foo:bar", ":");
    ok( list, "split string into list" );
    ok( list->length == 2, "two items in list" );
    ok( hemp_cstr_eq(hemp_list_item(list, 0), "foo"), "first item is foo" );
    ok( hemp_cstr_eq(hemp_list_item(list, 1), "bar"), "second item is bar" );
    hemp_list_free(list);

    list = hemp_cstr_split("wam:AND:bam:AND:mam", ":AND:");
    ok( list, "split string into list with wide delimiter" );
    ok( list->length == 3, "three items in list" );
    ok( hemp_cstr_eq(hemp_list_item(list, 0), "wam"), "first item is wam" );
    ok( hemp_cstr_eq(hemp_list_item(list, 1), "bam"), "second item is bam" );
    ok( hemp_cstr_eq(hemp_list_item(list, 2), "mam"), "third item is bam" );
    hemp_list_free(list);
}
