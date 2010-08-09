#include <hemp.h>
#include <tap.h>


void test_value();

int
main(
    int  argc, 
    char **argv, 
    char **env)
{
    printf("%s %d bits %s endian\n", HEMP_ARCHITECTURE, HEMP_WORD_LENGTH, HEMP_ENDIAN);
    plan_tests(2);
    test_value();
    hemp_mem_trace_ok();
    return exit_status();
}


void type_check(
    hemp_value_t value,
    hemp_u8_t    id,
    hemp_cstr_p  name
) {
    if (hemp_cstr_eq(HEMP_TYPE_NAME(value), name)) {
        pass("value type is %s", name);
    }
    else {
        fail("values type is %s, expected %s", HEMP_TYPE_NAME(value), name);
    }
    
    ok( HEMP_TYPE_ID(value) == id, "%s type is %d", name, id);
}


void test_value() {
    pass("testing value");
    hemp_value_t value;
    hemp_vtype_p vtable;

    printf("NAN64:\n");
    hemp_dump_u64(HEMP_NAN_MASK);
    printf("NUM64:\n");
    hemp_dump_u64(HEMP_TYPE_NUM_MASK);
    printf("INT64:\n");
    hemp_dump_u64(HEMP_TYPE_INT_MASK);
    printf("STR64:\n");
    hemp_dump_u64(HEMP_TYPE_STR_MASK);

    /* native double */
    printf("FLOAT: 3.14159\n");
    hemp_num_t expect = -314.59;
    value = HEMP_NUM_VAL(expect);
    hemp_dump_value(value);

    hemp_num_t n = HEMP_VAL_NUM(value);
    ok( ! HEMP_IS_TAGGED(value), "number is not a tagged value" );
    ok( HEMP_IS_NUM(value), "number is a number value" );
    ok( n == expect, "got num value back" );
    type_check(value, HEMP_TYPE_NUM_ID, "number");

    /* tagged int */
    printf("INT: 12345\n");
    value = HEMP_INT_VAL(12345);
    hemp_dump_value(value);
    type_check(value, HEMP_TYPE_INT_ID, "integer");

    hemp_u32_t i = HEMP_VAL_INT(value);
    ok( HEMP_IS_TAGGED(value), "integer is a tagged value" );
    ok( HEMP_IS_INT(value), "integer is an integer value" );
    ok( i == 12345, "got int value back" );

    /* tagged string pointer */
    hemp_cstr_p es = "Hello world!";
    printf("STRING: %s\n", es);
    value = HEMP_STR_VAL(es);
    hemp_dump_value(value);
    type_check(value, HEMP_TYPE_STR_ID, "string");

    hemp_cstr_p s = HEMP_VAL_STR(value);
    printf("[%p]   [%p]\n", es, s);
    ok( HEMP_IS_TAGGED(value), "string is a tagged value" );
    ok( HEMP_IS_STR(value), "string is a string value" );
    is( s, es, "got str value back" );
}

