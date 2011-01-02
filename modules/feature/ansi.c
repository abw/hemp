#include <hemp.h>
#include <hemp/debug.h>

#define HEMP_FEATURE_ANSI_VERSION 0.1

HEMP_FEATURE(hemp_feature_ansi);
HEMP_ELEMENT(hemp_element_ansi);
HEMP_PREFIX(hemp_element_ansi_prefix);
HEMP_VALUE(hemp_element_ansi_value);
HEMP_OUTPUT(hemp_element_ansi_text);


/*--------------------------------------------------------------------------
 * This is a quick hack to demonstrate the principle of having features
 * that can be plugged into an existing grammar. Ultimately it should be
 * possible to compose specify grammars as a base dialect + features, e.g.
 * tt3+ansi+html+whatever.  Furthermore, I'm not sure that this should be
 * implemented this way.  Would be better to have generic semantic colours
 * that can be rendered differently depending on the view (plain text, ANSI
 * text, ANSI extended text, HTML, etc).  However, if those views use 
 * templates to render output, then we might ultimately have ANSI-specific
 * templates, e.g.  [% use ANSI.green; green content %].  So it's here for
 * now.
 *--------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------
 * Binder function registers the ANSI feature with the hemp instance
 *--------------------------------------------------------------------------*/

HempBool
hemp_module_binder(
    HempModule module,
    Hemp   hemp
) {
    hemp_register_feature( hemp, "ansi",      &hemp_feature_ansi );
    hemp_register_element( hemp, "hemp.ansi", &hemp_element_ansi );
    return HEMP_TRUE;
}


/*--------------------------------------------------------------------------
 * Function to mixin the ANSI feature into a grammar.
 *--------------------------------------------------------------------------*/

HEMP_FEATURE(hemp_feature_ansi) {
    hemp_debug_call("hemp_feature_ansi()\n");
    
    // TODO: nice idea, but this doesn't work... well, it does work, but 
    // stashing the ANSI escape code in the end slot doesn't.  We need that
    // for the proper end token, 'end'.  Need to figure out a more general
    // way to specify grammar/element configuration items.

    HEMP_USE_ELEMENT2("hemp.ansi", "red",     HEMP_ANSI_RED     );
    HEMP_USE_ELEMENT2("hemp.ansi", "green",   HEMP_ANSI_GREEN   );
    HEMP_USE_ELEMENT2("hemp.ansi", "yellow",  HEMP_ANSI_YELLOW  );
    HEMP_USE_ELEMENT2("hemp.ansi", "blue",    HEMP_ANSI_BLUE    );
    HEMP_USE_ELEMENT2("hemp.ansi", "magenta", HEMP_ANSI_MAGENTA );
    HEMP_USE_ELEMENT2("hemp.ansi", "cyan",    HEMP_ANSI_CYAN    );
    HEMP_USE_ELEMENT2("hemp.ansi", "white",   HEMP_ANSI_WHITE   );
    return grammar;
}


/*--------------------------------------------------------------------------
 * Definition of hemp.ansi element
 *--------------------------------------------------------------------------*/

HEMP_ELEMENT(hemp_element_ansi) {
    hemp_debug_call("hemp_element_ansi()\n");
    element->parse_prefix    = &hemp_element_ansi_prefix;
    element->token           = &hemp_element_literal_text;
    element->source          = &hemp_element_literal_text;
    element->value           = &hemp_element_ansi_value;
    element->text            = &hemp_element_ansi_text;
    element->number          = &hemp_element_value_number;
    element->integer         = &hemp_element_value_integer;
    element->boolean         = &hemp_element_value_boolean;
    element->compare         = &hemp_element_value_compare;
    return element;
}


/*--------------------------------------------------------------------------
 * Function to parse an ANSI element, e.g. 
 *  %% green "Hello World"
 *  %% green { "Hello " name }
 *  %% green; "Hello " name; end
 *--------------------------------------------------------------------------*/

HEMP_PREFIX(hemp_element_ansi_prefix) {
    hemp_debug_msg("hemp_element_ansi_prefix()\n");

    HempFragment fragment = *fragptr;
    HempFragment block;

    /* skip past the keyword */
    hemp_advance(fragptr);

    block = hemp_parse_rhs_body(fragment);
    hemp_debug_msg("AAA\n");
    hemp_parse_body_terminator(fragment, block);
    hemp_debug_msg("BBB\n");

    return fragment;
}


/*--------------------------------------------------------------------------
 * When a raw value is requested we return the inner content unmodified.
 *--------------------------------------------------------------------------*/

HEMP_VALUE(hemp_element_ansi_value) {
    hemp_debug_msg("hemp_element_ansi_value()\n");
    HempFragment fragment = hemp_val_frag(value);
    HempValue    body     = hemp_rhs(fragment);
    return hemp_call(body, value, context);
}


/*--------------------------------------------------------------------------
 * When text is requested, we add the ANSI escape sequences to enable and 
 * reset the colour.  The escape sequence is stored in the element's end tag.
 *--------------------------------------------------------------------------*/

HEMP_OUTPUT(hemp_element_ansi_text) {
    hemp_debug_msg("hemp_element_ansi_text()\n");
    HempFragment fragment = hemp_val_frag(value);
    HempValue    body     = hemp_rhs(fragment);
    HempText     text;

    hemp_prepare_text(context, output, text);
    hemp_text_append_string(text, fragment->type->end);
    hemp_vtext(body, context, output);
    hemp_text_append_string(text, HEMP_ANSI_RESET);
    return output;
}

