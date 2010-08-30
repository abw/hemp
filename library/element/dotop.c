#include <math.h>
#include <hemp/element.h>

/*--------------------------------------------------------------------------
 * dotop '.'
 *--------------------------------------------------------------------------*/

HEMP_SYMBOL_FUNC(hemp_element_dotop_symbol) {
    hemp_element_infix_left_symbol(hemp, symbol);
    symbol->postfix = &hemp_element_dotop_infix;
    symbol->value   = &hemp_element_dotop_value;

    return symbol;
}


HEMP_POSTFIX_FUNC(hemp_element_dotop_infix) {
    hemp_element_p self = *elemptr;
    hemp_symbol_p  type = self->type;

    hemp_debug_call("hemp_element_dotop_parse_infix()\n");

    HEMP_INFIX_LEFT_PRECEDENCE;
    hemp_set_flag(self, HEMP_BE_INFIX);

    self->args.binary.lhs = lhs;
    hemp_go_next(elemptr);
    hemp_skip_whitespace(elemptr);
//  hemp_debug("dotop looking at %s: %s\n", (*elemptr)->type->name, (*elemptr)->token);

    self->args.binary.rhs = hemp_parse_word(elemptr, scope, type->lprec, 1);
    
    if (! self->args.binary.rhs)
        hemp_fatal("missing expression on rhs of %s\n", type->start);

    return hemp_parse_postfix(
        elemptr, scope, precedence, 0,
        self
    );
}


HEMP_EVAL_FUNC(hemp_element_dotop_value) {
    hemp_debug_call("hemp_element_dotop_value()\n");

    hemp_element_p  lhs  = element->args.binary.lhs;
    hemp_element_p  rhs  = element->args.binary.rhs;
    hemp_value_t    lval = lhs->type->value(lhs, context);
    hemp_text_p     text = hemp_text_init(16);

    if (hemp_is_undefined(lval))
        hemp_fatal("TODO: cannot call dotop on undefined value");

    /* hmmm... having to generate a text object to capture the string value 
     * of the RHS is less than ideal...
     */
    rhs->type->token(rhs, context, hemp_text_val(text));
    hemp_value_t result = hemp_send(lval, text->string, context);

    hemp_text_free(text);
    return result;
}


