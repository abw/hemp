#include <math.h>
#include <hemp/element.h>


/*--------------------------------------------------------------------------
 * boolean operators
 *--------------------------------------------------------------------------*/

HEMP_OUTPUT_FUNC(hemp_element_boolop_text) {
    debug_call("hemp_element_boolop_text()\n");

    hemp_text_p text;
    hemp_prepare_output(output, text, 10);      // FIXME

    hemp_text_append_cstr(
        text, 
        HEMP_IS_TRUE( element->type->boolean(element, context) )
            ? HEMP_STR_TRUE
            : HEMP_STR_FALSE
    );

    return output;
}

