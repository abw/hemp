#include <hemp/element.h>


/*--------------------------------------------------------------------------
 * parenthesis: ( )
 *--------------------------------------------------------------------------*/

HEMP_SYMBOL_FUNC(hemp_element_parens_symbol) {
    hemp_element_literal_symbol(hemp, symbol);
    symbol->cleanup = &hemp_element_text_clean;
    symbol->prefix  = &hemp_element_parens_prefix;
    symbol->postfix = &hemp_element_parens_postfix;
    symbol->text    = &hemp_element_value_text;
    symbol->value   = &hemp_element_parens_value;
    symbol->number  = &hemp_element_value_number;
    symbol->integer = &hemp_element_value_integer;
    symbol->boolean = &hemp_element_value_boolean;
    symbol->compare = &hemp_element_value_compare;
    return symbol;
}


HEMP_PREFIX_FUNC(hemp_element_parens_prefix) {
    hemp_debug("hemp_element_parens_prefix()\n");
    hemp_element_p self = *elemptr;
    hemp_symbol_p  type = self->type;
    hemp_element_p block;

    HEMP_PREFIX_PRECEDENCE;
    hemp_go_next(elemptr);

    block = hemp_element_parse_block(elemptr, scope, type->rprec, 1);

    if (! block)
        hemp_fatal("missing block for %s\n", type->start);
    
    hemp_set_expr_element(self, block);
    hemp_skip_whitespace(elemptr);

    hemp_debug("parsed test, next element is %s:\n", (*elemptr)->type->name);

//  return hemp_parse_postfix(elemptr, scope, precedence, force, element);

//    # check next token matches our FINISH token
//    return $self->fail_missing( $self->FINISH, $token)
//        unless $$token->is( $self->FINISH );
//    
//    # advance past finish token
//    $$token = $$token->next;
//
//    # list/hash constructs can be followed by postops 
//    return $$token->skip_ws->parse_infix($self, $token, $scope, $prec);


    return self;
}


HEMP_POSTFIX_FUNC(hemp_element_parens_postfix) {
    hemp_debug("hemp_element_parens_postfix()\n");
    return NULL;
}


HEMP_EVAL_FUNC(hemp_element_parens_value) {
    hemp_debug("hemp_element_parens_value()\n");
    return HempNothing;
}



/*--------------------------------------------------------------------------
 * square brackets: [ ]
 *--------------------------------------------------------------------------*/

HEMP_SYMBOL_FUNC(hemp_element_list_symbol) {
    hemp_element_literal_symbol(hemp, symbol);
/*
    TODO
    symbol->cleanup = &hemp_element_text_clean;
    symbol->prefix  = &hemp_element_list_prefix;
    symbol->postfix = &hemp_element_list_postfix;
    symbol->text    = &hemp_element_value_text;
    symbol->value   = &hemp_element_list_value;
    symbol->number  = &hemp_element_value_number;
    symbol->integer = &hemp_element_value_integer;
    symbol->boolean = &hemp_element_value_boolean;
    symbol->compare = &hemp_element_value_compare;
*/
    return symbol;
}


HEMP_PREFIX_FUNC(hemp_element_list_prefix) {
    hemp_debug("hemp_element_list_prefix()\n");
    hemp_element_p self = *elemptr;
    hemp_symbol_p  type = self->type;
    hemp_element_p block;

    HEMP_PREFIX_PRECEDENCE;
    hemp_go_next(elemptr);

    block = hemp_element_parse_block(elemptr, scope, type->rprec, 1);

    if (! block)
        hemp_fatal("missing block for %s\n", type->start);
    
    hemp_set_expr_element(self, block);
    hemp_skip_whitespace(elemptr);

    hemp_debug("parsed test, next element is %s:\n", (*elemptr)->type->name);

//  return hemp_parse_postfix(elemptr, scope, precedence, force, element);

//    # check next token matches our FINISH token
//    return $self->fail_missing( $self->FINISH, $token)
//        unless $$token->is( $self->FINISH );
//    
//    # advance past finish token
//    $$token = $$token->next;
//
//    # list/hash constructs can be followed by postops 
//    return $$token->skip_ws->parse_infix($self, $token, $scope, $prec);


    return self;
}


HEMP_POSTFIX_FUNC(hemp_element_list_postfix) {
    hemp_debug("hemp_element_parens_postfix()\n");
    return NULL;
}


HEMP_EVAL_FUNC(hemp_element_list_value) {
    hemp_debug("hemp_element_list_value()\n");
    return HempNothing;
}
