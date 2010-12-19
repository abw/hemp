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

HEMP_SCANNER(hemp_element_test_language_scanner);
HEMP_PREFIX(hemp_element_test_language_prefix);
HEMP_VALUE(hemp_element_test_language_value);

HEMP_VALUE(hemp_element_test_dialect_value);

HEMP_SCANNER(hemp_element_test_input_scanner);
HEMP_PREFIX(hemp_element_test_input_prefix);
HEMP_VALUE(hemp_element_test_input_value);
HEMP_CLEANUP(hemp_element_test_input_cleanup);

HEMP_POSTFIX(hemp_element_test_output_branch);

hemp_document hemp_dialect_test_prepare(hemp_document doc);
void hemp_dialect_test_cleanup(hemp_document doc);

typedef enum { 
    HEMP_TEST_INIT = 0,
    HEMP_TEST_READY,
    HEMP_TEST_RUN_OK,
    HEMP_TEST_RUN_ERROR
}   hemp_test_status;

hemp_string hemp_test_status_names[] = {
    "Not ready",
    "Ready",
    "Run OK",
    "Run Error"
};


struct hemp_test {
    hemp_type           type;
    hemp_text           name;
    hemp_text           input;
    hemp_text           output;
    hemp_text           expect;
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

hemp_bool
hemp_module_loader(
    hemp_module module
) {
//  hemp_debug_msg("loading hemp.test language module\n");
    return HEMP_TRUE;
}


/*--------------------------------------------------------------------------
 * Binder function called to bind the module into a hemp instance
 *--------------------------------------------------------------------------*/


hemp_bool
hemp_module_binder(
    hemp_module module,
    hemp_hemp   hemp
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

    if (&HempGlobal != hemp->global) {
        hemp_debug_msg("hemp.global is %p (namespace: %p)\n", hemp->global, hemp->global->namespace);
        hemp_debug_msg("HempGlobal is %p (namespace: %p)\n", &HempGlobal, HempGlobal.namespace);
        hemp_fatal("Plugin error: HempGlobal symbol has not been resolved correctly");
    }

    return HEMP_TRUE;
}


/*--------------------------------------------------------------------------
 * Function to construct an instance of the hemp.test language
 *--------------------------------------------------------------------------*/

HEMP_LANGUAGE(hemp_language_test) {
    hemp_debug_call("hemp_language_test(%p, %s)\n", hemp, name);

    hemp_language language = hemp_language_new(
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
    hemp_dialect dialect = hemp_dialect_new(hemp, name);
    dialect->prepare = &hemp_dialect_test_prepare;
    dialect->cleanup = &hemp_tagset_cleanup;
    return dialect;
}


hemp_document
hemp_dialect_test_prepare(
    hemp_document document
) {
    hemp_debug_call("hemp_dialect_test_prepare(%p)\n", document);

    hemp_dialect dialect = document->dialect;

    hemp_hemp    hemp    = hemp_document_hemp(document);
    hemp_tagset  tagset  = hemp_tagset_prepare(document);
    hemp_grammar grammar = hemp_grammar_instance(hemp, dialect->name);
    hemp_tagset_new_tag(tagset, "hemp.outline", "outline", "--", NULL, grammar);

    return document;
}



/*--------------------------------------------------------------------------
 * Function to construct an instance of the hemp.test grammar
 *--------------------------------------------------------------------------*/

HEMP_GRAMMAR(hemp_grammar_test) {
    hemp_grammar grammar = hemp_grammar_hemp_alpha(hemp, name);
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
    hemp_element element = (hemp_element) self;
    hemp_string  src     = document->scanptr;
    hemp_tag     tag     = hemp_document_current_tag(document);

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
    hemp_fragment fragment = *fragptr;
    hemp_fragment name;

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
    hemp_fragment   fragment    = hemp_val_frag(value);
    hemp_value      name        = hemp_lhs(fragment);
    hemp_text       text        = hemp_text_new();
    hemp_call(name, text, context, hemp_text_val(text));
    hemp_debug_msg("LANGUAGE: [%s]\n", text->string);
    hemp_language_instance(context->hemp, text->string);
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
    hemp_fragment   fragment    = hemp_val_frag(value);
    hemp_value      name        = hemp_lhs(fragment);
    hemp_value      result      = hemp_call(name, text, context, HempNothing);
    hemp_text       text        = hemp_val_text(result);
    hemp_debug_msg("DIALECT: [%s]\n", text->string);
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
    element->value           = &hemp_element_test_input_value,
    element->text            = &hemp_element_value_text;
    element->token           = &hemp_element_literal_text;
    element->source          = &hemp_element_literal_text;
    element->flags           = HEMP_BE_SOURCE | HEMP_BE_FIXED;
    return element;
}


HEMP_SCANNER(hemp_element_test_input_scanner) {
    hemp_element element = (hemp_element) self;
    hemp_string  src     = document->scanptr;
    hemp_tag     tag     = hemp_document_current_tag(document);

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
    hemp_fragment fragment = *fragptr;
    hemp_element  type     = fragment->type;
    hemp_fragment name, block, branch;

    hemp_debug_call("hemp_element_test_input_prefix()\n");

    HEMP_PREFIX_PRECEDENCE;
    hemp_advance(fragptr);

    name = *fragptr;
    hemp_advance(fragptr);

    /* specifying our precedence + 1 means that the block will auto-terminate
     * on the next text, because the test will have a lower precedence, as
     * implemented by the HEMP_PREFIX_PRECEDENCE above.
     */
    block = hemp_fragment_parse_block(fragptr, scope, type->rprec + 1, 1);

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


HEMP_VALUE(hemp_element_test_input_value) {
    hemp_debug_call("hemp_element_test_input_value()\n");

    hemp_fragment   fragment = hemp_val_frag(value);
    hemp_value      name     = hemp_lhs(fragment);
    hemp_value      block    = hemp_rhs(fragment);
    hemp_test       test     = hemp_test_new();
    hemp_hemp       hemp     = context->hemp;
    hemp_value      dialect;
    
    /* render the name */
    hemp_call(name, text, context, hemp_text_val(test->name));

    /* then the input block */
    hemp_call(block, text, context, hemp_text_val(test->input));

    /* look for the current dialect, specified in a dialect variable */
    dialect = hemp_hash_fetch(context->vars, "dialect");

    if (hemp_is_missing(dialect))
        hemp_fatal("No dialect defined");

    dialect = hemp_to_string(dialect, context);

    hemp_debug_msg("USING DIALECT: %s\n", hemp_val_str(dialect));

    test->status = HEMP_TEST_READY;

    HEMP_TRY;
        hemp_document document = hemp_document_instance(
            context->hemp,
            hemp_val_str(dialect),
            HEMP_TEXT, 
            test->input->string
        );
        hemp_document_process(document, context, test->output);
	//        hemp_text_append_string(test->output, "<TODO>");
        test->status = HEMP_TEST_RUN_OK;

    HEMP_CATCH_ALL;
        hemp_text error = hemp_error_text(hemp->error);
        hemp_debug_msg("ERROR: %s\n", error->string);
        hemp_text_truncate(test->output, 0);
        hemp_text_append_text(test->output, error);
        hemp_text_free(error);
        test->status = HEMP_TEST_RUN_ERROR;

    HEMP_END;

    /* TODO: render the fragment block if there is one, or generate a 
     * pass/fail result if not.
     */

    if (fragment->branch) {
        hemp_debug_msg("TODO: render branch\n");

        hemp_hash_store(context->vars, "test", hemp_obj_val((hemp_object) test));

        // TODO

        hemp_hash_store(context->vars, "test", HempMissing);
    }
    else if (test->status == HEMP_TEST_RUN_OK) {
        printf("TODO: OK %s\n", test->name->string);
    }
    else {
        printf("TODO: NOT OK %s\n", test->name->string);
    }

    hemp_debug_msg(
        "TEST (%d:%s): %s\n------\n%s\n=====\n%s\n-----\n", 
        test->status, hemp_test_status_names[test->status],
        test->name->string,
        test->input->string,
        test->output->string
    );

    hemp_test_free(test);

    return hemp_str_val("TODO: report test success/fail\n");
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
    return element;
}


HEMP_POSTFIX(hemp_element_test_output_branch) {
    hemp_fragment fragment = *fragptr;
    hemp_element  type     = fragment->type;
    hemp_fragment block;

    hemp_debug_call("hemp_element_test_output_branch(%s)\n", type->name);

    HEMP_PREFIX_PRECEDENCE;
    hemp_advance(fragptr);      /* skip keyword */
    hemp_advance(fragptr);      /* skip name - not interested */

    block = hemp_fragment_parse_block(fragptr, scope, type->rprec + 1, 1);

    if (! block)
        hemp_fatal("missing block for %s\n", type->start);

    hemp_set_rhs_fragment(fragment, block);

    return fragment;
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

