#include <hemp/test.h>


void test_values();
void test_number(hemp_num_t);
void test_integer(hemp_int_t);
void test_identity_values();
void test_value_conversion();
void test_number_conversion();
void test_integer_conversion();
void test_boolean_conversion();
void test_text_conversion();
void test_identity_conversion();


int main(
    int argc, char **argv, char **env
) {
    plan(231);

    test_values();
    test_identity_values();
    test_value_conversion();

    return done();
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


void test_values() {
    pass("testing values");
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
}


void test_identity_values() {
    pass("testing identity values");

    printf("HempMissing:\n");
    hemp_dump_value(HempMissing);
    ok( HEMP_IDENT_VTABLE_NO(HempMissing) == 0, "missing uses ident vtable 0" );
    is(   hemp_type_class(HempMissing), "Undefined", "Missing type class is Undefined" );
    is(   hemp_type_name(HempMissing),  "Missing",   "Missing class name is Missing" );
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
    ok(   HEMP_IDENT_VTABLE_NO(HempNothing) == 0, "nothing uses ident vtable 0" );
    is(   hemp_type_class(HempNothing), "Undefined", "Nothing type class is Undefined" );
    is(   hemp_type_name(HempNothing),  "Nothing",   "Nothing type name is Nothing" );
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
    ok(   HEMP_IDENT_VTABLE_NO(HempFalse) == 1, "false uses ident vtable 1" );
    is(   hemp_type_class(HempFalse),   "Truth", "False type class is Truth" );
    is(   hemp_type_name(HempFalse),    "False", "False type name is False" );
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
    ok(   HEMP_IDENT_VTABLE_NO(HempTrue) == 1, "true uses ident vtable 1" );
    is(   hemp_type_class(HempTrue),    "Truth", "True type class is Truth" );
    is(   hemp_type_name(HempTrue),     "True",  "True type name is True" );
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
    ok(   HEMP_IDENT_VTABLE_NO(HempBefore) == 3, "before uses ident vtable 3" );
    is(   hemp_type_class(HempBefore),  "Compare", "Before type class is Compare" );
    is(   hemp_type_name(HempBefore),   "Before",  "Before type name is Before" );
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
    ok(   HEMP_IDENT_VTABLE_NO(HempEqual) == 3, "equal uses ident vtable 3" );
    is(   hemp_type_class(HempEqual),   "Compare", "Equal type class is Compare" );
    is(   hemp_type_name(HempEqual),    "Equal",   "Equal type name is Equal" );
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
    ok(   HEMP_IDENT_VTABLE_NO(HempAfter) == 3, "after uses ident vtable 3" );
    is(   hemp_type_class(HempAfter),   "Compare", "After type class is Compare" );
    is(   hemp_type_name(HempAfter),    "After",   "After type name is After" );
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



/*--------------------------------------------------------------------------
 * value conversion
 *--------------------------------------------------------------------------*/

void test_value_conversion() {
    test_number_conversion();
    test_integer_conversion();
    test_boolean_conversion();
    test_text_conversion();
    test_identity_conversion();
}


void test_number_conversion() {
    pass("testing number -> xxx conversions");
    hemp_value_t number = hemp_num_val(3.14159);

    /* number -> integer */
    hemp_value_t integer = hemp_value_number_integer(number);
    ok( hemp_is_integer(integer), "result is integer" );
    ok( hemp_val_int(integer) == 3, "integer value correct" );

    /* number -> boolean */
    hemp_value_t truth = hemp_value_number_boolean(number);
    ok( hemp_is_boolean(truth), "result is boolean" );
    ok( hemp_is_true(truth), "boolean value correct" );

    /* number -> text */
    hemp_value_t text = hemp_value_number_text(number, HempNothing);
    ok( hemp_is_text(text), "result is text" );
    is( hemp_val_text(text)->string, "3.14159", "text string correct" );
    hemp_text_free( hemp_val_text(text) );
}


void test_integer_conversion() {
    pass("testing integer -> xxx conversions");
    hemp_value_t integer = hemp_int_val(42);

    /* integer -> number */
    hemp_value_t number = hemp_value_integer_number(integer);
    ok( hemp_is_number(number), "result is number" );
    eq( hemp_val_num(number), 42.0, "number value correct" );
    eq( hemp_val_num( hemp_to_num(integer) ), 42.0, "hemp_to_num()" );

    /* integer -> boolean */
    hemp_value_t truth = hemp_value_integer_boolean(integer);
    ok( hemp_is_boolean(truth), "result is boolean" );
    ok( hemp_is_true(truth), "boolean value correct" );
    ok( hemp_is_true( hemp_to_boolean(integer) ), "hemp_to_boolean()" );

    /* integer -> text */
    hemp_value_t text = hemp_value_integer_text(integer, HempNothing);
    ok( hemp_is_text(text), "result is text" );
    is( hemp_val_text(text)->string, "42", "text string correct" );
    hemp_text_free( hemp_val_text(text) );
}


void test_boolean_conversion() {
    hemp_p hemp = hemp_init();
    hemp_value_t truth = HempTrue;;

    /* boolean -> number */
    HEMP_TRY;
        hemp_value_t number = hemp_value_boolean_number(truth);
        fail("boolean should not convert to number");
    HEMP_CATCH_ALL;
        ok( "caught error: %s", hemp->error->message );
    HEMP_END;
    
    /* boolean -> integer */
    HEMP_TRY;
        hemp_value_t integer = hemp_value_boolean_integer(truth);
        fail("boolean should not convert to integer");
    HEMP_CATCH_ALL;
        ok( "caught error: %s", hemp->error->message );
    HEMP_END;

    /* boolean -> text */
    hemp_value_t text = hemp_value_boolean_text(HempTrue, HempNothing);
    ok( hemp_is_text(text), "true text" );
    is( hemp_val_text(text)->string, "True", "true string correct" );
    hemp_text_free( hemp_val_text(text) );

    text = hemp_value_boolean_text(HempFalse, HempNothing);
    ok( hemp_is_text(text), "false text" );
    is( hemp_val_text(text)->string, "False", "false string correct" );
    hemp_text_free( hemp_val_text(text) );

    hemp_free(hemp);
}


void test_text_conversion_is_number(
    hemp_cstr_p string,
    hemp_num_t  expect
) {
    hemp_p       hemp = hemp_init();
    hemp_text_p  text = hemp_text_from_cstr(string);
    hemp_value_t tval = hemp_text_val(text);
    hemp_value_t nval;
    
    HEMP_TRY; 
        nval = hemp_value_text_number(tval);
        ok( 
            hemp_val_num(nval) == expect, 
            "text conversion to number: %s [%g] [%g]", 
            string, hemp_val_num(nval), expect
        ); 
    HEMP_CATCH_ALL;
        pass("text->number conversion error: %s : %s", string, hemp->error->message);
    HEMP_END;

    hemp_text_free(text);
    hemp_free(hemp);
}


void test_text_conversion_not_number(
    hemp_cstr_p string
) {
    hemp_p       hemp = hemp_init();
    hemp_text_p  text = hemp_text_from_cstr(string);
    hemp_value_t tval = hemp_text_val(text);
    hemp_value_t nval;
    
    HEMP_TRY; 
        nval = hemp_value_text_number(tval);
        fail("string should not convert to a number: %s", string);
    HEMP_CATCH_ALL;
        pass("caught conversion error [%s]->number: %s", string, hemp->error->message);
    HEMP_END;

    hemp_text_free(text);
    hemp_free(hemp);
}


void test_text_conversion_is_integer(
    hemp_cstr_p string,
    hemp_int_t  expect
) {
    hemp_p       hemp = hemp_init();
    hemp_text_p  text = hemp_text_from_cstr(string);
    hemp_value_t tval = hemp_text_val(text);
    hemp_value_t ival;
    
    HEMP_TRY; 
        ival = hemp_value_text_integer(tval);
        ok( 
            hemp_val_int(ival) == expect, 
            "text conversion to integer: %s [%d] [%d]", 
            string, hemp_val_int(ival), expect
        ); 
    HEMP_CATCH_ALL;
        pass("text->integer conversion error: %s : %s", string, hemp->error->message);
    HEMP_END;

    hemp_text_free(text);
    hemp_free(hemp);
}


void test_text_conversion_not_integer(
    hemp_cstr_p string
) {
    hemp_p       hemp = hemp_init();
    hemp_text_p  text = hemp_text_from_cstr(string);
    hemp_value_t tval = hemp_text_val(text);
    hemp_value_t ival;
    
    HEMP_TRY; 
        ival = hemp_value_text_integer(tval);
        fail("string should not convert to a integer: %s", string);
    HEMP_CATCH_ALL;
        pass("caught conversion error [%s]->integer: %s", string, hemp->error->message);
    HEMP_END;

    hemp_text_free(text);
    hemp_free(hemp);
}


void test_text_conversion() {
    pass("testing text -> xxx conversions");
    hemp_text_p  text = hemp_text_from_cstr("123");
    hemp_value_t tval = hemp_text_val(text);

    /* text -> integer */
    test_text_conversion_is_integer("12", 12);
    test_text_conversion_is_integer("034", 34);
    test_text_conversion_is_integer("123.45", 123);
    test_text_conversion_is_number("0x0f", 15);

    test_text_conversion_not_integer("non-numerical");
    test_text_conversion_not_integer("42 with trailing cruft");
    test_text_conversion_not_integer("leading cruft 69");
    test_text_conversion_not_integer("");

    /* text -> number */
    test_text_conversion_is_number("12", 12.0);
    test_text_conversion_is_number("034", 34.0);
    test_text_conversion_is_number("123.45", 123.45);
    test_text_conversion_is_number("0x0f", 15);
 
    test_text_conversion_not_number("non-numerical");
    test_text_conversion_not_number("42 with trailing cruft");
    test_text_conversion_not_number("leading cruft 69");
    test_text_conversion_not_number("");

    /* text -> boolean */
    hemp_value_t truth = hemp_value_text_boolean(tval);
    ok( hemp_is_boolean(truth), "result is boolean" );
    ok( hemp_is_true(truth), "boolean value correct" );

    hemp_text_replace_cstr(text, "0");
    ok( 
        hemp_is_true( hemp_value_text_boolean(tval) ),
        "'0' string is true" 
    );

    hemp_text_replace_cstr(text, "0.0");
    ok( 
        hemp_is_true( hemp_value_text_boolean(tval) ), 
        "'0.0' string is true" 
    );

    hemp_text_replace_cstr(text, "False");
    ok( 
        hemp_is_true( hemp_value_text_boolean(tval) ), 
        "'False' string is true" 
    );

    hemp_text_replace_cstr(text, "FALSE");
    ok( 
        hemp_is_true( hemp_value_text_boolean(tval) ),
        "'FALSE' string is true" 
    );

    hemp_text_replace_cstr(text, "");
    ok( 
        hemp_is_false( hemp_value_text_boolean(tval) ), 
        "empty string is false" 
    );
    
    hemp_text_free(text);
}


void test_identity_conversion() {
    pass("testing identity -> xxx conversions");

    /* identity -> number is not allowed */
    hemp_value_t number = hemp_value_identity_number(HempMissing);

    /* identity -> boolean is sometimes allowed, depending on value */
    hemp_value_t truth;
    truth = hemp_value_identity_boolean(HempMissing);
    truth = hemp_value_identity_boolean(HempTrue);
    truth = hemp_value_identity_boolean(HempBefore);

    /* identity -> text is always allowed */
    hemp_value_t text;

    text = hemp_value_identity_text(HempMissing, HempNothing);
    ok( hemp_is_text(text), "missing converts to text" );
    is( hemp_val_text(text)->string, "Missing", "Missing text" );
    hemp_text_free( hemp_val_text(text) );

    text = hemp_value_identity_text(HempNothing, HempNothing);
    ok( hemp_is_text(text), "nothing converts to text" );
    is( hemp_val_text(text)->string, "Nothing", "Nothing text" );
    hemp_text_free( hemp_val_text(text) );

    text = hemp_value_identity_text(HempTrue, HempNothing);
    ok( hemp_is_text(text), "true converts to text" );
    is( hemp_val_text(text)->string, "True", "True text" );
    hemp_text_free( hemp_val_text(text) );

    text = hemp_value_identity_text(HempBefore, HempNothing);
    ok( hemp_is_text(text), "before converts to text" );
    is( hemp_val_text(text)->string, "Before", "True text" );
    hemp_text_free( hemp_val_text(text) );

    // TODO
}


