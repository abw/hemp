#include <hemp.h>
#include <tap.h>


void test_value();

int
main(
    int  argc, 
    char **argv, 
    char **env
) {
    plan_tests(73);
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

    printf("HEMP_NUM_MASK:\n");
    hemp_dump_u64(HEMP_TYPE_NUM_MASK);
    printf("HEMP_NAN_MASK:\n");
    hemp_dump_u64(HEMP_NAN_MASK);
    printf("HEMP_INT_MASK:\n");
    hemp_dump_u64(HEMP_TYPE_INT_MASK);
    printf("HEMP_STR_MASK:\n");
    hemp_dump_u64(HEMP_TYPE_STR_MASK);
    printf("------------------\n");

    /* native double */
    printf("number value: 3.14159\n");
    hemp_num_t expect = -314.59;
    value = hemp_num_val(expect);
    hemp_dump_value(value);

    hemp_num_t n = hemp_val_num(value);
    ok( ! HEMP_IS_TAGGED(value), "number is not a tagged value" );
    ok( HEMP_IS_NUM(value), "number is a number value" );
    ok( n == expect, "got num value back" );
    type_check(value, HEMP_TYPE_NUM_ID, "Number");
    ok( ! HEMP_IS_UNDEF(value),         "number is not undef" );
    ok( ! HEMP_IS_MISSING(value),       "number is not missing" );
    ok( ! HEMP_IS_NOTHING(value),       "number is not nothing" );
    ok( ! HEMP_IS_BOOLEAN(value),       "number is not boolean" );
    ok( ! HEMP_IS_TRUE(value),          "number is not true" );
    ok( ! HEMP_IS_FALSE(value),         "number is not false" );

    /* tagged int */
    printf("integer value: 12345\n");
    value = hemp_int_val(12345);
    hemp_dump_value(value);
    type_check(value, HEMP_TYPE_INT_ID, "Integer");

    hemp_u32_t i = hemp_val_int(value);
    ok( HEMP_IS_TAGGED(value), "integer is a tagged value" );
    ok( HEMP_IS_INT(value), "integer is an integer value" );
    ok( i == 12345, "got int value back" );
    ok( ! HEMP_IS_UNDEF(value),         "integer is not undef" );
    ok( ! HEMP_IS_MISSING(value),       "integer is not missing" );
    ok( ! HEMP_IS_NOTHING(value),       "integer is not nothing" );
    ok( ! HEMP_IS_BOOLEAN(value),       "integer is not boolean" );
    ok( ! HEMP_IS_TRUE(value),          "integer is not true" );
    ok( ! HEMP_IS_FALSE(value),         "integer is not false" );

    /* tagged string pointer */
    hemp_cstr_p es = "Hello world!";
    printf("string value: %s\n", es);
    value = hemp_str_val(es);
    hemp_dump_value(value);
    type_check(value, HEMP_TYPE_STR_ID, "String");
    ok( ! HEMP_IS_UNDEF(value),         "string is not undef" );
    ok( ! HEMP_IS_MISSING(value),       "string is not missing" );
    ok( ! HEMP_IS_NOTHING(value),       "string is not nothing" );
    ok( ! HEMP_IS_BOOLEAN(value),       "string is not boolean" );
    ok( ! HEMP_IS_TRUE(value),          "string is not true" );
    ok( ! HEMP_IS_FALSE(value),         "string is not false" );

    hemp_cstr_p s = hemp_val_str(value);
    ok( HEMP_IS_TAGGED(value), "string is a tagged value" );
    ok( HEMP_IS_STR(value), "string is a string value" );
    is( s, es, "got str value back" );

    /* boolean */
    printf("boolean value: %d\n", HEMP_TRUE);
    value = hemp_bool_val(HEMP_TRUE);
    hemp_dump_value(value);
    ok( value.bits == HempTrue.bits, "true identity" );
    hemp_bool_t b = hemp_val_bool(value);
    ok( b == HEMP_TRUE, "unboxed back to true" );


    printf("HempMissing:\n");
    hemp_dump_value(HempMissing);
    ok(   HEMP_IS_UNDEF(HempMissing),   "missing is undef" );
    ok(   HEMP_IS_MISSING(HempMissing), "missing is missing" );
    ok( ! HEMP_IS_NOTHING(HempMissing), "missing is not nothing" );
    ok( ! HEMP_IS_BOOLEAN(HempMissing), "missing is not boolean" );
    ok( ! HEMP_IS_TRUE(HempMissing),    "missing is not true" );
    ok( ! HEMP_IS_FALSE(HempMissing),   "missing is not false" );

    printf("HempNothing:\n");
    hemp_dump_value(HempNothing);
    ok(   HEMP_IS_UNDEF(HempNothing),   "nothing is undef" );
    ok( ! HEMP_IS_MISSING(HempNothing), "nothing is not missing" );
    ok(   HEMP_IS_NOTHING(HempNothing), "nothing is nothing" );
    ok( ! HEMP_IS_BOOLEAN(HempNothing), "nothing is not boolean" );
    ok( ! HEMP_IS_TRUE(HempNothing),    "nothing is not true" );
    ok( ! HEMP_IS_FALSE(HempNothing),   "nothing is not false" );

    printf("HempFalse:\n");
    hemp_dump_value(HempFalse);
    ok( ! HEMP_IS_UNDEF(HempFalse),     "false is not undef" );
    ok( ! HEMP_IS_MISSING(HempFalse),   "false is not missing" );
    ok( ! HEMP_IS_NOTHING(HempFalse),   "false is not nothing" );
    ok(   HEMP_IS_BOOLEAN(HempFalse),   "false is boolean" );
    ok( ! HEMP_IS_TRUE(HempFalse),      "false is not true" );
    ok(   HEMP_IS_FALSE(HempFalse),     "false is false" );

    printf("HempTrue:\n");
    hemp_dump_value(HempTrue);
    ok( ! HEMP_IS_UNDEF(HempTrue),      "true is not undef" );
    ok( ! HEMP_IS_MISSING(HempTrue),    "true is not missing" );
    ok( ! HEMP_IS_NOTHING(HempTrue),    "true is not nothing" );
    ok(   HEMP_IS_BOOLEAN(HempTrue),    "true is boolean" );
    ok(   HEMP_IS_TRUE(HempTrue),       "true is true" );
    ok( ! HEMP_IS_FALSE(HempTrue),      "true is not false" );

    printf("HempBefore:\n");
    hemp_dump_value(HempBefore);
    ok( ! HEMP_IS_BOOLEAN(HempBefore),  "before is not boolean" );
    ok(   HEMP_IS_BEFORE(HempBefore),   "before is before" );
    ok( ! HEMP_IS_EQUAL(HempBefore),    "before is not equal" );
    ok( ! HEMP_IS_AFTER(HempBefore),    "before is not after" );

    printf("HempEqual:\n");
    hemp_dump_value(HempEqual);
    ok( ! HEMP_IS_BOOLEAN(HempEqual),   "equal is not boolean" );
    ok( ! HEMP_IS_BEFORE(HempEqual),    "equal is not before" );
    ok(   HEMP_IS_EQUAL(HempEqual),     "equal is equal" );
    ok( ! HEMP_IS_AFTER(HempEqual),     "equal is not after" );

    printf("HempAfter:\n");
    hemp_dump_value(HempAfter);
    ok( ! HEMP_IS_BOOLEAN(HempAfter),   "after is not boolean" );
    ok( ! HEMP_IS_BEFORE(HempAfter),    "after is not before" );
    ok( ! HEMP_IS_EQUAL(HempAfter),     "after is not equal" );
    ok(   HEMP_IS_AFTER(HempAfter),     "after is after" );

}


