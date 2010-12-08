#include <hemp/language/hemp.h>


/*--------------------------------------------------------------------------
 * static tables defining symbol collections
 *--------------------------------------------------------------------------*/

static struct hemp_symbols hemp_symbols_hemp_core[] = {
    { "hemp.text",              &hemp_element_text_symbol               },
    { "hemp.space",             &hemp_element_space_symbol              },
    { "hemp.comment",           &hemp_element_comment_symbol            },
    { "hemp.tag_start",         &hemp_element_tag_start_symbol          },
    { "hemp.tag_end",           &hemp_element_tag_end_symbol            },
    { "hemp.delimiter",         &hemp_element_delimiter_symbol          },
    { "hemp.separator",         &hemp_element_separator_symbol          },
    { "hemp.terminator",        &hemp_element_terminator_symbol         },
    { "hemp.number",            &hemp_element_number_symbol             },
    { "hemp.integer",           &hemp_element_integer_symbol            },
    { "hemp.word",              &hemp_element_word_symbol               },
    { "hemp.squote",            &hemp_element_squote_symbol             },
    { "hemp.dquote",            &hemp_element_dquote_symbol             },
    { "hemp.block",             &hemp_element_block_symbol              },
    { "hemp.apply",             &hemp_element_apply_symbol              },
    { "hemp.dotop",             &hemp_element_dotop_symbol              },
    { "hemp.eof",               &hemp_element_eof_symbol                },
    { NULL, NULL },
};

static struct hemp_symbols hemp_symbols_hemp_identity[] = {
    { "hemp.identity.missing",  &hemp_element_identity_missing          },
    { "hemp.identity.nothing",  &hemp_element_identity_nothing          },
    { "hemp.identity.empty",    &hemp_element_identity_empty            },
    { "hemp.identity.true",     &hemp_element_identity_true             },
    { "hemp.identity.false",    &hemp_element_identity_false            },
    { "hemp.identity.before",   &hemp_element_identity_before           },
    { "hemp.identity.after",    &hemp_element_identity_after            },
    { "hemp.identity.equal",    &hemp_element_identity_equal            },
    { NULL, NULL },
};

//static struct hemp_symbols hemp_symbols_hemp_autoload[] = {
//    { "hemp.bracket.*",         &hemp_element_bracket_symbols           },
//    { "hemp.boolean.*",         &hemp_element_boolean_symbols           },
//    { "hemp.number.*",          &hemp_element_number_symbols            },
//    { "hemp.text.*",            &hemp_element_text_symbols              },
//    { "hemp.operator.assign.*", &hemp_element_assign_symbols            },
//};

static struct hemp_symbols hemp_symbols_hemp_bracket[] = {
//    { "hemp.bracket.parens",    &hemp_element_parens_symbol             },
//    { "hemp.bracket.end_parens",&hemp_element_terminator_symbol         },
    { "hemp.bracket.parens",    &hemp_element_parens_symbol             },
    { "hemp.bracket.list",      &hemp_element_list_symbol               },
    { "hemp.bracket.hash",      &hemp_element_hash_symbol               },
    { NULL, NULL },
};

static struct hemp_symbols hemp_symbols_hemp_boolean[] = {
    { "hemp.boolean.not",       &hemp_element_boolean_not_symbol        },
    { "hemp.boolean.and",       &hemp_element_boolean_and_symbol        },
    { "hemp.boolean.or",        &hemp_element_boolean_or_symbol         },
    { NULL, NULL },
};

static struct hemp_symbols hemp_symbols_hemp_number[] = {
    { "hemp.number.autoinc",    &hemp_element_number_autoinc_symbol     },
    { "hemp.number.autodec",    &hemp_element_number_autodec_symbol     },
    { "hemp.number.plus",       &hemp_element_number_plus_symbol        },
    { "hemp.number.minus",      &hemp_element_number_minus_symbol       },
    { "hemp.number.power",      &hemp_element_number_power_symbol       },
    { "hemp.number.multiply",   &hemp_element_number_multiply_symbol    },
    { "hemp.number.divide",     &hemp_element_number_divide_symbol      },
    { "hemp.number.divint",     &hemp_element_number_divint_symbol      },
    { "hemp.number.modulus",    &hemp_element_number_modulus_symbol     },
    { "hemp.number.compare",    &hemp_element_number_compare_symbol     },
    { "hemp.number.equal",      &hemp_element_number_equal_symbol       },
    { "hemp.number.not_equal",  &hemp_element_number_not_equal_symbol   },
    { "hemp.number.before",     &hemp_element_number_before_symbol      },
    { "hemp.number.not_before", &hemp_element_number_not_before_symbol  },
    { "hemp.number.after",      &hemp_element_number_after_symbol       },
    { "hemp.number.not_after",  &hemp_element_number_not_after_symbol   },
    { NULL, NULL },
};

static struct hemp_symbols hemp_symbols_hemp_text[] = {
    { "hemp.text.concat",       &hemp_element_text_concat_symbol        },
    { "hemp.text.compare",      &hemp_element_text_compare_symbol       },
    { "hemp.text.equal",        &hemp_element_text_equal_symbol         },
    { "hemp.text.not_equal",    &hemp_element_text_not_equal_symbol     },
    { "hemp.text.before",       &hemp_element_text_before_symbol        },
    { "hemp.text.not_before",   &hemp_element_text_not_before_symbol    },
    { "hemp.text.after",        &hemp_element_text_after_symbol         },
    { "hemp.text.not_after",    &hemp_element_text_not_after_symbol     },
    { NULL, NULL },
};

static struct hemp_symbols hemp_symbols_hemp_assign[] = {
    { "hemp.operator.assign.equals", &hemp_element_assign_symbol        },
    { NULL, NULL },
};

static struct hemp_grammars hemp_grammars_hemp[] = {
    { "hemp.alpha",             &hemp_grammar_hemp_alpha                },
    { "hemp.bravo",             &hemp_grammar_hemp_alpha                },
    { NULL, NULL },
};

static struct hemp_tags hemp_tags_hemp[] = {
    { "hemp.inline",            &hemp_tag_inline                        },
    { "hemp.outline",           &hemp_tag_outline                       },
    { "hemp.comment",           &hemp_tag_comment                       },
    { "hemp.control",           &hemp_tag_control                       },
    { "hemp.unplugged",         &hemp_tag_unplugged                     },
    { NULL, NULL },
};

/*--------------------------------------------------------------------------
 * hemp language initialisation
 *--------------------------------------------------------------------------*/

HEMP_LANGUAGE(hemp_language_hemp_new) {
    hemp_debug_call("hemp_language_hemp_new(%p, %s)\n", hemp, name);

    hemp_language language = hemp_language_new(
        hemp, name, HEMP_LANGUAGE_HEMP_VERSION
    );

    /* register all the basic symbols */
    hemp_register_elements(hemp, hemp_symbols_hemp_core);

//    hemp_register_elements(hemp, hemp_symbols_hemp_autoload);

    /* register factories for bracket, boolean, number and text operator symbols */
    hemp_register_element(hemp, "hemp.operator.assign.*",  &hemp_element_assign_symbols);
    hemp_register_element(hemp, "hemp.identity.*",         &hemp_element_identity_symbols);
    hemp_register_element(hemp, "hemp.bracket.*",          &hemp_element_bracket_symbols);
    hemp_register_element(hemp, "hemp.boolean.*",          &hemp_element_boolean_symbols);
    hemp_register_element(hemp, "hemp.number.*",           &hemp_element_number_symbols);
    hemp_register_element(hemp, "hemp.text.*",             &hemp_element_text_symbols);

    hemp_register_grammars(hemp, hemp_grammars_hemp);
    hemp_register_tags(hemp, hemp_tags_hemp);

    return language;
}


/*--------------------------------------------------------------------------
 * identity elements
 *--------------------------------------------------------------------------*/

HEMP_SYMBOLS(hemp_element_identity_symbols) {
    /* we should detect if we've done this already and skip it */
    hemp_register_elements(
        hemp, hemp_symbols_hemp_identity
    );

    /* now try again */
    return (hemp_action) hemp_hash_fetch_pointer(
        hemp->element->constructors, name
    );
}


/*--------------------------------------------------------------------------
 * bracket elements
 *--------------------------------------------------------------------------*/

HEMP_SYMBOLS(hemp_element_bracket_symbols) {
    /* we should detect if we've done this already and skip it */
    hemp_register_elements(
        hemp, hemp_symbols_hemp_bracket
    );

    /* now try again */
    return (hemp_action) hemp_hash_fetch_pointer(
        hemp->element->constructors, name
    );
}


/*--------------------------------------------------------------------------
 * boolean operator elements
 *--------------------------------------------------------------------------*/

HEMP_SYMBOLS(hemp_element_boolean_symbols) {
    /* we should detect if we've done this already and skip it */
    hemp_register_elements(
        hemp, hemp_symbols_hemp_boolean
    );

    /* now try again */
    return (hemp_action) hemp_hash_fetch_pointer(
        hemp->element->constructors, name
    );
}


/*--------------------------------------------------------------------------
 * number operator elements
 *--------------------------------------------------------------------------*/

HEMP_SYMBOLS(hemp_element_number_symbols) {
    /* we should detect if we've done this already and skip it */
    hemp_register_elements(
        hemp, hemp_symbols_hemp_number
    );

    /* now try again */
    return (hemp_action) hemp_hash_fetch_pointer(
        hemp->element->constructors, name
    );
}


/*--------------------------------------------------------------------------
 * text operator elements
 *--------------------------------------------------------------------------*/

HEMP_SYMBOLS(hemp_element_text_symbols) {
    /* we should detect if we've done this already and skip it */
    hemp_register_elements(
        hemp, hemp_symbols_hemp_text
    );

    /* now try again */
    return (hemp_action) hemp_hash_fetch_pointer(
        hemp->element->constructors, name
    );
}


/*--------------------------------------------------------------------------
 * assignment operator elements
 *--------------------------------------------------------------------------*/

HEMP_SYMBOLS(hemp_element_assign_symbols) {
    /* I'm getting tired of this... should have some way to specify the 
     * symbol set instead of a function... oh well it'll do for now...
     */
    hemp_register_elements(
        hemp, hemp_symbols_hemp_assign
    );

    return (hemp_action) hemp_hash_fetch_pointer(
        hemp->element->constructors, name
    );
}


/*--------------------------------------------------------------------------
 * grammars
 *--------------------------------------------------------------------------*/

HEMP_GRAMMAR(hemp_grammar_hemp_alpha) {
    hemp_debug_call("hemp_grammar_hemp_alpha(%p, %s)\n", hemp, name);
    hemp_grammar grammar = (hemp_grammar) hemp_grammar_new(hemp, name);
    hemp_grammar_add_hemp_alpha(grammar);
    return grammar;
}


HEMP_GRAMMAR(hemp_grammar_hemp_bravo) {
    hemp_debug_call("hemp_grammar_hemp_bravo(%p, %s)\n", hemp, name);
    hemp_grammar grammar = hemp_grammar_hemp_alpha(hemp, name);
    hemp_grammar_add_hemp_bravo(grammar);
    return grammar;
}

HEMP_GRAMMAR(hemp_grammar_hemp_charlie) {
    hemp_debug_call("hemp_grammar_hemp_charlie(%p, %s)\n", hemp, name);
    hemp_grammar grammar = hemp_grammar_hemp_bravo(hemp, name);
    hemp_grammar_add_hemp_charlie(grammar);
    return grammar;
}


void
hemp_grammar_add_hemp_alpha(
    hemp_grammar grammar
) {
    hemp_debug_call("hemp_grammar_add_hemp_alpha(%p)\n", grammar);
    HEMP_SYMBOL0("hemp.text");
    HEMP_SYMBOL0("hemp.space");
    HEMP_SYMBOL0("hemp.block");
    HEMP_SYMBOL1("hemp.comment", "#"        );
    HEMP_SYMBOL2("hemp.squote",  "'",  "'"  );
    HEMP_SYMBOL2("hemp.dquote",  "\"", "\"" );
}


void
hemp_grammar_add_hemp_bravo(
    hemp_grammar grammar
) {
    hemp_debug_call("hemp_grammar_add_hemp_bravo(%p)\n", grammar);

//    [ '$'       => sig_item         =>   0, 350 ],      # $foo
//    [ '@'       => sig_list         =>   0, 350 ],      # @foo
//    [ '%'       => sig_hash         =>   0, 350 ],      # %foo
//    [ '.'       => op_dot           => 340,   0 ],      # foo.bar

    HEMP_SYMBOL0("hemp.apply");

    HEMP_OPERATOR1("hemp.dotop",                ".",        200,  200);

//  HEMP_OPERATOR1("hemp.number.autoinc",       "++",       195,  195);
//  HEMP_OPERATOR1("hemp.number.autodec",       "--",       195,  195);
    HEMP_OPERATOR1("hemp.number.power",         "**",       190,    0);
    HEMP_OPERATOR1("hemp.number.plus",          "+",        175,  185);
    HEMP_OPERATOR1("hemp.number.minus",         "-",        175,  185);
    HEMP_OPERATOR1("hemp.number.multiply",      "*",        180,    0);
    HEMP_OPERATOR1("hemp.number.divide",        "/",        180,    0);
    HEMP_OPERATOR1("hemp.number.modulus",       "%",        180,    0);
    HEMP_OPERATOR1("hemp.number.divint",        "/i",       180,    0);

    HEMP_OPERATOR1("hemp.text.concat",          "~",        170,  170);
                
    HEMP_OPERATOR1("hemp.number.compare",       "<=>",      160,    0);
    HEMP_OPERATOR1("hemp.number.equal",         "==",       160,    0);
    HEMP_OPERATOR1("hemp.number.not_equal",     "!=",       160,    0);
    HEMP_OPERATOR1("hemp.number.before",        "<",        160,    0);
    HEMP_OPERATOR1("hemp.number.not_before",    ">=",       160,    0);
    HEMP_OPERATOR1("hemp.number.after",         ">",        160,    0);
    HEMP_OPERATOR1("hemp.number.not_after",     "<=",       160,    0);

    HEMP_OPERATOR1("hemp.text.compare",         "cmp",      160,    0);
    HEMP_OPERATOR1("hemp.text.equal",           "eq",       160,    0);
    HEMP_OPERATOR1("hemp.text.not_equal",       "ne",       160,    0);
    HEMP_OPERATOR1("hemp.text.before",          "lt",       160,    0);
    HEMP_OPERATOR1("hemp.text.not_before",      "ge",       160,    0);
    HEMP_OPERATOR1("hemp.text.after",           "gt",       160,    0);
    HEMP_OPERATOR1("hemp.text.not_after",       "le",       160,    0);

    HEMP_OPERATOR1("hemp.boolean.not",          "!",          0,  185);
    HEMP_OPERATOR1("hemp.boolean.and",          "&&",       155,    0);
    HEMP_OPERATOR1("hemp.boolean.or",           "||",       150,    0);
//  HEMP_OPERATOR1("hemp.boolean.nor",          "!!",       250,    0);

//    [ '..'      => op_range         => 240,   0 ],      # 1 .. 91
//    [ 'to'      => op_range         => 240,   0 ],      # 1 to 91 by 10      # TODO: to
//    [ 'by'      => op_step          => 240,   0 ],      # 1 to 91 by 10      # TODO: by
//    
//    [ '?'       => op_ternary       => 230,   0 ],      # foo ? bar 
//
//    # this used to be above ? : but I think it's better here so that 
//    # something like a -> a > 10 ? "big" : "small" is parsed as 
//    # a -> ((a > 10) ? "big" : "small")
//    [ '->'      => arrow            => 230,   0 ],      # a -> a + 1
//

    HEMP_OPERATOR1("hemp.boolean.not",          "not",        0,  120);
    HEMP_OPERATOR1("hemp.boolean.and",          "and",      115,    0);
    HEMP_OPERATOR1("hemp.boolean.or",           "or",       110,    0);
//  HEMP_OPERATOR1("hemp.boolean.nor",          "nor",      110,    0);

//    # grouping constructs...
    HEMP_OPERATOR2("hemp.bracket.parens",       "(", ")",     0,    0);
    HEMP_OPERATOR2("hemp.bracket.list",         "[", "]",     0,    0);
    HEMP_OPERATOR2("hemp.bracket.hash",         "{", "}",     0,    0);
    HEMP_OPERATOR1("hemp.terminator",           ")",          0,    0);
    HEMP_OPERATOR1("hemp.terminator",           "]",          0,    0);
    HEMP_OPERATOR1("hemp.terminator",           "}",          0,    0);

    HEMP_OPERATOR1("hemp.delimiter",            ",",          0,    0);
    HEMP_OPERATOR1("hemp.separator",            ";",          0,    0);

//    # Other punctuation marks
//    [ ':'       => terminator       =>   0,   0 ],
//    [ 'in'      => terminator       =>   0,   0 ],
//    [ '#'       => fragment         =>   0,   0 ],
//    
//    # One token to end them all and in the darkness bind them
//    [ 'end'     => end              =>   0,   0 ],
}


void
hemp_grammar_add_hemp_charlie(
    hemp_grammar grammar
) {
    hemp_debug_call("hemp_grammar_add_hemp_charlie(%p)\n", grammar);

    HEMP_OPERATOR1("hemp.number.autoinc",       "++",       195,  195);
    HEMP_OPERATOR1("hemp.number.autodec",       "--",       195,  195);

    /* binary assignment operators */
    HEMP_OPERATOR1("hemp.operator.assign.equals",  "=",      0,  130);
//    [ '='       => op_assign        => 200,   0 ],      # foo = bar

//    [ '=>'      => op_pair          => 200,   0 ],      # foo => bar
//    [ '~='      => txt_combine_set  => 200,   0 ],      # foo ~= bar
//    [ '+='      => num_add_set      => 200,   0 ],      # foo += bar
//    [ '-='      => num_sub_set      => 200,   0 ],      # foo -= bar
//    [ '*='      => num_mul_set      => 200,   0 ],      # foo *= bar
//    [ '/='      => num_div_set      => 200,   0 ],      # foo /= bar
//    [ '&&='     => bool_and_set     => 200,   0 ],      # foo &&= bar
//    [ '||='     => bool_or_set      => 200,   0 ],      # foo ||= bar
//    [ '!!='     => bool_nor_set     => 200,   0 ],      # foo !!= bar

}
