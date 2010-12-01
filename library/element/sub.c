#include <hemp/element.h>

HEMP_PREFIX_FUNC(hemp_element_sub_prefix);
HEMP_VALUE_FUNC(hemp_element_sub_value);
HEMP_OUTPUT_FUNC(hemp_element_sub_text);
HEMP_CLEAN_FUNC(hemp_element_sub_clean);


HEMP_SYMBOL(hemp_element_sub_symbol) {
    hemp_debug_call("hemp_element_sub_symbol()\n");
    symbol->parse_prefix    = &hemp_element_sub_prefix;
    symbol->cleanup         = &hemp_element_sub_clean;
    symbol->token           = &hemp_element_literal_token;
    symbol->source          = &hemp_element_literal_source;
    symbol->value           = &hemp_element_sub_value;
    symbol->text            = &hemp_element_sub_text;
    symbol->number          = &hemp_element_value_number;
    symbol->integer         = &hemp_element_value_integer;
    symbol->boolean         = &hemp_element_value_boolean;
    symbol->compare         = &hemp_element_value_compare;
    symbol->flags           = HEMP_BE_SOURCE;
    return symbol;
}


HEMP_PREFIX_FUNC(hemp_element_sub_prefix) {
    hemp_debug_call("hemp_element_sub_prefix()\n");

    hemp_element self   = *elemptr;
    hemp_symbol  type   = self->type;
    hemp_element name   = NULL;
    hemp_element params = NULL;

    /* skip past the 'block' keyword */
    hemp_go_next(elemptr);

    /* next token might be the opening parenthesis of an argument list */
    params = hemp_parse_params(elemptr, scope, 0, 1);
    
    /* otherwise it could be a subroutine name... */
    if (! params) {
        hemp_skip_whitespace(elemptr);
        name = hemp_parse_fixed(elemptr, scope, type->lprec, 1);
    }

    /* ...which can be followed by args */
    if (name) {
        hemp_set_flag(self, HEMP_BE_NAMED);
        params = hemp_parse_params(elemptr, scope, 0, 1);
    }

    hemp_skip_whitespace(elemptr);

    // TODO: this should call a dedicated block/body method so that elements
    // like '{' can define the appropriate behaviour to capture a block 
    // instead of building a hash...

    hemp_element block = hemp_element_parse_block(elemptr, scope, 0, 1);

    if (! block)
        hemp_fatal("missing block for %s\n", type->start);

    hemp_debug_msg("sub block: %p\n", block);
    hemp_set_lhs_element(self, name);
//    hemp_set_rhs_element(self, block);

    if (params) {
        hemp_debug("constructing code value to manage sub with prototype\n");
        /* construct a code value to wrap around the block and handle params */
        hemp_set_flag(self, HEMP_BE_ARGS);
        hemp_code   code    = hemp_code_new();
        hemp_proto  proto   = hemp_code_proto(code);
        hemp_value  protov  = hemp_ptr_val(proto);
        code->body          = hemp_elem_val(block);
        params->type->parse_proto(params, scope, protov);
        hemp_set_rhs(self, hemp_code_val(code));
    }
    else {
        hemp_set_rhs_element(self, block);
    }

    if (hemp_element_terminator_matches(*elemptr, type->end)) {
//      hemp_debug_msg("found matching terminator for %s => %s\n", type->start, type->end);
        hemp_go_next(elemptr);
    }
    else {
        hemp_fatal("missing terminator to match %s => %s\n", type->start, type->end);
    }

    return self;
}


HEMP_VALUE_FUNC(hemp_element_sub_value) {
    hemp_debug_call("hemp_element_sub_value()\n");

    hemp_element  element = hemp_val_elem(value);
    hemp_value    name    = hemp_lhs(element);
    hemp_value    block   = hemp_rhs(element);
    hemp_size     length;

    if (hemp_val_elem(name)) {
        /* if the subroutine is named then we define it as a variable */
        hemp_value  value = hemp_call(name, value, context);
        hemp_string string;

        if (hemp_is_string(value)) {
            string = hemp_val_str(value);
            length = strlen(string);
        }
        else {
            // FIXME - this code isn't used at present, but may be as and 
            // when we support subroutines with non-static names... but 
            // on second thoughts, perhaps we don't need that anyway.
            hemp_debug_msg("WARNING!  Memory leak!\n");
            hemp_text text  = hemp_text_new();
            hemp_obcall(name, text, context, hemp_text_val(text));
            string = hemp_string_clone(text->string, "sub name");
            length = text->length;
        }
        hemp_hash_store_keylen(
            context->vars, string, block, length
        );
    }

    return block;
}


HEMP_OUTPUT_FUNC(hemp_element_sub_text) {
    hemp_debug_call("hemp_element_sub_text()\n");

    /* call the value() function to define the sub, but generate no output */
    hemp_element_sub_value(value, context);

    hemp_text text;
    hemp_prepare_text(context, output, text);
    return output;
}


HEMP_CLEAN_FUNC(hemp_element_sub_clean) {
    hemp_debug_call("hemp_element_sub_clean(%p)\n", element);

    if (hemp_has_flag(element, HEMP_BE_ARGS)) {
        hemp_code code = hemp_val_code( hemp_rhs(element) );
        hemp_code_free(code);
    }
}

