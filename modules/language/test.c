#include <hemp.h>

#define HEMP_TEST_NAME     "hemp.test"
#define HEMP_TEST_VERSION   0.1

HEMP_LANGUAGE(hemp_language_test);
HEMP_DIALECT(hemp_dialect_test);
HEMP_GRAMMAR(hemp_grammar_test);

HEMP_ELEMENTS(hemp_language_test_elements);

HEMP_ELEMENT(hemp_element_test_language);
HEMP_ELEMENT(hemp_element_test_dialect);
HEMP_ELEMENT(hemp_element_test_input);
HEMP_ELEMENT(hemp_element_test_output);
HEMP_ELEMENT(hemp_element_test_error);

HEMP_DOC_SCAN(hemp_dialect_test_scanner);

HEMP_SCANNER(hemp_element_test_language_scanner);
HEMP_PREFIX(hemp_element_test_language_prefix);
HEMP_VALUE(hemp_element_test_language_value);

HEMP_VALUE(hemp_element_test_dialect_value);

HEMP_SCANNER(hemp_element_test_input_scanner);
HEMP_PREFIX(hemp_element_test_input_prefix);
HEMP_OUTPUT(hemp_element_test_input_text);
HEMP_CLEANUP(hemp_element_test_input_cleanup);

HEMP_POSTFIX(hemp_element_test_output_branch);
HEMP_OUTPUT(hemp_element_test_output_text);

HempDocument hemp_dialect_test_prepare(HempDocument doc);
void hemp_dialect_test_cleanup(HempDocument doc);

typedef enum { 
    HEMP_TEST_INIT = 0,
    HEMP_TEST_READY,
    HEMP_TEST_RUN_OK,
    HEMP_TEST_RUN_ERROR
}   hemp_test_status;

HempString hemp_test_status_names[] = {
    "Not ready",
    "Ready",
    "Run OK",
    "Run Error"
};


struct hemp_test {
    HempType           type;
    HempText           name;
    HempText           input;
    HempText           output;
    HempText           expect;
    hemp_test_status    status;
};

static struct hemp_elements hemp_elements_test[] = {
    { "hemp.test.language", &hemp_element_test_language },
    { "hemp.test.dialect",  &hemp_element_test_dialect  },
    { "hemp.test.input",    &hemp_element_test_input    },
    { "hemp.test.output",   &hemp_element_test_output   },
    { "hemp.test.error",    &hemp_element_test_error    },
    { NULL, NULL },
};

typedef struct hemp_test *hemp_test;

hemp_test   hemp_test_new();
void        hemp_test_free(hemp_test);


/*--------------------------------------------------------------------------
 * Loader function called once when the module is loader
 *--------------------------------------------------------------------------*/

HempBool
hemp_module_loader(
    HempModule module
) {
//  hemp_debug_msg("loading hemp.test language module\n");
    return HEMP_TRUE;
}


/*--------------------------------------------------------------------------
 * Binder function called to bind the module into a hemp instance
 *--------------------------------------------------------------------------*/


HempBool
hemp_module_binder(
    HempModule module,
    Hemp   hemp
) {
//  hemp_debug_msg("Binding module: %s\n", module->name);
    hemp_register_language(hemp, "test", &hemp_language_test);

    /* Fuckola!  I'm not doing this right.  My globals have got different 
     * addresses... need to figure out the right compiler incantation and/or
     * way to pass globals around
     * 
     * Later... OK, I think it's working now.  Didn't have my compile/link
     * flags set right.  But I'm gonna leave this here for now until I've 
     * convinced myself it's working OK on other platforms.
     */

    if (&HempGlobalData != hemp->global) {
        hemp_debug_msg("hemp.global is %p (namespace: %p)\n", hemp->global, hemp->global->namespace);
        hemp_debug_msg("HempGlobal is %p (namespace: %p)\n", &HempGlobalData, HempGlobalData.namespace);
        hemp_fatal("Plugin error: HempGlobal symbol has not been resolved correctly");
    }

    return HEMP_TRUE;
}


/*--------------------------------------------------------------------------
 * Function to construct an instance of the hemp.test language
 *--------------------------------------------------------------------------*/

HEMP_LANGUAGE(hemp_language_test) {
    hemp_debug_call("hemp_language_test(%p, %s)\n", hemp, name);

    HempLanguage language = hemp_language_new(
        hemp, name, HEMP_TEST_VERSION
    );

    hemp_register_elements(hemp, hemp_elements_test);
    hemp_register_grammar(hemp, name, &hemp_grammar_test);
    hemp_register_dialect(hemp, name, &hemp_dialect_test);

    return language;
}


/*--------------------------------------------------------------------------
 * Function to construct an instance of the hemp.test dialect
 *--------------------------------------------------------------------------*/

HEMP_DIALECT(hemp_dialect_test) {
    HempDialect dialect = hemp_dialect_new(hemp, name);
    dialect->scanner = &hemp_dialect_test_scanner;
    return dialect;
}


HEMP_DOC_SCAN(hemp_dialect_test_scanner) {
    hemp_debug_msg("hemp_dialect_test_scanner(%p)\n", document);

    HempDialect dialect = document->dialect;

    Hemp    hemp    = hemp_document_hemp(document);
    HempTagset  tagset  = hemp_tagset_new(document);
    HempGrammar grammar = hemp_grammar(hemp, dialect->name);
    HempBool    result;

    hemp_tagset_new_tag(tagset, "hemp.outline", "outline", "--", NULL, grammar);

    result = hemp_tagset_scanner(
        tagset, document
    );

    hemp_tagset_free(tagset);

    return result;
}


/*--------------------------------------------------------------------------
 * Function to construct an instance of the hemp.test grammar
 *--------------------------------------------------------------------------*/

HEMP_GRAMMAR(hemp_grammar_test) {
    HempGrammar grammar = hemp_grammar_hemp_alpha(hemp, name);
    HEMP_USE_BLOCK("hemp.test.language",    "language",     11);
    HEMP_USE_BLOCK("hemp.test.dialect",     "dialect",      11);
    HEMP_USE_BLOCK("hemp.test.input",       "test",         11);
    HEMP_USE_BLOCK("hemp.test.output",      "expect",       11);
    HEMP_USE_BLOCK("hemp.test.error",       "error",        11);
    return grammar;
}


/*--------------------------------------------------------------------------
 * hemp.test.language element - specify document language
 *--------------------------------------------------------------------------*/

HEMP_ELEMENT(hemp_element_test_language) {
    hemp_debug_call("hemp_element_test_language(%s)\n", element->name);
    element->parse_prefix    = &hemp_element_test_language_prefix;
    element->scanner         = &hemp_element_test_language_scanner;
    element->value           = &hemp_element_test_language_value,
    element->text            = &hemp_element_value_text;
    element->token           = &hemp_element_literal_text;
    element->source          = &hemp_element_literal_text;
    element->flags           = HEMP_BE_SOURCE | HEMP_BE_FIXED;
    return element;
}


HEMP_SCANNER(hemp_element_test_language_scanner) {
    HempElement element = (HempElement) self;
    HempString  src     = document->scanptr;
    HempTag     tag     = hemp_document_current_tag(document);

    hemp_debug_call("hemp_element_test_language_scanner(%s)\n", element->name);

    if (! tag)
        hemp_fatal("Can't scan test without a tag in scope\n");

    if (! tag->to_eol)
        hemp_fatal("No line scanner for current '%s' tag\n", tag->name);

    /* add the test element */
    hemp_document_scanned_to(
        document, element, src
    );

    hemp_scan_whitespace(document);
    
    /* scan to the end of line (or tag, whichever comes first) */
    src = tag->to_eol(tag, src);
    
    /* add a text element for the rest of the line */
    hemp_document_scanned_to(
        document, HempElementText, src
    );

    return HEMP_TRUE;
}


HEMP_PREFIX(hemp_element_test_language_prefix) {
    HempFragment fragment = *fragptr;
    HempFragment name;

    hemp_debug_call("hemp_element_test_language_prefix(%s)\n", fragment->type->name);

    HEMP_PREFIX_PRECEDENCE;

    /* skip keyword and any whitespace following it */
    hemp_advance(fragptr);
    hemp_skip_whitespace(fragptr);

    /* parse the rest of the line/tag as the language name */
    name = *fragptr;
    hemp_advance(fragptr);
    hemp_set_lhs_fragment(fragment, name);

    return fragment;
}


HEMP_VALUE(hemp_element_test_language_value) {
    HempFragment   fragment    = hemp_val_frag(value);
    HempValue      name        = hemp_lhs(fragment);
    HempText       text        = hemp_text_new();
    hemp_call(name, text, context, hemp_text_val(text));
//  hemp_debug_msg("LANGUAGE: [%s]\n", text->string);
    hemp_language(context->hemp, text->string);
    hemp_text_free(text);
    return hemp_blank();
}


/*--------------------------------------------------------------------------
 * hemp.test.dialect element - specify document dialect
 *--------------------------------------------------------------------------*/

HEMP_ELEMENT(hemp_element_test_dialect) {
    hemp_debug_call("hemp_element_test_dialect(%s)\n", element->name);
    hemp_element_test_language(hemp, element);
    element->value = &hemp_element_test_dialect_value;
    return element;
}


HEMP_VALUE(hemp_element_test_dialect_value) {
    HempFragment   fragment    = hemp_val_frag(value);
    HempValue      name        = hemp_lhs(fragment);
    HempValue      result      = hemp_call(name, text, context, HempNothing);
//  HempText       text        = hemp_val_text(result);
//  hemp_debug_msg("DIALECT: [%s]\n", text->string);
    hemp_hash_store(context->vars, "dialect", result);
    return hemp_blank();
}


/*--------------------------------------------------------------------------
 * hemp.test.input element - what gets fed into a document test
 *--------------------------------------------------------------------------*/

HEMP_ELEMENT(hemp_element_test_input) {
    hemp_debug_call("hemp_element_test_input(%s)\n", element->name);
    element->parse_prefix    = &hemp_element_test_input_prefix;
    element->scanner         = &hemp_element_test_input_scanner;
//  element->cleanup         = &hemp_element_test_input_cleanup,
    element->text            = &hemp_element_test_input_text,
    element->value           = &hemp_element_text_value;
    element->token           = &hemp_element_literal_text;
    element->source          = &hemp_element_literal_text;
    element->flags           = HEMP_BE_SOURCE | HEMP_BE_FIXED;
    return element;
}


HEMP_SCANNER(hemp_element_test_input_scanner) {
    HempElement element = (HempElement) self;
    HempString  src     = document->scanptr;
    HempTag     tag     = hemp_document_current_tag(document);

    hemp_debug_call("hemp_element_test_input_scanner(%s)\n", element->name);

    if (! tag)
        hemp_fatal("Can't scan test without a tag in scope\n");

    if (! tag->to_eol)
        hemp_fatal("No line scanner for current '%s' tag\n", tag->name);

    /* add the test element */
    hemp_document_scanned_to(
        document, element, src
    );
    
    /* scan to the end of line (or tag, whichever comes first) */
    src = tag->to_eol(tag, src);
    
    /* add a text element for the rest of the line */
    hemp_document_scanned_to(
        document, HempElementText, src
    );

    return HEMP_TRUE;
}


HEMP_PREFIX(hemp_element_test_input_prefix) {
    HempFragment fragment = *fragptr;
    HempElement  type     = fragment->type;
    HempFragment name, block, branch;

    hemp_debug_call("hemp_element_test_input_prefix()\n");

    HEMP_PREFIX_PRECEDENCE;
    hemp_advance(fragptr);

    name = *fragptr;
    hemp_advance(fragptr);

    /* specifying our precedence + 1 means that the block will auto-terminate
     * on the next text, because the test will have a lower precedence, as
     * implemented by the HEMP_PREFIX_PRECEDENCE above.
     */
    block = hemp_parse_block(fragptr, scope, type->rprec + 1, 1);

    if (! block)
        hemp_fatal("missing block for %s\n", type->start);
    
    hemp_set_lhs_fragment(fragment, name);
    hemp_set_rhs_fragment(fragment, block);

    /* look for a dangling expect/error branches */
    branch = hemp_parse_branch(fragptr, scope, 0, 0, fragment);

    if (branch) {
//      hemp_debug_msg("got branch: %s\n", branch->type->name);
        fragment->branch = branch;
    }

//  hemp_debug("parsed %s, next element is %s:\n", type->name, (*fragptr)->type->name);

    return fragment;
}


HEMP_OUTPUT(hemp_element_test_input_text) {
    hemp_debug_call("hemp_element_test_input_value()\n");

    HempFragment   fragment = hemp_val_frag(value);
    HempValue      name     = hemp_lhs(fragment);
    HempValue      block    = hemp_rhs(fragment);
    hemp_test       test     = hemp_test_new();
    Hemp       hemp     = context->hemp;
    HempText       text;
    HempValue      dvalue;
    HempString     dialect;

    hemp_prepare_text(context, output, text);

    /* render the name */
    hemp_vtext(name, context, hemp_text_val(test->name));

    /* then the input block */
    hemp_vtext(block, context, hemp_text_val(test->input));

    /* look for the current dialect, specified in a dialect variable */
    dvalue = hemp_hash_fetch(context->vars, "dialect");

    if (hemp_is_missing(dvalue))
        hemp_fatal("No dialect defined");

    dialect = hemp_to_string(dvalue, context);

    test->status = HEMP_TEST_READY;

    HEMP_TRY;
        /* render the input block as a document */
        HempDocument document = hemp_document(
            context->hemp,
            dialect,
            HEMP_TEXT, 
            test->input->string
        );
        hemp_document_process(document, context, test->output);
        test->status = HEMP_TEST_RUN_OK;

    HEMP_CATCH_ALL;
        /* store error text in the output and set status accordingly */
        HempText error = hemp_error_text(hemp->error);
        hemp_text_truncate(test->output, 0);
        hemp_text_append_text(test->output, error);
        hemp_text_free(error);
        test->status = HEMP_TEST_RUN_ERROR;

    HEMP_END;

    if (fragment->branch) {
        /* render the dangling expect/error block, with a ref to this test */
        hemp_hash_store(context->vars, "test", hemp_ptr_val(test));
        hemp_vtext(hemp_frag_val(fragment->branch), context, output);
        hemp_hash_store(context->vars, "test", HempMissing);
    }
    else if (test->status == HEMP_TEST_RUN_OK) {
        /* generate a simple OK message */
        hemp_text_append_string(text, "ok ");
        hemp_text_append_text(text, test->name);
        hemp_text_append_string(text, "\n");
    }
    else {
        /* generate a simple NOT OK message */
        hemp_text_append_string(text, "not ok ");
        hemp_text_append_text(text, test->name);
        hemp_text_append_string(text, "\n");
    }

//    hemp_debug_msg(
//        "TEST (%d:%s): %s\n------\n%s\n=====\n%s\n-----\n", 
//        test->status, hemp_test_status_names[test->status],
//        test->name->string,
//        test->input->string,
//        test->output->string
//    );

    hemp_test_free(test);

    return output;
}


//HEMP_CLEANUP(hemp_element_test_input_cleanup) {
//    hemp_debug_msg("hemp_element_test_input_cleanup()\n");
//    hemp_element_block_cleanup(
//        hemp_rhs_fragment(fragment)
//    );
//}


/*--------------------------------------------------------------------------
 * hemp.test.output element - the expected output for the preceding input
 *--------------------------------------------------------------------------*/

HEMP_ELEMENT(hemp_element_test_output) {
    hemp_debug_call("hemp_element_test_output(%s)\n", element->name);
    hemp_element_test_input(hemp, element);
    element->parse_prefix = NULL;
    element->parse_branch = &hemp_element_test_output_branch;
    element->text         = &hemp_element_test_output_text;
    element->value        = &hemp_element_text_value;
    return element;
}


HEMP_POSTFIX(hemp_element_test_output_branch) {
    HempFragment fragment = *fragptr;
    HempElement  type     = fragment->type;
    HempFragment block;

    hemp_debug_call("hemp_element_test_output_branch(%s)\n", type->name);

    HEMP_PREFIX_PRECEDENCE;
    hemp_advance(fragptr);      /* skip keyword */
    hemp_advance(fragptr);      /* skip name - not interested */

    block = hemp_parse_block(fragptr, scope, type->rprec + 1, 1);

    if (! block)
        hemp_fatal("missing block for %s\n", type->start);

    hemp_set_rhs_fragment(fragment, block);

    return fragment;
}


HEMP_OUTPUT(hemp_element_test_output_text) {
    hemp_debug_call("hemp_element_test_output_text()\n");

    HempFragment   fragment = hemp_val_frag(value);
    HempValue      block    = hemp_rhs(fragment);
    HempText       text;
    hemp_test       test;

    hemp_prepare_text(context, output, text);

    test = (hemp_test) hemp_hash_fetch_pointer(context->vars, "test");

    if (! test)
        hemp_fatal("test is not defined in %s", hemp_type_name(value));

    if (test->status != HEMP_TEST_RUN_OK) {
        // return fail
        hemp_debug_msg("TODO: test failed: %s\n", test->name->string);
        return output;
    }

    hemp_vtext(block, context, hemp_text_val(test->expect));
    hemp_string_chomp(test->output->string);
    hemp_string_chomp(test->expect->string);

    if (hemp_string_eq(test->output->string, test->expect->string)) {
//      printf("EXPECT: [%s%s%s]\n", HEMP_ANSI_YELLOW, expect, HEMP_ANSI_RESET);
//      printf("OUTPUT: [%s%s%s]\n", HEMP_ANSI_GREEN, output->string, HEMP_ANSI_RESET);
//        hemp_debug_msg("TODO: ok %s matches output\n", test->name->string);
        hemp_text_append_string(text, HEMP_ANSI_GREEN);
        hemp_text_append_string(text, "ok (output) ");
        hemp_text_append_text(text, test->name);
        hemp_text_append_string(text, HEMP_ANSI_RESET);
        hemp_text_append_string(text, "\n");
    }
    else {
//        hemp_debug_msg("TODO: not ok %s (output mismatch)\n", test->name->string);
//        printf("EXPECT: [%s%s%s]\n", HEMP_ANSI_YELLOW, test->expect->string, HEMP_ANSI_RESET);
//        printf("OUTPUT: [%s%s%s]\n", HEMP_ANSI_RED, test->output->string, HEMP_ANSI_RESET);
        hemp_text_append_string(text, HEMP_ANSI_RED);
        hemp_text_append_string(text, "NOT ok (output) ");
        hemp_text_append_text(text, test->name);
        hemp_text_append_string(text, HEMP_ANSI_RESET);
        hemp_text_append_string(text, "\n");
    }

    return output;
}


/*--------------------------------------------------------------------------
 * hemp.test.error element - the expected error for the preceding input
 *--------------------------------------------------------------------------*/

HEMP_ELEMENT(hemp_element_test_error) {
    hemp_debug_call("hemp_element_test_error()\n");
    hemp_element_test_output(hemp, element);
    return element;
}



/*--------------------------------------------------------------------------
 * Functions for creating, manipulating and inspecting a runtime test object
 *--------------------------------------------------------------------------*/

hemp_test
hemp_test_new() {
    hemp_test   test;
    HEMP_ALLOCATE(test);
    test->name   = hemp_text_new();
    test->input  = hemp_text_new();
    test->output = hemp_text_new();
    test->expect = hemp_text_new();
    test->status = HEMP_TEST_INIT;
    return test;
}


void
hemp_test_free(
    hemp_test   test
) {
    hemp_text_free(test->name);
    hemp_text_free(test->input);
    hemp_text_free(test->output);
    hemp_text_free(test->expect);

    hemp_mem_free(test);
}

