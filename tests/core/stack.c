#include <hemp/test.h>

void test_stack();
void test_stack_pop(HempStack, HempString);
void test_stack_top(HempStack, HempString);
void test_stack_item(HempString, HempString, HempString, HempString, HempString);


int main(
    int argc, char **argv, char **env
) {
    plan(36);
    test_stack();
    return done();
}


void test_stack() {
    HempStack stack = hemp_stack_new(4);
    ok( stack, "created stack" );

    /* push a couple of items */
    hemp_stack_push(stack, "one");
    test_stack_top(stack, "one");
    eq( stack->depth, 1, "one item on stack");

    hemp_stack_push(stack, "two");
    test_stack_top(stack, "two");
    eq( stack->depth, 2, "two items on stack");

    /* try popping and re-pushing */
    test_stack_pop(stack, "two");
    test_stack_top(stack, "one");
    eq( stack->depth, 1, "one item on stack again");

    hemp_stack_push(stack, "two");
    test_stack_top(stack, "two");
    eq( stack->depth, 2, "two items on stack again");

    /* push a couple more */
    hemp_stack_push(stack, "three");
    test_stack_top(stack, "three");
    eq( stack->depth, 3, "three items on stack");

    hemp_stack_push(stack, "four");
    test_stack_top(stack, "four");
    eq( stack->depth, 4, "four items on stack");

    /* at this point the stack should automatically grow */
    hemp_stack_push(stack, "five");
    test_stack_top(stack, "five");
    eq( stack->depth, 5, "five items on stack");

    hemp_stack_push(stack, "six");
    test_stack_top(stack, "six");
    eq( stack->depth, 6, "six items on stack");

    /* make sure we get everything back again */
    test_stack_pop(stack, "six");
    test_stack_top(stack, "five");
    eq( stack->depth, 5, "five items on stack");

    test_stack_pop(stack, "five");
    test_stack_top(stack, "four");
    eq( stack->depth, 4, "four items on stack");

    test_stack_pop(stack, "four");
    test_stack_top(stack, "three");
    eq( stack->depth, 3, "three items on stack");
    
    test_stack_pop(stack, "three");
    test_stack_top(stack, "two");
    eq( stack->depth, 2, "two items on stack");

    test_stack_pop(stack, "two");
    test_stack_top(stack, "one");
    eq( stack->depth, 1, "one items on stack");

    test_stack_pop(stack, "one");
    eq( stack->depth, 0, "nothing on stack");

    hemp_stack_free(stack);
    pass("released stack");
}


void test_stack_item(
    HempString item,
    HempString expect,
    HempString match,
    HempString mismatch,
    HempString nothing
) {
    if (item) {
        if (hemp_string_eq(item, expect)) {
            ok(match, item);
        }
        else {
            fail(mismatch, item, expect);
        }
    }
    else {
        fail(nothing, expect);
    }
}


void test_stack_pop(
    HempStack  stack,
    HempString expect
) {
    test_stack_item(
        hemp_stack_pop(stack),
        expect,
        "popped %s",
        "popped %s, expected %s",
        "expected to pop %s but got nothing"
    );
}


void test_stack_top(
    HempStack  stack,
    HempString expect
) {
    test_stack_item(
        hemp_stack_top(stack),
        expect,
        "top is %s",
        "top is %s, expected %s",
        "expected %s at top but got nothing"
    );
}


