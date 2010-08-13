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
    if (hemp_cstr_eq(hemp_type_name(value), name)) {
        pass("value type is %s", name);
    }
    else {
        fail("values type is %s, expected %s", hemp_type_name(value), name);
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
    ok( ! hemp_is_tagged(value), "number is not a tagged value" );
    ok( hemp_is_num(value), "number is a number value" );
    ok( n == expect, "got num value back" );
    type_check(value, HEMP_TYPE_NUM_ID, "Number");
    ok( ! hemp_is_undef(value),         "number is not undef" );
    ok( ! hemp_is_missing(value),       "number is not missing" );
    ok( ! hemp_is_nothing(value),       "number is not nothing" );
    ok( ! hemp_is_boolean(value),       "number is not boolean" );
    ok( ! hemp_is_true(value),          "number is not true" );
    ok( ! hemp_is_false(value),         "number is not false" );

    /* tagged int */
    printf("integer value: 12345\n");
    value = hemp_int_val(12345);
    hemp_dump_value(value);
    type_check(value, HEMP_TYPE_INT_ID, "Integer");

    hemp_u32_t i = hemp_val_int(value);
    ok( hemp_is_tagged(value), "integer is a tagged value" );
    ok( hemp_is_int(value), "integer is an integer value" );
    ok( i == 12345, "got int value back" );
    ok( ! hemp_is_undef(value),         "integer is not undef" );
    ok( ! hemp_is_missing(value),       "integer is not missing" );
    ok( ! hemp_is_nothing(value),       "integer is not nothing" );
    ok( ! hemp_is_boolean(value),       "integer is not boolean" );
    ok( ! hemp_is_true(value),          "integer is not true" );
    ok( ! hemp_is_false(value),         "integer is not false" );

    /* tagged string pointer */
    hemp_cstr_p es = "Hello world!";
    printf("string value: %s\n", es);
    value = hemp_str_val(es);
    hemp_dump_value(value);
    type_check(value, HEMP_TYPE_STR_ID, "String");
    ok( ! hemp_is_undef(value),         "string is not undef" );
    ok( ! hemp_is_missing(value),       "string is not missing" );
    ok( ! hemp_is_nothing(value),       "string is not nothing" );
    ok( ! hemp_is_boolean(value),       "string is not boolean" );
    ok( ! hemp_is_true(value),          "string is not true" );
    ok( ! hemp_is_false(value),         "string is not false" );

    hemp_cstr_p s = hemp_val_str(value);
    ok( hemp_is_tagged(value), "string is a tagged value" );
    ok( hemp_is_str(value), "string is a string value" );
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
    ok(   hemp_is_undef(HempMissing),   "missing is undef" );
    ok(   hemp_is_missing(HempMissing), "missing is missing" );
    ok( ! hemp_is_nothing(HempMissing), "missing is not nothing" );
    ok( ! hemp_is_boolean(HempMissing), "missing is not boolean" );
    ok( ! hemp_is_true(HempMissing),    "missing is not true" );
    ok( ! hemp_is_false(HempMissing),   "missing is not false" );

    printf("HempNothing:\n");
    hemp_dump_value(HempNothing);
    ok(   hemp_is_undef(HempNothing),   "nothing is undef" );
    ok( ! hemp_is_missing(HempNothing), "nothing is not missing" );
    ok(   hemp_is_nothing(HempNothing), "nothing is nothing" );
    ok( ! hemp_is_boolean(HempNothing), "nothing is not boolean" );
    ok( ! hemp_is_true(HempNothing),    "nothing is not true" );
    ok( ! hemp_is_false(HempNothing),   "nothing is not false" );

    printf("HempFalse:\n");
    hemp_dump_value(HempFalse);
    ok( ! hemp_is_undef(HempFalse),     "false is not undef" );
    ok( ! hemp_is_missing(HempFalse),   "false is not missing" );
    ok( ! hemp_is_nothing(HempFalse),   "false is not nothing" );
    ok(   hemp_is_boolean(HempFalse),   "false is boolean" );
    ok( ! hemp_is_true(HempFalse),      "false is not true" );
    ok(   hemp_is_false(HempFalse),     "false is false" );

    printf("HempTrue:\n");
    hemp_dump_value(HempTrue);
    ok( ! hemp_is_undef(HempTrue),      "true is not undef" );
    ok( ! hemp_is_missing(HempTrue),    "true is not missing" );
    ok( ! hemp_is_nothing(HempTrue),    "true is not nothing" );
    ok(   hemp_is_boolean(HempTrue),    "true is boolean" );
    ok(   hemp_is_true(HempTrue),       "true is true" );
    ok( ! hemp_is_false(HempTrue),      "true is not false" );

    printf("HempBefore:\n");
    hemp_dump_value(HempBefore);
    ok( ! hemp_is_boolean(HempBefore),  "before is not boolean" );
    ok(   hemp_is_before(HempBefore),   "before is before" );
    ok( ! hemp_is_equal(HempBefore),    "before is not equal" );
    ok( ! hemp_is_after(HempBefore),    "before is not after" );

    printf("HempEqual:\n");
    hemp_dump_value(HempEqual);
    ok( ! hemp_is_boolean(HempEqual),   "equal is not boolean" );
    ok( ! hemp_is_before(HempEqual),    "equal is not before" );
    ok(   hemp_is_equal(HempEqual),     "equal is equal" );
    ok( ! hemp_is_after(HempEqual),     "equal is not after" );

    printf("HempAfter:\n");
    hemp_dump_value(HempAfter);
    ok( ! hemp_is_boolean(HempAfter),   "after is not boolean" );
    ok( ! hemp_is_before(HempAfter),    "after is not before" );
    ok( ! hemp_is_equal(HempAfter),     "after is not equal" );
    ok(   hemp_is_after(HempAfter),     "after is after" );

}


