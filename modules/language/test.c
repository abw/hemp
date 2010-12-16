#include <hemp.h>

#define HEMP_TEST_NAME     "hemp.test"
#define HEMP_TEST_VERSION   0.1

HEMP_LANGUAGE(hemp_language_test);
HEMP_DIALECT(hemp_dialect_test);
HEMP_GRAMMAR(hemp_grammar_test);

HEMP_ELEMENTS(hemp_language_test_elements);

HEMP_ELEMENT(hemp_element_test_input);
HEMP_ELEMENT(hemp_element_test_output);
HEMP_ELEMENT(hemp_element_test_error);

HEMP_SCANNER(hemp_element_test_input_scanner);
HEMP_PREFIX(hemp_element_test_input_prefix);
HEMP_VALUE(hemp_element_test_input_value);
HEMP_CLEANUP(hemp_element_test_input_cleanup);


hemp_document hemp_dialect_test_prepare(hemp_document doc);
void hemp_dialect_test_cleanup(hemp_document doc);


static struct hemp_elements hemp_elements_test[] = {
    { "hemp.test.input",  &hemp_element_test_input    },
    { "hemp.test.output", &hemp_element_test_output   },
    { "hemp.test.error",  &hemp_element_test_error    },
    { NULL, NULL },
};



/*--------------------------------------------------------------------------
 * Binder function called to bind the used module into a hemp instance
 *--------------------------------------------------------------------------*/

hemp_bool
hemp_module_binder(
    hemp_module module,
    hemp_hemp   hemp
) {
    hemp_debug_msg("MODULE name: %s\n", module->name);
    hemp_register_language(hemp, "test", &hemp_language_test);
    return HEMP_TRUE;
}


/*--------------------------------------------------------------------------
 * Function to construct an instance of the hemp.test language
 *--------------------------------------------------------------------------*/

HEMP_LANGUAGE(hemp_language_test) {
    hemp_debug_msg("hemp_language_test(%p, %s)\n", hemp, name);

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
    dialect->cleanup = &hemp_dialect_test_cleanup;
    return dialect;
}


hemp_document
hemp_dialect_test_prepare(
    hemp_document document
) {
    hemp_debug("hemp_dialect_test_prepare(%p)\n", document);

    hemp_dialect dialect = document->dialect;
    hemp_debug_msg("preparing dialect: %p / %s\n", dialect, dialect->name);
    
    hemp_hemp    hemp    = hemp_document_hemp(document);
    hemp_tagset  tagset  = hemp_tagset_prepare(document);
    hemp_grammar grammar = hemp_grammar_instance(hemp, dialect->name);

    hemp_tagset_new_tag(tagset, "hemp.outline", "outline", "--", NULL, grammar);

    return document;
}


void
hemp_dialect_test_cleanup(
    hemp_document document
) {
    hemp_debug_msg("hemp_dialect_test_cleanup(%p)\n", document);
}



/*--------------------------------------------------------------------------
 * Function to construct an instance of the hemp.test grammar
 *--------------------------------------------------------------------------*/

HEMP_GRAMMAR(hemp_grammar_test) {
    hemp_debug("hemp_grammar_test(%p, %s)\n", hemp, name);
    hemp_grammar grammar = hemp_grammar_hemp_alpha(hemp, name);
    HEMP_USE_BLOCK("test.test",   "test",   11);
    HEMP_USE_BLOCK("test.expect", "expect", 11);
    HEMP_USE_BLOCK("test.error",  "error",  11);
    return grammar;
}


/*--------------------------------------------------------------------------
 * hemp.test.input element - what gets fed into a document test
 *--------------------------------------------------------------------------*/

HEMP_ELEMENT(hemp_element_test_input) {
    element->parse_prefix    = &hemp_element_test_input_prefix;
    element->scanner         = &hemp_element_test_input_scanner;
    element->cleanup         = &hemp_element_test_input_cleanup,
    element->value           = &hemp_element_test_input_value,
    element->text            = &hemp_element_value_text,
    element->token           = &hemp_element_literal_text;
    element->source          = &hemp_element_literal_text;
    element->flags           = HEMP_BE_SOURCE | HEMP_BE_FIXED;
    return element;
}


HEMP_SCANNER(hemp_element_test_input_scanner) {
    hemp_debug_msg("hemp_element_test_input_scanner()\n");

    hemp_element element = (hemp_element) self;
    hemp_string  src     = document->scanptr;
    hemp_tag     tag     = hemp_document_current_tag(document);

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
    hemp_fragment self = *fragptr;
    hemp_element  type = self->type;
    hemp_fragment name, block;

    hemp_debug_msg("hemp_element_test_input_prefix()\n");
    hemp_debug("my precedence: %d   request precedence: %d\n", type->rprec, precedence);

    // hmmm... how do we make the first --test end when the next is reached...
    // just have to parse block with rprec + 1, I thinkg
    HEMP_PREFIX_PRECEDENCE;
    hemp_advance(fragptr);

    name = *fragptr;
    hemp_advance(fragptr);

    hemp_debug_msg("%s name\n", type->name);
    hemp_fragment_dump(name);

    block = hemp_fragment_parse_block(fragptr, scope, type->rprec + 1, 1);

    if (! block)
        hemp_fatal("missing block for %s\n", type->start);
    
    hemp_set_lhs_fragment(self, name);
    hemp_set_rhs_fragment(self, block);

    hemp_skip_whitespace(fragptr);

    hemp_debug("parsed %s, next element is %s:\n", type->name, (*fragptr)->type->name);

    return self;
}


HEMP_VALUE(hemp_element_test_input_value) {
//  hemp_text text = hemp_context_tmp_text(context);
    hemp_debug_msg("TODO: hemp_element_test_input_value()\n");
    return hemp_str_val("TODO: test value");
}


HEMP_CLEANUP(hemp_element_test_input_cleanup) {
    hemp_debug_msg("hemp_element_test_input_cleanup()\n");
}


/*--------------------------------------------------------------------------
 * hemp.test.output element - the expected output for the preceding input
 *--------------------------------------------------------------------------*/

HEMP_ELEMENT(hemp_element_test_output) {
    hemp_element_test_input(hemp, element);
    return element;
}


/*--------------------------------------------------------------------------
 * hemp.test.error element - the expected error for the preceding input
 *--------------------------------------------------------------------------*/

HEMP_ELEMENT(hemp_element_test_error) {
    hemp_element_test_input(hemp, element);
    return element;
}

