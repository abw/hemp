#include <hemp.h>

#define HEMP_TT3_LANGUAGE_VERSION 0.2

HEMP_DIALECT_FUNC(hemp_dialect_tt3);
HEMP_GRAMMAR_FUNC(hemp_grammar_tt3_core);
HEMP_GRAMMAR_FUNC(hemp_grammar_tt3_command);
HEMP_GRAMMAR_FUNC(hemp_grammar_tt3_control);
HEMP_SYMBOLS_FUNC(hemp_element_tt3_command_symbols);
HEMP_SYMBOL_FUNC(hemp_element_tt3_if_symbol);
HEMP_SYMBOL_FUNC(hemp_element_tt3_TODO_symbol);

HEMP_SYMBOL_FUNC(hemp_element_tt3_sub_symbol);
HEMP_PREFIX_FUNC(hemp_element_tt3_sub_prefix);
HEMP_VALUE_FUNC(hemp_element_tt3_sub_value);

/* see comment in language/hemp.c */
#define DONT_OPTIMISE_ME_AWAY  asm("");

hemp_template_p hemp_dialect_tt3_prepare(hemp_template_p tmpl);
void hemp_dialect_tt3_cleanup(hemp_template_p tmpl);


static struct hemp_symbols_s hemp_symbols_tt3_command[] = {
    { "tt3.command.if",   &hemp_element_tt3_if_symbol  },
    { "tt3.command.sub",  &hemp_element_tt3_sub_symbol },
    { NULL, NULL },
};


HEMP_LANGUAGE_FUNC(hemp_language_tt3_init) {
    hemp_debug_call("hemp_language_tt3_init(%p, %s)\n", hemp, name);

    hemp_language_p language = hemp_language_init(
        hemp, name, HEMP_TT3_LANGUAGE_VERSION
    );
    
    /* register handlers for command symbols */
    HEMP_ELEMENT("tt3.command.*", &hemp_element_tt3_command_symbols);

    /* register grammars */
    HEMP_GRAMMAR("tt3.core",    &hemp_grammar_tt3_core);
    HEMP_GRAMMAR("tt3.command", &hemp_grammar_tt3_command);
    HEMP_GRAMMAR("tt3.control", &hemp_grammar_tt3_control);

    /* register dialects */
    HEMP_DIALECT(HEMP_TT3, &hemp_dialect_tt3);

//    hemp_register_grammar(
//        hemp, HEMP_TT3, (hemp_actor_f) &hemp_grammar_tt3
//    );

    return language;
}


/*--------------------------------------------------------------------------
 * dialects
 *--------------------------------------------------------------------------*/

HEMP_DIALECT_FUNC(hemp_dialect_tt3) {
    hemp_dialect_p dialect = hemp_dialect_init(hemp, name);
    
    dialect->prepare = &hemp_dialect_tt3_prepare;
    dialect->scanner = &hemp_scan_text;
    dialect->cleanup = NULL; // &hemp_dialect_tt3_cleanup;
    
    return dialect;
}


hemp_template_p
hemp_dialect_tt3_prepare(
    hemp_template_p tmpl
    // TODO: options
) {
    hemp_debug_call("hemp_dialect_tt3_prepare(%p)\n", tmpl);

    hemp_p         hemp    = tmpl->dialect->hemp;
    hemp_tagset_p  tagset  = tmpl->tagset;
    hemp_grammar_p command = hemp_grammar(hemp, "tt3.command");
    hemp_grammar_p control = hemp_grammar(hemp, "tt3.control");

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
    hemp_template_p tmpl
) {
    hemp_debug_call("hemp_dialect_tt3_cleanup(%p)\n", tmpl);
}



/*--------------------------------------------------------------------------
 * grammars
 *--------------------------------------------------------------------------*/

HEMP_GRAMMAR_FUNC(hemp_grammar_tt3_core) {
    hemp_debug_call("hemp_grammar_tt3_core(%p, %s)\n", hemp, name);
    hemp_grammar_p grammar = hemp_grammar_hemp_charlie(hemp, name);
    HEMP_SYMBOL2("hemp.squote", "q<<", ">>");
    HEMP_OPERATOR1("hemp.terminator", "end", 0, 0);

    return grammar;
}


HEMP_GRAMMAR_FUNC(hemp_grammar_tt3_command) {
    hemp_debug_call("hemp_grammar_tt3_command(%p, %s)\n", hemp, name);
    hemp_grammar_p grammar = hemp_grammar_tt3_core(hemp, name);
    HEMP_OPERATOR1("tt3.command.if", "if", 100, 100);
    HEMP_OPERATOR2("tt3.command.sub", "sub", "end", 100, 100);
    return grammar;
}


HEMP_GRAMMAR_FUNC(hemp_grammar_tt3_control) {
    hemp_debug_call("hemp_grammar_tt3_control(%p, %s)\n", hemp, name);
    hemp_grammar_p grammar = hemp_grammar_tt3_core(hemp, name);
    return grammar;
}



/*--------------------------------------------------------------------------
 * elements
 *--------------------------------------------------------------------------*/

hemp_action_p
hemp_element_tt3_command_symbols(
    hemp_p     hemp,
    hemp_str_p name
) {
    hemp_debug_yellow("** Initialising tt3 command symbols (%s requested)\n", name);

    /* we should detect if we've done this already and skip it */
    HEMP_ELEMENTS(hemp_symbols_tt3_command);

    /* now try again */
    return (hemp_action_p) hemp_hash_fetch_pointer(
        hemp->elements->constructors, name
    );
}


hemp_symbol_p
hemp_element_tt3_TODO_symbol(
    hemp_p        hemp,
    hemp_symbol_p symbol
) {
    hemp_todo("tt3 constructor for %s symbol", symbol->name);
    DONT_OPTIMISE_ME_AWAY;
    return symbol;
}


hemp_symbol_p
hemp_element_tt3_if_symbol(
    hemp_p        hemp,
    hemp_symbol_p symbol
) {
    return hemp_element_tt3_TODO_symbol(hemp, symbol);
}


/*--------------------------------------------------------------------------
 * sub
 *--------------------------------------------------------------------------*/

hemp_symbol_p
hemp_element_tt3_sub_symbol(
    hemp_p        hemp,
    hemp_symbol_p symbol
) {
    hemp_debug("hemp_element_tt3_sub_symbol()\n");
    symbol->token   = &hemp_element_literal_token;
    symbol->source  = &hemp_element_literal_source;
    symbol->prefix  = &hemp_element_tt3_sub_prefix;
    symbol->value   = &hemp_element_tt3_sub_value;
    symbol->text    = &hemp_element_value_text;
    symbol->number  = &hemp_element_value_number;
    symbol->integer = &hemp_element_value_integer;
    symbol->boolean = &hemp_element_value_boolean;
    symbol->compare = &hemp_element_value_compare;
    symbol->flags   = HEMP_BE_SOURCE;
    return symbol;
}


HEMP_PREFIX_FUNC(hemp_element_tt3_sub_prefix) {
    hemp_debug("hemp_element_tt3_sub_prefix()\n");

    hemp_element_p self = *elemptr;
    hemp_symbol_p  type = self->type;
    hemp_element_p name = NULL;
    hemp_element_p args = NULL;

    /* skip past the 'block' keyword */
    hemp_go_next(elemptr);

    /* next token might be the opening parenthesis of an argument list */
    args = hemp_parse_params(elemptr, scope, 0, 1, self);
    
    if (args) {
        /* NOTE: quick string comparison hack until we've got something better in place */
        // if (hemp_string_eq((*elemptr)->type->name, "hemp.bracket.parens")) {
        hemp_debug("found parens after sub\n");
    }
    else {
        hemp_skip_whitespace(elemptr);
        name = hemp_parse_fixed(elemptr, scope, type->lprec, 1);

        if (name) {
            hemp_debug("found name for sub\n");
            hemp_set_flag(self, HEMP_BE_NAMED);
            args = hemp_parse_params(elemptr, scope, 0, 1, self);
            if (args) {
                // if (hemp_string_eq((*elemptr)->type->name, "hemp.bracket.parens")) {
                hemp_debug("found parens after sub name\n");
            }
        }
    }

    hemp_skip_whitespace(elemptr);

    // TODO: this should call a dedicated block/body method so that elements
    // like '{' can define the appropriate behaviour to capture a block 
    // instead of building a hash...

   hemp_element_p block = hemp_element_parse_block(elemptr, scope, 0, 1);

    if (! block)
        hemp_fatal("missing block for %s\n", type->start);

    hemp_debug("parsed block for sub\n");
    hemp_set_lhs_element(self, name);
    hemp_set_rhs_element(self, block);
    hemp_set_block_args(block, hemp_elem_val(args));

    if (hemp_element_terminator_matches(*elemptr, type->end)) {
        hemp_debug("found matching terminator for %s => %s\n", type->start, type->end);
        hemp_go_next(elemptr);
    }
    else {
        hemp_fatal("missing terminator to match %s => %s\n", type->start, type->end);
    }

    return self;
}


HEMP_VALUE_FUNC(hemp_element_tt3_sub_value) {
    hemp_debug("hemp_element_sub_value()\n");

    hemp_element_p  element = hemp_val_elem(value);
    hemp_value_t    name    = hemp_lhs(element);
    hemp_value_t    block   = hemp_rhs(element);
    hemp_value_t    args    = hemp_block_args( hemp_val_elem(block) );

    if (hemp_val_elem(name)) {
        hemp_text_p text  = hemp_text_new();
        hemp_obcall(name, text, context, hemp_text_val(text));
        hemp_debug("sub is named: %s\n", text->string);
        hemp_text_free(text);

    }
    
    if (hemp_val_elem(args)) {
        hemp_debug("sub has params\n");
    }

    return hemp_str_val("TODO: hemp_element_sub_value()");
}
