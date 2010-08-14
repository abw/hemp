#include <hemp.h>
#include <tap.h>


void test_value();
void test_value_conversion();
void test_identity_values();

int
main(
    int  argc, 
    char **argv, 
    char **env
) {
    plan_tests(185);

    test_value();
    hemp_mem_trace_ok();
    hemp_mem_trace_reset();
    
    test_value_conversion();
    hemp_mem_trace_ok();
    hemp_mem_trace_reset();

    test_identity_values();
    hemp_mem_trace_ok();
    hemp_mem_trace_reset();

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


void test_number(hemp_num_t expect) {
    printf("number value: %g\n", expect);
    hemp_value_t value = hemp_num_val(expect);
    hemp_dump_value(value);

    hemp_num_t n = hemp_val_num(value);
    ok( ! hemp_is_tagged(value), "number is not a tagged value" );
    ok( hemp_is_number(value), "number is a number value" );
    ok( n == expect, "got num value back" );

    type_check(value, HEMP_TYPE_NUMBER_ID, "Number");
    ok( ! hemp_is_undef(value),         "number is not undef" );
    ok( ! hemp_is_missing(value),       "number is not missing" );
    ok( ! hemp_is_nothing(value),       "number is not nothing" );
    ok( ! hemp_is_boolean(value),       "number is not boolean" );
    ok( ! hemp_is_compare(value),       "number is not compare" );
    ok( ! hemp_is_true(value),          "number is not true" );
    ok( ! hemp_is_false(value),         "number is not false" );
}


void test_integer(hemp_int_t expect) {
    /* tagged int */
    printf("integer value: %d\n", expect);
    hemp_value_t value = hemp_int_val(expect);
    hemp_dump_value(value);
    type_check(value, HEMP_TYPE_INTEGER_ID, "Integer");

    hemp_u32_t i = hemp_val_int(value);
    ok( hemp_is_tagged(value), "integer is a tagged value" );
    ok( hemp_is_integer(value), "integer is an integer value" );
    ok( i == expect, "got int value back" );

    ok( ! hemp_is_undef(value),         "integer is not undef" );
    ok( ! hemp_is_missing(value),       "integer is not missing" );
    ok( ! hemp_is_nothing(value),       "integer is not nothing" );
    ok( ! hemp_is_boolean(value),       "integer is not boolean" );
    ok( ! hemp_is_compare(value),       "integer is not compare" );
    ok( ! hemp_is_true(value),          "integer is not true" );
    ok( ! hemp_is_false(value),         "integer is not false" );
}


void test_value() {
    pass("testing value");
    hemp_value_t value;
    hemp_vtype_p vtable;

    printf("HEMP_NUMBER_MASK:\n");
    hemp_dump_u64(HEMP_TYPE_NUMBER_MASK);
    printf("HEMP_NAN_MASK:\n");
    hemp_dump_u64(HEMP_NAN_MASK);
    printf("HEMP_INTEGER_MASK:\n");
    hemp_dump_u64(HEMP_TYPE_INTEGER_MASK);
    printf("HEMP_STRING_MASK:\n");
    hemp_dump_u64(HEMP_TYPE_STRING_MASK);
    printf("------------------\n");

    /* native double */
    test_number(3000);
    test_number(3.14159);
    test_number(-42.43);

    test_integer(123);
    test_integer(456789);
    test_integer(-69);

    /* tagged string pointer */
    hemp_cstr_p es = "Hello world!";
    printf("string value: %s\n", es);
    value = hemp_str_val(es);
    hemp_dump_value(value);
    type_check(value, HEMP_TYPE_STRING_ID, "String");
    ok( ! hemp_is_undef(value),         "string is not undef" );
    ok( ! hemp_is_missing(value),       "string is not missing" );
    ok( ! hemp_is_nothing(value),       "string is not nothing" );
    ok( ! hemp_is_boolean(value),       "string is not boolean" );
    ok( ! hemp_is_compare(value),       "string is not compare" );
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
    ok( ! hemp_is_compare(HempMissing), "missing is not compare" );
    ok( ! hemp_is_true(HempMissing),    "missing is not true" );
    ok( ! hemp_is_false(HempMissing),   "missing is not false" );
    ok( ! hemp_is_equal(HempMissing),   "missing is not equal" );
    ok( ! hemp_is_before(HempMissing),  "missing is not before" );
    ok( ! hemp_is_after(HempMissing),   "missing is not after" );

    printf("HempNothing:\n");
    hemp_dump_value(HempNothing);
    ok(   hemp_is_undef(HempNothing),   "nothing is undef" );
    ok( ! hemp_is_missing(HempNothing), "nothing is not missing" );
    ok(   hemp_is_nothing(HempNothing), "nothing is nothing" );
    ok( ! hemp_is_boolean(HempNothing), "nothing is not boolean" );
    ok( ! hemp_is_compare(HempNothing), "nothing is not compare" );
    ok( ! hemp_is_true(HempNothing),    "nothing is not true" );
    ok( ! hemp_is_false(HempNothing),   "nothing is not false" );
    ok( ! hemp_is_equal(HempNothing),   "nothing is not equal" );
    ok( ! hemp_is_before(HempNothing),  "nothing is not before" );
    ok( ! hemp_is_after(HempNothing),   "nothing is not after" );

    printf("HempFalse:\n");
    hemp_dump_value(HempFalse);
    ok( ! hemp_is_undef(HempFalse),     "false is not undef" );
    ok( ! hemp_is_missing(HempFalse),   "false is not missing" );
    ok( ! hemp_is_nothing(HempFalse),   "false is not nothing" );
    ok(   hemp_is_boolean(HempFalse),   "false is boolean" );
    ok( ! hemp_is_compare(HempFalse),   "false is not compare" );
    ok( ! hemp_is_true(HempFalse),      "false is not true" );
    ok(   hemp_is_false(HempFalse),     "false is false" );
    ok( ! hemp_is_equal(HempFalse),     "false is not equal" );
    ok( ! hemp_is_before(HempFalse),    "false is not before" );
    ok( ! hemp_is_after(HempFalse),     "false is not after" );

    printf("HempTrue:\n");
    hemp_dump_value(HempTrue);
    ok( ! hemp_is_undef(HempTrue),      "true is not undef" );
    ok( ! hemp_is_missing(HempTrue),    "true is not missing" );
    ok( ! hemp_is_nothing(HempTrue),    "true is not nothing" );
    ok(   hemp_is_boolean(HempTrue),    "true is boolean" );
    ok( ! hemp_is_compare(HempTrue),    "true is not compare" );
    ok(   hemp_is_true(HempTrue),       "true is true" );
    ok( ! hemp_is_false(HempTrue),      "true is not false" );
    ok( ! hemp_is_equal(HempTrue),      "true is not equal" );
    ok( ! hemp_is_before(HempTrue),     "true is not after" );
    ok( ! hemp_is_after(HempTrue),      "true is not before" );

    printf("HempBefore:\n");
    hemp_dump_value(HempBefore);
    ok( ! hemp_is_undef(HempBefore),    "before is not undef" );
    ok( ! hemp_is_missing(HempBefore),  "before is not missing" );
    ok( ! hemp_is_nothing(HempBefore),  "before is not nothing" );
    ok(   hemp_is_boolean(HempBefore),  "before is boolean" );
    ok(   hemp_is_true(HempBefore),     "before is true" );
    ok( ! hemp_is_false(HempBefore),    "before is not false" );
    ok(   hemp_is_before(HempBefore),   "before is before" );
    ok( ! hemp_is_equal(HempBefore),    "before is not equal" );
    ok( ! hemp_is_after(HempBefore),    "before is not after" );

    printf("HempEqual:\n");
    hemp_dump_value(HempEqual);
    ok( ! hemp_is_undef(HempEqual),     "equal is not undef" );
    ok( ! hemp_is_missing(HempEqual),   "equal is not missing" );
    ok( ! hemp_is_nothing(HempEqual),   "equal is not nothing" );
    ok(   hemp_is_boolean(HempEqual),   "equal is boolean" );
    ok( ! hemp_is_true(HempEqual),      "equal is not true" );
    ok(   hemp_is_false(HempEqual),     "equal is false" );
    ok( ! hemp_is_before(HempEqual),    "equal is not before" );
    ok(   hemp_is_equal(HempEqual),     "equal is equal" );
    ok( ! hemp_is_after(HempEqual),     "equal is not after" );

    printf("HempAfter:\n");
    hemp_dump_value(HempAfter);
    ok( ! hemp_is_undef(HempAfter),     "after is not undef" );
    ok( ! hemp_is_missing(HempAfter),   "after is not missing" );
    ok( ! hemp_is_nothing(HempAfter),   "after is not nothing" );
    ok(   hemp_is_boolean(HempAfter),   "after is boolean" );
    ok(   hemp_is_true(HempAfter),      "after is true" );
    ok( ! hemp_is_false(HempAfter),     "after is not false" );
    ok( ! hemp_is_before(HempAfter),    "after is not before" );
    ok( ! hemp_is_equal(HempAfter),     "after is not equal" );
    ok(   hemp_is_after(HempAfter),     "after is after" );

}


void test_value_conversion() {
    hemp_value_t value  = hemp_int_val(42);
    hemp_value_f method = hemp_vmethod(value,number);
    ok( method, "got int->number method: %p", method);
    is( hemp_type_name(value), "Integer", "value is integer" );

    value = method(value);
    is( hemp_type_name(value), "Number", "value is now a number" );
    hemp_debug("number: %g\n", hemp_val_num(value));

    value = hemp_vcall(value, integer);
    is( hemp_type_name(value), "Integer", "back to an integer" );
    hemp_debug("integer: %d\n", hemp_val_int(value));

    value = hemp_to_text(value);
    is( hemp_type_name(value), "Text", "now text" );
    hemp_debug("text: %s\n", hemp_val_text(value)->string);

    hemp_value_t v2 = hemp_to_text(value);
    ok( hemp_val_text(v2) == hemp_val_text(value), "text to text no-op" );

    hemp_text_free(hemp_val_text(value));
}


void test_identity_values() {
    pass("testing identity value");

    hemp_dump_value(HempMissing);
    ok( HEMP_IDENT_VTABLE_NO(HempMissing) == 0, "missing uses ident vtable 0" );
    is( hemp_type_class(HempMissing), "Undefined", "Missing type class is Undefined" );
    is( hemp_type_name(HempMissing), "Missing", "Missing class name is Missing" );
    
    hemp_dump_value(HempNothing);
    ok( HEMP_IDENT_VTABLE_NO(HempNothing) == 0, "nothing uses ident vtable 0" );
    is( hemp_type_class(HempNothing), "Undefined", "Nothing type class is Undefined" );
    is( hemp_type_name(HempNothing), "Nothing", "Nothing type name is Nothing" );

    hemp_dump_value(HempFalse);
    ok( HEMP_IDENT_VTABLE_NO(HempFalse) == 1, "false uses ident vtable 1" );
    is( hemp_type_class(HempFalse), "Truth", "False type class is Truth" );
    is( hemp_type_name(HempFalse), "False", "False type name is False" );

    hemp_dump_value(HempTrue);
    ok( HEMP_IDENT_VTABLE_NO(HempTrue) == 1, "true uses ident vtable 1" );
    is( hemp_type_class(HempTrue), "Truth", "True type class is Truth" );
    is( hemp_type_name(HempTrue), "True", "True type name is Truth" );

    hemp_dump_value(HempBefore);
    ok( HEMP_IDENT_VTABLE_NO(HempBefore) == 3, "before uses ident vtable 3" );
    is( hemp_type_class(HempBefore), "Compare", "Before type class is Compare" );
    is( hemp_type_name(HempBefore), "Before", "Before type name is Before" );

    hemp_dump_value(HempEqual);
    ok( HEMP_IDENT_VTABLE_NO(HempEqual) == 3, "equal uses ident vtable 3" );
    is( hemp_type_class(HempEqual), "Compare", "Equal type class is Compare" );
    is( hemp_type_name(HempEqual), "Equal", "Equal type name is Equal" );

    hemp_dump_value(HempAfter);
    ok( HEMP_IDENT_VTABLE_NO(HempAfter) == 3, "after uses ident vtable 3" );
    is( hemp_type_class(HempAfter), "Compare", "After type class is Compare" );
    is( hemp_type_name(HempAfter), "After", "After type name is After" );
}


