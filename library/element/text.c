#include <hemp/element.h>


/*--------------------------------------------------------------------------
 * global element types
 *--------------------------------------------------------------------------*/

hemp_element HempElementText = NULL;


/*--------------------------------------------------------------------------
 * text elements and operators created as dynamic grammar elements
 *--------------------------------------------------------------------------*/

HEMP_GLOBAL_ELEMENT(hemp_global_element_text) {
    return hemp_element_text(
        NULL,
        hemp_element_new("hemp.text", NULL, NULL)
    );
}


/*--------------------------------------------------------------------------
 * text element
 *--------------------------------------------------------------------------*/

HEMP_ELEMENT(hemp_element_text) {
    hemp_element_literal(hemp, element);
    element->parse_prefix = &hemp_element_literal_prefix;
    element->flags        = HEMP_BE_SOURCE | HEMP_BE_FIXED;
    return element;
}


HEMP_PREFIX(hemp_element_text_prefix) {
    hemp_debug_call("hemp_element_text_prefix()\n");
    
    /* Advance the pointer to the next element after this one and return the 
     * current element as the yielded expression.
     */
    hemp_fragment fragment = *fragptr;
    hemp_advance(fragptr);
    return fragment;
}



HEMP_VALUE(hemp_element_text_value) {
    return hemp_obcall(value, text, context, HempNothing);
}


/* TODO: perhaps move this as it only applies to FIXED literal text
 * like squote and dquote 
 */

HEMP_CLEANUP(hemp_element_text_cleanup) {
    hemp_debug_call("hemp_element_text_cleanup(%p)\n", element);

    /* squote and dquote elements may have allocated memory to accomodate
     * translated escape sequences, e.g. \n \\, etc
     */

    if (hemp_not_flag(fragment, HEMP_BE_SOURCE)) {
        hemp_mem_free((hemp_memory) hemp_val_str(fragment->args.value));
    }
}


/*--------------------------------------------------------------------------
 * text concatentation '~'
 *--------------------------------------------------------------------------*/

HEMP_ELEMENT(hemp_element_text_concat) {
    hemp_element_infix_left(hemp, element);
    element->value = &hemp_element_text_value;
    element->text  = &hemp_element_text_concat_value;
    return element;
}


HEMP_OUTPUT(hemp_element_text_concat_value) {
    hemp_debug_call("hemp_element_text_concat_value()\n");
    hemp_fragment fragment = hemp_val_frag(value);

    hemp_text text;
    hemp_prepare_text(context, output, text);
    hemp_value lhs = hemp_lhs(fragment);
    hemp_value rhs = hemp_rhs(fragment);
    hemp_obcall(lhs, text, context, output);
    hemp_obcall(rhs, text, context, output);

    return output;
}


/*--------------------------------------------------------------------------
 * text comparison, e.g. '<=>'
 *--------------------------------------------------------------------------*/

HEMP_ELEMENT(hemp_element_text_compare) {
    hemp_element_compare(hemp, element);
    element->compare = &hemp_element_text_compare_value;
    return element;
}


HEMP_VALUE(hemp_element_text_compare_value) {
    hemp_debug_call("hemp_element_text_compare_value()\n");

    hemp_fragment fragment = hemp_val_frag(value);
    hemp_value    lhs      = hemp_lhs(fragment);
    hemp_value    rhs      = hemp_rhs(fragment);
    hemp_value    lval     = hemp_obcall(lhs, text, context, HempNothing);
    hemp_value    rval     = hemp_obcall(rhs, text, context, HempNothing);
    hemp_int      compare  = strcmp( 
        hemp_val_text(lval)->string,
        hemp_val_text(rval)->string
    );
//    hemp_text_free( hemp_val_text(lval) );
//    hemp_text_free( hemp_val_text(rval) );
    
    return  compare < 0 ? HempBefore
        :   compare > 0 ? HempAfter
        :                 HempEqual;
}


/*--------------------------------------------------------------------------
 * text equality, e.g. 'eq'
 *--------------------------------------------------------------------------*/

HEMP_ELEMENT(hemp_element_text_equal) {
    hemp_element_compare_equal(hemp, element);
    element->compare = &hemp_element_text_compare_value;
    return element;
}


/*--------------------------------------------------------------------------
 * text inequality, e.g. 'ne'
 *--------------------------------------------------------------------------*/

HEMP_ELEMENT(hemp_element_text_not_equal) {
    hemp_element_compare_not_equal(hemp, element);
    element->compare = &hemp_element_text_compare_value;
    return element;
}


/*--------------------------------------------------------------------------
 * text less than, e.g. 'lt'
 *--------------------------------------------------------------------------*/

HEMP_ELEMENT(hemp_element_text_before) {
    hemp_element_compare_before(hemp, element);
    element->compare = &hemp_element_text_compare_value;
    return element;
}


/*--------------------------------------------------------------------------
 * text more than more than or equal to, e.g. 'ge'
 *--------------------------------------------------------------------------*/

HEMP_ELEMENT(hemp_element_text_not_before) {
    hemp_element_compare_not_before(hemp, element);
    element->compare = &hemp_element_text_compare_value;
    return element;
}


/*--------------------------------------------------------------------------
 * text more than, e.g. 'gt'
 *--------------------------------------------------------------------------*/

HEMP_ELEMENT(hemp_element_text_after) {
    hemp_element_compare_after(hemp, element);
    element->compare = &hemp_element_text_compare_value;
    return element;
}


/*--------------------------------------------------------------------------
 * text less than or equal to, e.g. 'le'
 *--------------------------------------------------------------------------*/

HEMP_ELEMENT(hemp_element_text_not_after) {
    hemp_element_compare_not_after(hemp, element);
    element->compare = &hemp_element_text_compare_value;
    return element;
}




