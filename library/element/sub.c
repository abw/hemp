#include <hemp/element.h>


HEMP_ELEMENT(hemp_element_sub) {
    hemp_debug_call("hemp_element_sub()\n");
    element->parse_prefix    = &hemp_element_sub_prefix;
    element->cleanup         = &hemp_element_sub_cleanup;
    element->token           = &hemp_element_literal_text;
    element->source          = &hemp_element_literal_text;
    element->value           = &hemp_element_sub_value;
    element->text            = &hemp_element_sub_text;
    element->number          = &hemp_element_value_number;
    element->integer         = &hemp_element_value_integer;
    element->boolean         = &hemp_element_value_boolean;
    element->compare         = &hemp_element_value_compare;
    element->flags           = HEMP_BE_SOURCE;
    return element;
}


HEMP_PREFIX(hemp_element_sub_prefix) {
    hemp_debug_call("hemp_element_sub_prefix()\n");

    hemp_fragment fragment = *fragptr;
    hemp_element  type     = fragment->type;
    hemp_fragment name     = NULL;
    hemp_fragment params   = NULL;

    /* skip past the 'block' keyword */
    hemp_advance(fragptr);

    /* next token might be the opening parenthesis of an argument list */
    params = hemp_parse_params(fragptr, scope, 0, 1);
    
    /* otherwise it could be a subroutine name... */
    if (! params) {
        hemp_skip_whitespace(fragptr);
        name = hemp_parse_fixed(fragptr, scope, type->lprec, 1);
    }

    /* ...which can be followed by args */
    if (name) {
        hemp_set_flag(fragment, HEMP_BE_NAMED);
        params = hemp_parse_params(fragptr, scope, 0, 1);
    }

    hemp_skip_whitespace(fragptr);

//  hemp_debug_msg(
//      "body elem is %s, parse_body function is at %p\n", 
//      (*elemptr)->type->name, (*elemptr)->type->parse_body
//  );

    /* see comments in Template::TT3::Element::parse_body wrt precedence/force */
    hemp_fragment block = hemp_parse_body(fragptr, scope, type->rprec, 1);

    if (! block)
        hemp_fatal("missing block for %s\n", type->start);

    hemp_set_lhs_fragment(fragment, name);

    if (params) {
        /* construct a code value to wrap around the block and handle params */
        hemp_set_flag(fragment, HEMP_BE_ARGS);
        hemp_code   code    = hemp_code_new();
        hemp_proto  proto   = hemp_code_proto(code);
        hemp_value  protov  = hemp_ptr_val(proto);
        code->body          = hemp_frag_val(block);
        params->type->parse_proto(params, scope, protov);
        hemp_set_rhs(fragment, hemp_code_val(code));
    }
    else {
        hemp_set_rhs_fragment(fragment, block);
    }

    hemp_parse_body_terminator(fragment, block);

    return fragment;
}


HEMP_VALUE(hemp_element_sub_value) {
    hemp_debug_call("hemp_element_sub_value()\n");

    hemp_fragment fragment = hemp_val_frag(value);
    hemp_value    name     = hemp_lhs(fragment);
    hemp_value    block    = hemp_rhs(fragment);
    hemp_size     length;

    if (hemp_val_frag(name)) {
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
//      hemp_debug_msg("sub value setting %s\n", hemp_type_name(block));
        hemp_hash_store_keylen(
            context->vars, string, block, length
        );
    }

    return block;
}


HEMP_OUTPUT(hemp_element_sub_text) {
    hemp_debug_call("hemp_element_sub_text()\n");

    /* call the value() function to define the sub, but generate no output */
    hemp_element_sub_value(value, context);

    hemp_text text;
    hemp_prepare_text(context, output, text);
    return output;
}


HEMP_CLEANUP(hemp_element_sub_cleanup) {
    hemp_debug_call("hemp_element_sub_clean(%p)\n", fragment);

    if (hemp_has_flag(fragment, HEMP_BE_ARGS)) {
        hemp_code code = hemp_val_code( hemp_rhs(fragment) );
        hemp_code_free(code);
    }
}

