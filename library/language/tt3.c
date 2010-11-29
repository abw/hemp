#include <hemp/language/tt3.h>

HEMP_PREFIX_FUNC(hemp_element_tt3_sub_prefix);
HEMP_VALUE_FUNC(hemp_element_tt3_sub_value);
HEMP_OUTPUT_FUNC(hemp_element_tt3_sub_text);
void hemp_element_tt3_sub_clean(hemp_element);

/* see comment in language/hemp.c */
#define DONT_OPTIMISE_ME_AWAY  asm("");

hemp_template hemp_dialect_tt3_prepare(hemp_template tmpl);
void hemp_dialect_tt3_cleanup(hemp_template tmpl);


static struct hemp_symbols hemp_symbols_tt3_command[] = {
    { "tt3.command.if",   &hemp_element_tt3_if_symbol  },
    { "tt3.command.sub",  &hemp_element_tt3_sub_symbol },
    { NULL, NULL },
};


HEMP_LANGUAGE(hemp_language_tt3_new) {
    hemp_debug_call("hemp_language_tt3_new(%p, %s)\n", hemp, name);

    hemp_language language = hemp_language_new(
        hemp, name, HEMP_LANGUAGE_TT3_VERSION
    );
    
    /* register handlers for command symbols */
    HEMP_ELEMENT("tt3.command.*", &hemp_element_tt3_command_symbols);

    /* register grammars */
    hemp_register_grammar(
        hemp, "tt3.core",    &hemp_grammar_tt3_core
    );
    hemp_register_grammar(
        hemp, "tt3.command", &hemp_grammar_tt3_command
    );
    hemp_register_grammar(
        hemp, "tt3.control", &hemp_grammar_tt3_control
    );

    /* register dialects */
    hemp_register_dialect(
        hemp, HEMP_TT3, &hemp_dialect_tt3
    );

//    hemp_register_grammar(
//        hemp, HEMP_TT3, (hemp_actor) &hemp_grammar_tt3
//    );

    return language;
}


/*--------------------------------------------------------------------------
 * dialects
 *--------------------------------------------------------------------------*/

HEMP_DIALECT(hemp_dialect_tt3) {
    hemp_dialect dialect = hemp_dialect_new(hemp, name);
    
    dialect->prepare = &hemp_dialect_tt3_prepare;
    dialect->scanner = &hemp_scan_text;
    dialect->cleanup = NULL; // &hemp_dialect_tt3_cleanup;
    
    return dialect;
}


hemp_template
hemp_dialect_tt3_prepare(
    hemp_template tmpl
    // TODO: options
) {
    hemp_debug_call("hemp_dialect_tt3_prepare(%p)\n", tmpl);

    hemp_hemp         hemp    = tmpl->dialect->hemp;
    hemp_tagset  tagset  = tmpl->tagset;
    hemp_grammar command = hemp_grammar_instance(hemp, "tt3.command");
    hemp_grammar control = hemp_grammar_instance(hemp, "tt3.control");

    hemp_tagset_add_tag(
        tagset, 
        hemp_tag_init(
            "comment",
            HEMP_INLINE_TAG,
            "[#", "#]",
            &hemp_scan_comment_tag,
            NULL
        )
    );

    hemp_tagset_add_tag(
        tagset, 
        hemp_tag_init(
            "control",
            HEMP_INLINE_TAG,
            "[?", "?]",
            &hemp_scan_control_tag,
            control
        )
    );

    hemp_tagset_add_tag(
        tagset, 
        hemp_tag_init(
            "outline", 
            HEMP_OUTLINE_TAG,
            "%%", NULL,
            &hemp_outline_tag_scanner,
            command
        )
    );

    hemp_tagset_add_tag(
        tagset, 
        hemp_tag_init(
            "inline", 
            HEMP_INLINE_TAG,
            "[%", "%]",
            &hemp_inline_tag_scanner,
            command
        )
    );

//    hemp_tagset_add_inline_tag(tagset, HempTagVariable);
//    hemp_tagset_add_inline_tag(tagset, HempTagEmbed);

    return tmpl;
}


void
hemp_dialect_tt3_cleanup(
    hemp_template tmpl
) {
    hemp_debug_call("hemp_dialect_tt3_cleanup(%p)\n", tmpl);
}



/*--------------------------------------------------------------------------
 * grammars
 *--------------------------------------------------------------------------*/

HEMP_GRAMMAR(hemp_grammar_tt3_core) {
    hemp_debug_call("hemp_grammar_tt3_core(%p, %s)\n", hemp, name);
    hemp_grammar grammar = hemp_grammar_hemp_charlie(hemp, name);
    HEMP_SYMBOL2("hemp.squote", "q<<", ">>");
    HEMP_OPERATOR1("hemp.terminator", "end", 0, 0);

    return grammar;
}


HEMP_GRAMMAR(hemp_grammar_tt3_command) {
    hemp_debug_call("hemp_grammar_tt3_command(%p, %s)\n", hemp, name);
    hemp_grammar grammar = hemp_grammar_tt3_core(hemp, name);
    HEMP_OPERATOR1("tt3.command.if", "if", 100, 100);
    HEMP_OPERATOR2("tt3.command.sub", "sub", "end", 100, 100);
    return grammar;
}


HEMP_GRAMMAR(hemp_grammar_tt3_control) {
    hemp_debug_call("hemp_grammar_tt3_control(%p, %s)\n", hemp, name);
    hemp_grammar grammar = hemp_grammar_tt3_core(hemp, name);
    return grammar;
}



/*--------------------------------------------------------------------------
 * elements
 *--------------------------------------------------------------------------*/

hemp_action
hemp_element_tt3_command_symbols(
    hemp_hemp     hemp,
    hemp_string name
) {
    hemp_debug_init("** Initialising tt3 command symbols (%s requested)\n", name);

    /* we should detect if we've done this already and skip it */
    hemp_register_elements(
        hemp, hemp_symbols_tt3_command
    );

    /* now try again */
    return (hemp_action) hemp_hash_fetch_pointer(
        hemp->element->constructors, name
    );
}


hemp_symbol
hemp_element_tt3_TODO_symbol(
    hemp_hemp        hemp,
    hemp_symbol symbol
) {
    hemp_todo("tt3 constructor for %s symbol", symbol->name);
    DONT_OPTIMISE_ME_AWAY;
    return symbol;
}


hemp_symbol
hemp_element_tt3_if_symbol(
    hemp_hemp        hemp,
    hemp_symbol symbol
) {
    return hemp_element_tt3_TODO_symbol(hemp, symbol);
}


/*--------------------------------------------------------------------------
 * sub
 *--------------------------------------------------------------------------*/

hemp_symbol
hemp_element_tt3_sub_symbol(
    hemp_hemp        hemp,
    hemp_symbol symbol
) {
    hemp_debug_call("hemp_element_tt3_sub_symbol()\n");
    symbol->cleanup = &hemp_element_tt3_sub_clean;
    symbol->token   = &hemp_element_literal_token;
    symbol->source  = &hemp_element_literal_source;
    symbol->parse_prefix  = &hemp_element_tt3_sub_prefix;
    symbol->value   = &hemp_element_tt3_sub_value;
    symbol->text    = &hemp_element_tt3_sub_text;
    symbol->number  = &hemp_element_value_number;
    symbol->integer = &hemp_element_value_integer;
    symbol->boolean = &hemp_element_value_boolean;
    symbol->compare = &hemp_element_value_compare;
    symbol->flags   = HEMP_BE_SOURCE;
    return symbol;
}


HEMP_PREFIX_FUNC(hemp_element_tt3_sub_prefix) {
    hemp_debug_call("hemp_element_tt3_sub_prefix()\n");

    hemp_element self = *elemptr;
    hemp_symbol  type = self->type;
    hemp_element name = NULL;
    hemp_element args = NULL;

    /* skip past the 'block' keyword */
    hemp_go_next(elemptr);

    /* next token might be the opening parenthesis of an argument list */
    args = hemp_parse_params(elemptr, scope, 0, 1, self);
    
    /* otherwise it could be a subroutine name... */
    if (! args) {
        hemp_skip_whitespace(elemptr);
        name = hemp_parse_fixed(elemptr, scope, type->lprec, 1);
    }

    /* ...which can be followed by args */
    if (name) {
        hemp_set_flag(self, HEMP_BE_NAMED);
        args = hemp_parse_params(elemptr, scope, 0, 1, self);
    }

    hemp_skip_whitespace(elemptr);

    // TODO: this should call a dedicated block/body method so that elements
    // like '{' can define the appropriate behaviour to capture a block 
    // instead of building a hash...

    hemp_element block = hemp_element_parse_block(elemptr, scope, 0, 1);

    if (! block)
        hemp_fatal("missing block for %s\n", type->start);

    hemp_set_lhs_element(self, name);
    hemp_set_rhs_element(self, block);

    if (args) {
        hemp_set_flag(block, HEMP_BE_ARGS);
        hemp_set_block_args(block, hemp_elem_val(args));
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


HEMP_VALUE_FUNC(hemp_element_tt3_sub_value) {
    hemp_debug_call("hemp_element_tt3_sub_value()\n");

    hemp_element  element = hemp_val_elem(value);
    hemp_value    name    = hemp_lhs(element);
    hemp_value    block   = hemp_rhs(element);
    hemp_size     length;

    if (hemp_val_elem(name)) {
        /* if the subroutine is named then we define it as a variable */
        hemp_value    value   = hemp_call(name, value, context);
        hemp_string      string;

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


HEMP_OUTPUT_FUNC(hemp_element_tt3_sub_text) {
    hemp_debug_call("hemp_element_tt3_sub_text()\n");

    /* call the value() function to define the sub, but generate no output */
    hemp_element_tt3_sub_value(value, context);

    hemp_text text;
    hemp_prepare_text(context, output, text);
    return output;
}


void
hemp_element_tt3_sub_clean(
    hemp_element element
) {
    hemp_debug_call("hemp_element_tt3_sub_clean(%p)\n", element);

    hemp_element_block_clean(
        hemp_rhs_element(element)
    );
}

