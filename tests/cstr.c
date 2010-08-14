#include <hemp.h>
#include <tap.h>


void test_line();
void test_space();
void test_splits();
void test_words();
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
    plan_tests(48);

    test_line();
    hemp_mem_trace_ok();
    hemp_mem_trace_reset();

    test_space();
    hemp_mem_trace_ok();
    hemp_mem_trace_reset();

    test_splits();
    hemp_mem_trace_ok();
    hemp_mem_trace_reset();

    test_words();
    hemp_mem_trace_ok();
    hemp_mem_trace_reset();

    return exit_status();
}



void test_line() {
    hemp_cstr_p source = "Hello world\nnext line\nfoo";
    hemp_cstr_p search = source + 6;
    is( search, "world\nnext line\nfoo", "ready to look for next line" );

    ok( hemp_cstr_to_next_line(&search), "got second line" );
    is( search, "next line\nfoo", "at start of line" );

    ok( hemp_cstr_to_next_line(&search), "got third line" );
    is( search, "foo", "at start of third line" );
}


void test_space() {
    hemp_cstr_p paddy = hemp_cstr_copy("  foo bar baz  ");
    ok( paddy, "created padded string" );

    hemp_cstr_trim(paddy);
    is( paddy, "foo bar baz", "trimmed string" );

    hemp_cstr_p space = hemp_cstr_next_space(paddy);
    ok( space, "found first space" );
    is( space, " bar baz", "space after first word" );

    space = hemp_cstr_next_space(++space);
    ok( space, "found second space" );
    is( space, " baz", " space after second word" );

    space = hemp_cstr_next_space(++space);
    ok( ! space, "no more space" );

    space = paddy;
    ok( hemp_cstr_to_next_space(&space), "moved to first space" );
    is( space, " bar baz", "space after first word" );

    space++;
    ok( hemp_cstr_to_next_space(&space), "moved to second space" );
    is( space, " baz", "space after second word" );

    space++;
    ok( ! hemp_cstr_to_next_space(&space), "no more spaces" );

    hemp_mem_free(paddy);
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



void test_words() {
    hemp_cstr_p text = " foo bar  baz\n\t  bam  ";
    hemp_list_p list;
    
    list = hemp_cstr_words(text);
    ok( list, "got words" );
    ok( list->length == 4, "four items in list" );
    is( hemp_list_item(list,0), "foo", "first word is foo" );
    is( hemp_list_item(list,1), "bar", "second word is bar" );
    is( hemp_list_item(list,2), "baz", "third word is baz" );
    is( hemp_list_item(list,3), "bam", "fourth word is bam" );
    hemp_list_free(list);

    list = hemp_cstr_nwords(text, 2);
    ok( list, "got more words" );
    ok( list->length == 2, "two items in list" );
    is( hemp_list_item(list,0), "foo", "first word is foo" );
    is( hemp_list_item(list,1), "bar  baz\n\t  bam  ", "second word is bar..." );
    hemp_list_free(list);

}
