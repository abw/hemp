#include <hemp/test.h>


void test_action();
int result;

int
main(
    int argc, char **argv, char **env
) {
    plan(4);
    test_action();
    return done();
}

HempMemory
add_actor(
    int n1,
    int n2
) {
    result = n1 + n2;
    return NULL;
}


HempMemory
printf_actor(
    char *format,
    char *string
) {
    printf(format, string);
    return NULL;
}


void test_action() {
    HempAction action = hemp_action_new(
        (HempActor) &printf_actor,
        "hello %s"
    );
    ok( action, "created 'hello world' action" );

    hemp_action_run(action, "world\n");
    hemp_action_free(action);

    action = hemp_action_new(
        (HempActor) &add_actor,
        (HempMemory) 300            /* HACK: cast to avoid warnings */
    );
    ok( action, "created a + b action" );

    hemp_action_run(action, 60);
    ok( result == 360, "300 + 60 = 360" );

    hemp_action_run(action, 120);
    ok( result == 420, "300 + 120 = 420" );

    hemp_action_free(action);
}
