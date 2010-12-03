#include <hemp/test.h>
#include <math.h>

void test_values();
void test_number(hemp_num);
void test_integer(hemp_int);
void test_identity_values();
void test_value_conversion();
void test_number_conversion();
void test_integer_conversion();
//void test_boolean_conversion();
void test_text_conversion();
void test_identity_conversion();


int main(
    int argc, char **argv, char **env
) {
    plan(263);
    
    hemp_hemp hemp = hemp_new();
    
    test_values();
    test_identity_values();
    test_value_conversion();
    
    hemp_free(hemp);

    return done();
}


void type_check(
    hemp_value value,
    hemp_u8    id,
    hemp_string   name
) {
    if (hemp_string_eq(hemp_type_name(value), name)) {
        pass("value type is %s", name);
    }
    else {
        fail("values type is %s, expected %s", hemp_type_name(value), name);
    }
    
    ok( HEMP_TYPE_ID(value) == id, "%s type is %d", name, id);
}


void test_number(hemp_num expect) {
    printf("number value: %g\n", expect);
    hemp_value value = hemp_num_val(expect);
    hemp_dump_value(value);

    hemp_num n = hemp_val_num(value);
    ok( ! hemp_is_tagged(value), "number is not a tagged value" );
    ok( hemp_is_number(value), "number is a number value" );
    ok( n == expect, "got num value back" );

    type_check(value, HEMP_NUMBER_ID, "Number");
    ok( ! hemp_is_undef(value),         "number is not undef" );
    ok( ! hemp_is_missing(value),       "number is not missing" );
    ok( ! hemp_is_nothing(value),       "number is not nothing" );
    ok( ! hemp_is_boolean(value),       "number is not boolean" );
    ok( ! hemp_is_compare(value),       "number is not compare" );
    ok( ! hemp_is_true(value),          "number is not true" );
    ok( ! hemp_is_false(value),         "number is not false" );
}


void test_integer(hemp_int expect) {
    /* tagged int */
    printf("integer value: %d\n", expect);
    hemp_value value = hemp_int_val(expect);
    hemp_dump_value(value);
    type_check(value, HEMP_INTEGER_ID, "Integer");

    hemp_u32 i = hemp_val_int(value);
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
    hemp_value value;

    printf("HEMP_NAN:\n");
    hemp_dump_u64(HEMP_NAN);
    printf("HEMP_INTEGER_TAG:\n");
    hemp_dump_u64(HEMP_INTEGER_TAG);
    printf("HEMP_POINTER_TAG:\n");
    hemp_dump_u64(HEMP_POINTER_TAG);
    printf("------------------\n");

    /* native double */
    test_number(3000);
    test_number(3.14159);
    test_number(-42.43);

    test_integer(123);
    test_integer(456789);
    test_integer(-69);

    /* tagged string pointer */
    hemp_string es = "Hello world!";
    printf("string value: %s\n", es);
    value = hemp_str_val(es);
    hemp_dump_value(value);
    type_check(value, HEMP_STRING_ID, "String");
    ok( ! hemp_is_undef(value),         "string is not undef" );
    ok( ! hemp_is_missing(value),       "string is not missing" );
    ok( ! hemp_is_nothing(value),       "string is not nothing" );
    ok( ! hemp_is_boolean(value),       "string is not boolean" );
    ok( ! hemp_is_compare(value),       "string is not compare" );
    ok( ! hemp_is_true(value),          "string is not true" );
    ok( ! hemp_is_false(value),         "string is not false" );

    hemp_string s = hemp_val_str(value);
    ok( hemp_is_tagged(value), "string is a tagged value" );
    ok( hemp_is_string(value), "string is a string value" );
    ok( ! hemp_is_pointer(value), "string is not a pointer (well, not a raw pointer)" );
    is( s, es, "got str value back" );

    /* boolean */
    printf("boolean value: %d\n", HEMP_TRUE);
    value = hemp_bool_val(HEMP_TRUE);
    hemp_dump_value(value);
    ok( value.bits == HempTrue.bits, "true identity" );
    hemp_bool b = hemp_val_bool(value);
    ok( b == HEMP_TRUE, "unboxed back to true" );
}


void test_identity_values() {
    pass("testing identity values");

    printf("HempMissing:\n");
    hemp_dump_value(HempMissing);
//    ok( HEMP_IDENT_VTABLE_NO(HempMissing) == 0, "missing uses ident vtable 0" );
    is(   hemp_type_name(HempMissing),      "Identity", "Missing type name is Identity" );
    is(   hemp_identity_name(HempMissing),  "Missing", "Missing ident name is Missing" );
    ok(   hemp_is_undef(HempMissing),       "missing is undef" );
    ok(   hemp_is_missing(HempMissing),     "missing is missing" );
    ok( ! hemp_is_nothing(HempMissing),     "missing is not nothing" );
    ok(   hemp_is_boolean(HempMissing),     "missing is boolean" );
    ok( ! hemp_is_compare(HempMissing),     "missing is not compare" );
    ok( ! hemp_is_true(HempMissing),        "missing is not true" );
    ok(   hemp_is_false(HempMissing),       "missing is false" );
    ok( ! hemp_is_equal(HempMissing),       "missing is not equal" );
    ok( ! hemp_is_before(HempMissing),      "missing is not before" );
    ok( ! hemp_is_after(HempMissing),       "missing is not after" );
    
    printf("HempNothing:\n");
    hemp_dump_value(HempNothing);
//    ok(   HEMP_IDENT_VTABLE_NO(HempNothing) == 0, "nothing uses ident vtable 0" );
    is(   hemp_type_name(HempNothing),      "Identity", "Nothing type name is Identity" );
    is(   hemp_identity_name(HempNothing),  "Nothing",  "Nothing ident name is Nothing" );
    ok(   hemp_is_undef(HempNothing),       "nothing is undef" );
    ok( ! hemp_is_missing(HempNothing),     "nothing is not missing" );
    ok(   hemp_is_nothing(HempNothing),     "nothing is nothing" );
    ok(   hemp_is_boolean(HempNothing),     "nothing is boolean" );
    ok( ! hemp_is_compare(HempNothing),     "nothing is not compare" );
    ok( ! hemp_is_true(HempNothing),        "nothing is not true" );
    ok(   hemp_is_false(HempNothing),       "nothing is false" );
    ok( ! hemp_is_equal(HempNothing),       "nothing is not equal" );
    ok( ! hemp_is_before(HempNothing),      "nothing is not before" );
    ok( ! hemp_is_after(HempNothing),       "nothing is not after" );

    printf("HempEmpty:\n");
    hemp_dump_value(HempEmpty);
    is(   hemp_type_name(HempEmpty),      "Identity", "Empty type name is Identity" );
    is(   hemp_identity_name(HempEmpty),  "Empty",  "Empty ident name is Empty" );
    ok( ! hemp_is_undef(HempEmpty),       "empty is not undef" );
    ok( ! hemp_is_missing(HempEmpty),     "empty is not missing" );
    ok( ! hemp_is_nothing(HempEmpty),     "empty is not nothing" );
    ok(   hemp_is_empty(HempEmpty),       "empty is empty" );
    ok(   hemp_is_boolean(HempEmpty),     "empty is boolean" );
    ok( ! hemp_is_compare(HempEmpty),     "empty is not compare" );
    ok( ! hemp_is_true(HempEmpty),        "empty is not true" );
    ok(   hemp_is_false(HempEmpty),       "empty is false" );
    ok( ! hemp_is_equal(HempEmpty),       "empty is not equal" );
    ok( ! hemp_is_before(HempEmpty),      "empty is not before" );
    ok( ! hemp_is_after(HempEmpty),       "empty is not after" );

    printf("HempFalse:\n");
    hemp_dump_value(HempFalse);
//    ok(   HEMP_IDENT_VTABLE_NO(HempFalse) == 1, "false uses ident vtable 1" );
    is(   hemp_type_name(HempFalse),        "Identity", "False type name is Identity" );
    is(   hemp_identity_name(HempFalse),    "False", "False ident name is False" );
    ok( ! hemp_is_undef(HempFalse),         "false is not undef" );
    ok( ! hemp_is_missing(HempFalse),       "false is not missing" );
    ok( ! hemp_is_nothing(HempFalse),       "false is not nothing" );
    ok(   hemp_is_boolean(HempFalse),       "false is boolean" );
    ok( ! hemp_is_compare(HempFalse),       "false is not compare" );
    ok( ! hemp_is_true(HempFalse),          "false is not true" );
    ok(   hemp_is_false(HempFalse),         "false is false" );
    ok( ! hemp_is_equal(HempFalse),         "false is not equal" );
    ok( ! hemp_is_before(HempFalse),        "false is not before" );
    ok( ! hemp_is_after(HempFalse),         "false is not after" );

    printf("HempTrue:\n");
    hemp_dump_value(HempTrue);
//    ok(   HEMP_IDENT_VTABLE_NO(HempTrue) == 1, "true uses ident vtable 1" );
    is(   hemp_type_name(HempTrue),         "Identity", "True type name is Identity" );
    is(   hemp_identity_name(HempTrue),     "True",  "True ident name is True" );
    ok( ! hemp_is_undef(HempTrue),          "true is not undef" );
    ok( ! hemp_is_missing(HempTrue),        "true is not missing" );
    ok( ! hemp_is_nothing(HempTrue),        "true is not nothing" );
    ok(   hemp_is_boolean(HempTrue),        "true is boolean" );
    ok( ! hemp_is_compare(HempTrue),        "true is not compare" );
    ok(   hemp_is_true(HempTrue),           "true is true" );
    ok( ! hemp_is_false(HempTrue),          "true is not false" );
    ok( ! hemp_is_equal(HempTrue),          "true is not equal" );
    ok( ! hemp_is_before(HempTrue),         "true is not after" );
    ok( ! hemp_is_after(HempTrue),          "true is not before" );

    printf("HempBefore:\n");
    hemp_dump_value(HempBefore);
//    ok(   HEMP_IDENT_VTABLE_NO(HempBefore) == 3, "before uses ident vtable 3" );
    is(   hemp_type_name(HempBefore),       "Identity", "Before type name is Identity" );
    is(   hemp_identity_name(HempBefore),   "Before",  "Before ident name is Before" );
    ok( ! hemp_is_undef(HempBefore),        "before is not undef" );
    ok( ! hemp_is_missing(HempBefore),      "before is not missing" );
    ok( ! hemp_is_nothing(HempBefore),      "before is not nothing" );
    ok(   hemp_is_boolean(HempBefore),      "before is boolean" );
    ok(   hemp_is_true(HempBefore),         "before is true" );
    ok( ! hemp_is_false(HempBefore),        "before is not false" );
    ok(   hemp_is_before(HempBefore),       "before is before" );
    ok( ! hemp_is_equal(HempBefore),        "before is not equal" );
    ok( ! hemp_is_after(HempBefore),        "before is not after" );

    printf("HempEqual:\n");
    hemp_dump_value(HempEqual);
//    ok(   HEMP_IDENT_VTABLE_NO(HempEqual) == 3, "equal uses ident vtable 3" );
    is(   hemp_type_name(HempEqual),        "Identity", "Equal type name is Identity" );
    is(   hemp_identity_name(HempEqual),    "Equal",   "Equal ident name is Equal" );
    ok( ! hemp_is_undef(HempEqual),         "equal is not undef" );
    ok( ! hemp_is_missing(HempEqual),       "equal is not missing" );
    ok( ! hemp_is_nothing(HempEqual),       "equal is not nothing" );
    ok(   hemp_is_boolean(HempEqual),       "equal is boolean" );
    ok( ! hemp_is_true(HempEqual),          "equal is not true" );
    ok(   hemp_is_false(HempEqual),         "equal is false" );
    ok( ! hemp_is_before(HempEqual),        "equal is not before" );
    ok(   hemp_is_equal(HempEqual),         "equal is equal" );
    ok( ! hemp_is_after(HempEqual),         "equal is not after" );

    printf("HempAfter:\n");
    hemp_dump_value(HempAfter);
//    ok(   HEMP_IDENT_VTABLE_NO(HempAfter) == 3, "after uses ident vtable 3" );
    is(   hemp_type_name(HempAfter),        "Identity", "After type name is Identity" );
    is(   hemp_identity_name(HempAfter),    "After",   "After ident name is After" );
    ok( ! hemp_is_undef(HempAfter),         "after is not undef" );
    ok( ! hemp_is_missing(HempAfter),       "after is not missing" );
    ok( ! hemp_is_nothing(HempAfter),       "after is not nothing" );
    ok(   hemp_is_boolean(HempAfter),       "after is boolean" );
    ok(   hemp_is_true(HempAfter),          "after is true" );
    ok( ! hemp_is_false(HempAfter),         "after is not false" );
    ok( ! hemp_is_before(HempAfter),        "after is not before" );
    ok( ! hemp_is_equal(HempAfter),         "after is not equal" );
    ok(   hemp_is_after(HempAfter),         "after is after" );


    /* reassure myself that 0.0 (all zero bits) doesn't trigger any weird behaviour */
    printf("Zero:\n");
    hemp_value zero = hemp_num_val(0.0);
    hemp_dump_value(zero);
    ok( ! hemp_is_undef(zero),          "zero is not undef" );
    ok( ! hemp_is_missing(zero),        "zero is not missing" );
    ok( ! hemp_is_nothing(zero),        "zero is not nothing" );
    ok( ! hemp_is_boolean(zero),        "zero is not boolean" );
    ok( ! hemp_is_compare(zero),        "zero is not compare" );
    ok( ! hemp_is_true(zero),           "zero is not true" );
    ok( ! hemp_is_false(zero),          "zero is not false" );
    ok( ! hemp_is_equal(zero),          "zero is not equal" );
    ok( ! hemp_is_before(zero),         "zero is not before" );
    ok( ! hemp_is_after(zero),          "zero is not after" );

    /* same for  Infinity */
    printf("Infinity:\n");
    hemp_value infinity = hemp_num_val(INFINITY);
    hemp_dump_value(infinity);
    ok( ! hemp_is_undef(infinity),      "infinity is not undef" );
    ok( ! hemp_is_missing(infinity),    "infinity is not missing" );
    ok( ! hemp_is_nothing(infinity),    "infinity is not nothing" );
    ok( ! hemp_is_boolean(infinity),    "infinity is not boolean" );
    ok( ! hemp_is_compare(infinity),    "infinity is not compare" );
    ok( ! hemp_is_true(infinity),       "infinity is not true" );
    ok( ! hemp_is_false(infinity),      "infinity is not false" );
    ok( ! hemp_is_equal(infinity),      "infinity is not equal" );
    ok( ! hemp_is_before(infinity),     "infinity is not before" );
    ok( ! hemp_is_after(infinity),      "nan is not after" );

    /* same for  Nan */
    printf("NaN:\n");
    hemp_value nan = hemp_num_val(NAN);
    hemp_dump_value(nan);
    ok( ! hemp_is_undef(nan),           "nan is not undef" );
    ok( ! hemp_is_missing(nan),         "nan is not missing" );
    ok( ! hemp_is_nothing(nan),         "nan is not nothing" );
    ok( ! hemp_is_boolean(nan),         "nan is not boolean" );
    ok( ! hemp_is_compare(nan),         "nan is not compare" );
    ok( ! hemp_is_true(nan),            "nan is not true" );
    ok( ! hemp_is_false(nan),           "nan is not false" );
    ok( ! hemp_is_equal(nan),           "nan is not equal" );
    ok( ! hemp_is_before(nan),          "nan is not before" );
    ok( ! hemp_is_after(nan),           "nan is not after" );
}



/*--------------------------------------------------------------------------
 * value conversion
 *--------------------------------------------------------------------------*/

void test_value_conversion() {
    test_number_conversion();
    test_integer_conversion();
//    test_boolean_conversion();

    test_text_conversion();
    test_identity_conversion();
}


void test_number_conversion() {
    hemp_hemp          hemp    = hemp_new();
    hemp_context  context = hemp_context_instance(hemp);
    
    pass("testing number -> xxx conversions");
    hemp_value number = hemp_num_val(3.14159);

    /* number -> integer */
    hemp_value integer = hemp_type_number_integer(number, context);
    ok( hemp_is_integer(integer), "result is integer" );
    ok( hemp_val_int(integer) == 3, "integer value correct" );

    /* number -> boolean */
    hemp_value truth = hemp_type_number_boolean(number, context);
    ok( hemp_is_boolean(truth), "result is boolean" );
    ok( hemp_is_true(truth), "boolean value correct" );

    /* number -> text */
    hemp_value text = hemp_type_number_text(number, context, HempNothing);
    ok( hemp_is_text(text), "result is text" );
    is( hemp_val_text(text)->string, "3.14159", "text string correct" );

// Text now managed by context
//    hemp_text_free( hemp_val_text(text) );

    hemp_context_free(context);
    hemp_free(hemp);
}


void test_integer_conversion() {
    hemp_hemp          hemp    = hemp_new();
    hemp_context  context = hemp_context_instance(hemp);

    pass("testing integer -> xxx conversions");
    hemp_value integer = hemp_int_val(42);

    /* integer -> number */
    hemp_value number = hemp_type_integer_number(integer, context);
    ok( hemp_is_number(number), "result is number" );
    eq( hemp_val_num(number), 42.0, "number value correct" );
    eq( hemp_val_num( hemp_to_number(integer, context) ), 42.0, "hemp_to_number()" );

    /* integer -> boolean */
    hemp_value truth = hemp_type_integer_boolean(integer, context);
    ok( hemp_is_boolean(truth), "result is boolean" );
    ok( hemp_is_true(truth), "boolean value correct" );
    ok( hemp_is_true( hemp_to_boolean(integer, context) ), "hemp_to_boolean()" );

    /* integer -> text */
    hemp_value text = hemp_type_integer_text(integer, context, HempNothing);
    ok( hemp_is_text(text), "result is text" );
    is( hemp_val_text(text)->string, "42", "text string correct" );

// Text now managed by context
//    hemp_text_free( hemp_val_text(text) );

    hemp_context_free(context);
    hemp_free(hemp);
}


//void test_boolean_conversion() {
//    hemp_hemp          hemp    = hemp_new();
//    hemp_context  context = hemp_context_instance(hemp);
//    hemp_value    truth   = HempTrue;;
//
//    /* boolean -> number */
//    HEMP_TRY;
//        hemp_value number = hemp_value_boolean_number(truth, context);
//        fail("boolean should not convert to number");
//    HEMP_CATCH_ALL;
//        ok( hemp->error, "caught error: %s", hemp->error->message );
//    HEMP_END;
//    
//    /* boolean -> integer */
//    HEMP_TRY;
//        hemp_value integer = hemp_value_boolean_integer(truth, context);
//        fail("boolean should not convert to integer");
//    HEMP_CATCH_ALL;
//        ok( hemp->error, "caught error: %s", hemp->error->message );
//    HEMP_END;
//
//    /* boolean -> text */
//    hemp_value text = hemp_value_boolean_text(HempTrue, context, HempNothing);
//    ok( hemp_is_text(text), "true text" );
//    is( hemp_val_text(text)->string, "True", "true string correct" );
//    hemp_text_free( hemp_val_text(text) );
//
//    text = hemp_value_boolean_text(HempFalse, context, HempNothing);
//    ok( hemp_is_text(text), "false text" );
//    is( hemp_val_text(text)->string, "False", "false string correct" );
//    hemp_text_free( hemp_val_text(text) );
//
//    hemp_context_free(context);
//    hemp_free(hemp);
//}


void test_text_conversion_is_number(
    hemp_string string,
    hemp_num expect
) {
    hemp_hemp          hemp    = hemp_new();
    hemp_context  context = hemp_context_instance(hemp);
    hemp_text     text    = hemp_text_from_string(string);
    hemp_value    tval    = hemp_text_val(text);
    hemp_value    nval;
    
    HEMP_TRY; 
        nval = hemp_type_text_number(tval, context);
        ok( 
            hemp_val_num(nval) == expect, 
            "text conversion to number: %s [%g] [%g]", 
            string, hemp_val_num(nval), expect
        ); 
    HEMP_CATCH_ALL;
        pass("text->number conversion error: %s : %s", string, hemp->error->message);
    HEMP_END;

    hemp_text_free(text);
    hemp_context_free(context);
    hemp_free(hemp);
}


void test_text_conversion_not_number(
    hemp_string string
) {
    hemp_hemp          hemp    = hemp_new();
    hemp_context  context = hemp_context_instance(hemp);
    hemp_text     text    = hemp_text_from_string(string);
    hemp_value    tval    = hemp_text_val(text);
    hemp_value    nval;
    
    HEMP_TRY; 
        nval = hemp_type_text_number(tval, context);
        fail("string should not convert to a number:\n  %s", string);
    HEMP_CATCH_ALL;
        pass("caught error [%s]->number: %s", string, hemp->error->message);
    HEMP_END;

    hemp_text_free(text);
    hemp_context_free(context);
    hemp_free(hemp);
}


void test_text_conversion_is_integer(
    hemp_string string,
    hemp_int expect
) {
    hemp_hemp          hemp    = hemp_new();
    hemp_context  context = hemp_context_instance(hemp);
    hemp_text     text    = hemp_text_from_string(string);
    hemp_value    tval    = hemp_text_val(text);
    hemp_value    ival;
    
    HEMP_TRY; 
        ival = hemp_type_text_integer(tval, context);
        ok( 
            hemp_val_int(ival) == expect, 
            "text conversion to integer: %s [%d] [%d]", 
            string, hemp_val_int(ival), expect
        ); 
    HEMP_CATCH_ALL;
        pass("text->integer conversion error: %s : %s", string, hemp->error->message);
    HEMP_END;

    hemp_text_free(text);
    hemp_context_free(context);
    hemp_free(hemp);
}


void test_text_conversion_not_integer(
    hemp_string string
) {
    hemp_hemp          hemp    = hemp_new();
    hemp_context  context = hemp_context_instance(hemp);
    hemp_text     text    = hemp_text_from_string(string);
    hemp_value    tval    = hemp_text_val(text);
    hemp_value    ival;
    
    HEMP_TRY; 
        ival = hemp_type_text_integer(tval, context);
        fail("string should not convert to a integer: %s", string);
    HEMP_CATCH_ALL;
        pass("caught error [%s]->integer: %s", string, hemp->error->message);
    HEMP_END;

    hemp_text_free(text);
    hemp_context_free(context);
    hemp_free(hemp);
}



void test_text_conversion() {
    pass("testing text -> xxx conversions");
    hemp_hemp          hemp    = hemp_new();
    hemp_context  context = hemp_context_instance(hemp);
    hemp_text     text    = hemp_text_from_string("123");
    hemp_value    tval    = hemp_text_val(text);

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
    hemp_value truth = hemp_type_text_boolean(tval, context);
    ok( hemp_is_boolean(truth), "result is boolean" );
    ok( hemp_is_true(truth), "boolean value correct" );

    hemp_text_replace_string(text, "0");
    ok( 
        hemp_is_true( hemp_type_text_boolean(tval, context) ),
        "'0' string is true" 
    );

    hemp_text_replace_string(text, "0.0");
    ok( 
        hemp_is_true( hemp_type_text_boolean(tval, context) ), 
        "'0.0' string is true" 
    );

    hemp_text_replace_string(text, "False");
    ok( 
        hemp_is_true( hemp_type_text_boolean(tval, context) ), 
        "'False' string is true" 
    );

    hemp_text_replace_string(text, "FALSE");
    ok( 
        hemp_is_true( hemp_type_text_boolean(tval, context) ),
        "'FALSE' string is true" 
    );

    hemp_text_replace_string(text, "");
    ok( 
        hemp_is_false( hemp_type_text_boolean(tval, context) ), 
        "empty string is false" 
    );

    hemp_text_free(text);
    hemp_context_free(context);
    hemp_free(hemp);
}


void test_identity_conversion() {
    hemp_hemp          hemp    = hemp_new();
    hemp_context  context = hemp_context_instance(hemp);
    hemp_value    number;
    
    pass("testing identity -> xxx conversions");

    /* identity -> number is not allowed */
    HEMP_TRY; 
        number = hemp_type_identity_number(HempMissing, context);
        fail("missing should not convert to a number");
    HEMP_CATCH_ALL;
        pass("caught error: %s", hemp->error->message);
    HEMP_END;

    /* identity -> boolean is sometimes allowed, depending on value */
    hemp_value truth;
    truth = hemp_type_identity_boolean(HempMissing, context);
    truth = hemp_type_identity_boolean(HempTrue, context);
    truth = hemp_type_identity_boolean(HempBefore, context);

    /* identity -> text is always allowed */
    hemp_value text;

    text = hemp_type_identity_text(HempFalse, context, HempNothing);
    ok( hemp_is_text(text), "false converts to text" );
    is( hemp_val_text(text)->string, "False", "False text" );
// Text now managed by context
//  hemp_text_free( hemp_val_text(text) );

    text = hemp_type_identity_text(HempTrue, context, HempNothing);
    ok( hemp_is_text(text), "true converts to text" );
    is( hemp_val_text(text)->string, "True", "True text" );
//  hemp_text_free( hemp_val_text(text) );

    text = hemp_type_identity_text(HempBefore, context, HempNothing);
    ok( hemp_is_text(text), "before converts to text" );
    is( hemp_val_text(text)->string, "Before", "Before text" );
//  hemp_text_free( hemp_val_text(text) );

    text = hemp_type_identity_text(HempAfter, context, HempNothing);
    ok( hemp_is_text(text), "after converts to text" );
    is( hemp_val_text(text)->string, "After", "After text" );
//  hemp_text_free( hemp_val_text(text) );


    // TODO
    hemp_context_free(context);
    hemp_free(hemp);
}


