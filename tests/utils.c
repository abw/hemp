#include <hemp.h>
#include <tap.h>


void test_md5();
void test_cstr();
void test_splits();
void split_ok(
    hemp_list_p list,
    hemp_pos_t  index,
    hemp_cstr_p left,
    hemp_cstr_p right
);


int
main(
    int  argc, 
    char **argv, 
    char **env
) {
    plan_tests(27);

    test_md5();
    hemp_mem_trace_ok();
    hemp_mem_trace_reset();

    test_cstr();
    hemp_mem_trace_ok();
    hemp_mem_trace_reset();

    test_splits();
    hemp_mem_trace_ok();
    hemp_mem_trace_reset();

    return exit_status();
}


void test_md5() {
    hemp_md5_t md5;

    hemp_md5_init(&md5);
    pass( "created md5 object" );

    hemp_md5_update(&md5, "hello ", 6);
    pass( "updated md5 object" );

    hemp_md5_update(&md5, "world", 5);
    pass( "updated md5 object again" );

    hemp_md5_final(&md5);
    pass( "finalised md5 object" );

    ok(
        hemp_cstr_eq(
            md5.output,
            "b746ca570487aa55ab3f1dff25d7949a"
        ),
        "MD5 [%s] matches", md5.output
    );
}


void test_cstr() {
    hemp_cstr_p source = "Hello world\nnext line";
    hemp_cstr_p search = source + 6;
    is( search, "world\nnext line", "ready to look for next line" );
    hemp_cstr_to_next_line(&search);
    is( search, "next line", "advanced to next line" );
}

void test_splits() {
    hemp_cstr_p text = "foo.bar.baz.bam";
    hemp_list_p list;

    /* test single character split token */
    list = hemp_cstr_splits(text, ".");
    ok( list, "got splits list for dot" );
    ok( list->length == 3, "list has 3 splits" );

    split_ok(list, 0, "foo", "bar.baz.bam");
    split_ok(list, 1, "foo.bar", "baz.bam");
    split_ok(list, 2, "foo.bar.baz", "bam");
    
    hemp_list_free(list);

    /* test multi character split token */
    list = hemp_cstr_splits(text, "ba");
    ok( list, "got list of split on ba" );
    ok( list->length == 3, "list has 3 splits" );
    
    split_ok(list, 0, "foo.", "r.baz.bam");
    split_ok(list, 1, "foo.bar.", "z.bam");
    split_ok(list, 2, "foo.bar.baz.", "m");
    hemp_list_free(list);

    /* test no split token */
    list = hemp_cstr_splits(text, "missing");
    ok( ! list, "no splits" );
}

void split_ok(
    hemp_list_p list,
    hemp_pos_t  index,
    hemp_cstr_p left,
    hemp_cstr_p right
) {
    hemp_cstr_split_p split = (hemp_cstr_split_p) hemp_list_item(list, index);
    if (split) {
        ok( hemp_cstr_eq(split->left,  left),  "left is [%s] expected [%s]",  split->left, left);
        ok( hemp_cstr_eq(split->right, right), "right is [%s] expected [%s]", split->right, right);
    }
    else {
        fail("no item %d in split list", index);
        fail("cannot check item %d left/right", index);
    }
}    