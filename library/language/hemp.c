#include <hemp/language/hemp.h>


/*--------------------------------------------------------------------------
 * static tables defining symbol collections
 *--------------------------------------------------------------------------*/

static struct hemp_elements hemp_elements_hemp_core[] = {
    { "hemp.text",                  &hemp_element_text                  },
    { "hemp.space",                 &hemp_element_space                 },
    { "hemp.comment",               &hemp_element_comment               },
    { "hemp.tag_start",             &hemp_element_tag_start             },
    { "hemp.tag_end",               &hemp_element_tag_end               },
    { "hemp.delimiter",             &hemp_element_delimiter             },
    { "hemp.separator",             &hemp_element_separator             },
    { "hemp.terminator",            &hemp_element_terminator            },
    { "hemp.number",                &hemp_element_number                },
    { "hemp.integer",               &hemp_element_integer               },
    { "hemp.word",                  &hemp_element_word                  },
    { "hemp.squote",                &hemp_element_squote                },
    { "hemp.dquote",                &hemp_element_dquote                },
    { "hemp.block",                 &hemp_element_block                 },
    { "hemp.apply",                 &hemp_element_apply                 },
    { "hemp.dotop",                 &hemp_element_dotop                 },
    { "hemp.eof",                   &hemp_element_eof                   },
    { NULL, NULL },
};

static struct hemp_elements hemp_elements_hemp_identity[] = {
    { "hemp.identity.missing",      &hemp_element_identity_missing      },
    { "hemp.identity.nothing",      &hemp_element_identity_nothing      },
    { "hemp.identity.empty",        &hemp_element_identity_empty        },
    { "hemp.identity.true",         &hemp_element_identity_true         },
    { "hemp.identity.false",        &hemp_element_identity_false        },
    { "hemp.identity.before",       &hemp_element_identity_before       },
    { "hemp.identity.after",        &hemp_element_identity_after        },
    { "hemp.identity.equal",        &hemp_element_identity_equal        },
    { NULL, NULL },
};

//static struct hemp_elements hemp_elements_hemp_autoload[] = {
//  { "hemp.bracket.*",             &hemp_element_brackets              },
//  { "hemp.boolean.*",             &hemp_element_booleans              },
//  { "hemp.number.*",              &hemp_element_numbers               },
//  { "hemp.text.*",                &hemp_element_texts                 },
//  { "hemp.operator.assign.*",     &hemp_element_assigns               },
//};

static struct hemp_elements hemp_elements_hemp_bracket[] = {
    { "hemp.bracket.parens",        &hemp_element_parens                },
    { "hemp.bracket.list",          &hemp_element_list                  },
    { "hemp.bracket.hash",          &hemp_element_hash                  },
    { NULL, NULL },
};

static struct hemp_elements hemp_elements_hemp_boolean[] = {
    { "hemp.boolean.not",           &hemp_element_boolean_not           },
    { "hemp.boolean.and",           &hemp_element_boolean_and           },
    { "hemp.boolean.or",            &hemp_element_boolean_or            },
    { NULL, NULL },
};

static struct hemp_elements hemp_elements_hemp_number[] = {
    { "hemp.number.autoinc",        &hemp_element_number_autoinc        },
    { "hemp.number.autodec",        &hemp_element_number_autodec        },
    { "hemp.number.plus",           &hemp_element_number_plus           },
    { "hemp.number.minus",          &hemp_element_number_minus          },
    { "hemp.number.power",          &hemp_element_number_power          },
    { "hemp.number.multiply",       &hemp_element_number_multiply       },
    { "hemp.number.divide",         &hemp_element_number_divide         },
    { "hemp.number.divint",         &hemp_element_number_divint         },
    { "hemp.number.modulus",        &hemp_element_number_modulus        },
    { "hemp.number.compare",        &hemp_element_number_compare        },
    { "hemp.number.equal",          &hemp_element_number_equal          },
    { "hemp.number.not_equal",      &hemp_element_number_not_equal      },
    { "hemp.number.before",         &hemp_element_number_before         },
    { "hemp.number.not_before",     &hemp_element_number_not_before     },
    { "hemp.number.after",          &hemp_element_number_after          },
    { "hemp.number.not_after",      &hemp_element_number_not_after      },
    { NULL, NULL },
};

static struct hemp_elements hemp_elements_hemp_text[] = {
    { "hemp.text.concat",           &hemp_element_text_concat           },
    { "hemp.text.compare",          &hemp_element_text_compare          },
    { "hemp.text.equal",            &hemp_element_text_equal            },
    { "hemp.text.not_equal",        &hemp_element_text_not_equal        },
    { "hemp.text.before",           &hemp_element_text_before           },
    { "hemp.text.not_before",       &hemp_element_text_not_before       },
    { "hemp.text.after",            &hemp_element_text_after            },
    { "hemp.text.not_after",        &hemp_element_text_not_after        },
    { NULL, NULL },
};

static struct hemp_elements hemp_elements_hemp_assign[] = {
    { "hemp.operator.assign.equals", &hemp_element_assign               },
    { NULL, NULL },
};

static struct hemp_grammars hemp_grammars_hemp[] = {
    { "hemp.alpha",                 &hemp_grammar_hemp_alpha            },
    { "hemp.bravo",                 &hemp_grammar_hemp_alpha            },
    { NULL, NULL },
};

static struct hemp_tags hemp_tags_hemp[] = {
    { "hemp.inline",                &hemp_tag_inline                    },
    { "hemp.outline",               &hemp_tag_outline                   },
    { "hemp.comment",               &hemp_tag_comment                   },
    { "hemp.control",               &hemp_tag_control                   },
    { "hemp.unplugged",             &hemp_tag_unplugged                 },
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
    hemp_register_elements(hemp, hemp_elements_hemp_core);

//    hemp_register_elements(hemp, hemp_elements_hemp_autoload);

    /* register factories for bracket, boolean, number and text operator symbols */
    hemp_register_element(hemp, "hemp.operator.assign.*",  &hemp_element_assign_elements);
    hemp_register_element(hemp, "hemp.identity.*",         &hemp_element_identity_elements);
    hemp_register_element(hemp, "hemp.bracket.*",          &hemp_element_bracket_elements);
    hemp_register_element(hemp, "hemp.boolean.*",          &hemp_element_boolean_elements);
    hemp_register_element(hemp, "hemp.number.*",           &hemp_element_number_elements);
    hemp_register_element(hemp, "hemp.text.*",             &hemp_element_text_elements);

    hemp_register_element(hemp, "hemp.tag.inline",         &hemp_element_tag_inline);

    hemp_register_grammars(hemp, hemp_grammars_hemp);
    hemp_register_tags(hemp, hemp_tags_hemp);

    return language;
}


/*--------------------------------------------------------------------------
 * identity elements
 *--------------------------------------------------------------------------*/

HEMP_ELEMENTS(hemp_element_identity_elements) {
    /* we should detect if we've done this already and skip it */
    hemp_register_elements(
        hemp, hemp_elements_hemp_identity
    );

    /* now try again */
    return (hemp_action) hemp_hash_fetch_pointer(
        hemp->element->constructors, name
    );
}


/*--------------------------------------------------------------------------
 * bracket elements
 *--------------------------------------------------------------------------*/

HEMP_ELEMENTS(hemp_element_bracket_elements) {
    /* we should detect if we've done this already and skip it */
    hemp_register_elements(
        hemp, hemp_elements_hemp_bracket
    );

    /* now try again */
    return (hemp_action) hemp_hash_fetch_pointer(
        hemp->element->constructors, name
    );
}


/*--------------------------------------------------------------------------
 * boolean operator elements
 *--------------------------------------------------------------------------*/

HEMP_ELEMENTS(hemp_element_boolean_elements) {
    /* we should detect if we've done this already and skip it */
    hemp_register_elements(
        hemp, hemp_elements_hemp_boolean
    );

    /* now try again */
    return (hemp_action) hemp_hash_fetch_pointer(
        hemp->element->constructors, name
    );
}


/*--------------------------------------------------------------------------
 * number operator elements
 *--------------------------------------------------------------------------*/

HEMP_ELEMENTS(hemp_element_number_elements) {
    /* we should detect if we've done this already and skip it */
    hemp_register_elements(
        hemp, hemp_elements_hemp_number
    );

    /* now try again */
    return (hemp_action) hemp_hash_fetch_pointer(
        hemp->element->constructors, name
    );
}


/*--------------------------------------------------------------------------
 * text operator elements
 *--------------------------------------------------------------------------*/

HEMP_ELEMENTS(hemp_element_text_elements) {
    /* we should detect if we've done this already and skip it */
    hemp_register_elements(
        hemp, hemp_elements_hemp_text
    );

    /* now try again */
    return (hemp_action) hemp_hash_fetch_pointer(
        hemp->element->constructors, name
    );
}


/*--------------------------------------------------------------------------
 * assignment operator elements
 *--------------------------------------------------------------------------*/

HEMP_ELEMENTS(hemp_element_assign_elements) {
    /* I'm getting tired of this... should have some way to specify the 
     * symbol set instead of a function... oh well it'll do for now...
     */
    hemp_register_elements(
        hemp, hemp_elements_hemp_assign
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
    HEMP_USE_ELEMENT0("hemp.text");
    HEMP_USE_ELEMENT0("hemp.space");
    HEMP_USE_ELEMENT0("hemp.block");
    HEMP_USE_ELEMENT1("hemp.comment", "#"        );
    HEMP_USE_ELEMENT2("hemp.squote",  "'",  "'"  );
    HEMP_USE_ELEMENT2("hemp.dquote",  "\"", "\"" );
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

    HEMP_USE_ELEMENT0("hemp.apply");

    HEMP_USE_OPERATOR1("hemp.dotop",                ".",        200,  200);

//  HEMP_USE_OPERATOR1("hemp.number.autoinc",       "++",       195,  195);
//  HEMP_USE_OPERATOR1("hemp.number.autodec",       "--",       195,  195);
    HEMP_USE_OPERATOR1("hemp.number.power",         "**",       190,    0);
    HEMP_USE_OPERATOR1("hemp.number.plus",          "+",        175,  185);
    HEMP_USE_OPERATOR1("hemp.number.minus",         "-",        175,  185);
    HEMP_USE_OPERATOR1("hemp.number.multiply",      "*",        180,    0);
    HEMP_USE_OPERATOR1("hemp.number.divide",        "/",        180,    0);
    HEMP_USE_OPERATOR1("hemp.number.modulus",       "%",        180,    0);
    HEMP_USE_OPERATOR1("hemp.number.divint",        "/i",       180,    0);

    HEMP_USE_OPERATOR1("hemp.text.concat",          "~",        170,  170);
                
    HEMP_USE_OPERATOR1("hemp.number.compare",       "<=>",      160,    0);
    HEMP_USE_OPERATOR1("hemp.number.equal",         "==",       160,    0);
    HEMP_USE_OPERATOR1("hemp.number.not_equal",     "!=",       160,    0);
    HEMP_USE_OPERATOR1("hemp.number.before",        "<",        160,    0);
    HEMP_USE_OPERATOR1("hemp.number.not_before",    ">=",       160,    0);
    HEMP_USE_OPERATOR1("hemp.number.after",         ">",        160,    0);
    HEMP_USE_OPERATOR1("hemp.number.not_after",     "<=",       160,    0);

    HEMP_USE_OPERATOR1("hemp.text.compare",         "cmp",      160,    0);
    HEMP_USE_OPERATOR1("hemp.text.equal",           "eq",       160,    0);
    HEMP_USE_OPERATOR1("hemp.text.not_equal",       "ne",       160,    0);
    HEMP_USE_OPERATOR1("hemp.text.before",          "lt",       160,    0);
    HEMP_USE_OPERATOR1("hemp.text.not_before",      "ge",       160,    0);
    HEMP_USE_OPERATOR1("hemp.text.after",           "gt",       160,    0);
    HEMP_USE_OPERATOR1("hemp.text.not_after",       "le",       160,    0);

    HEMP_USE_OPERATOR1("hemp.boolean.not",          "!",          0,  185);
    HEMP_USE_OPERATOR1("hemp.boolean.and",          "&&",       155,    0);
    HEMP_USE_OPERATOR1("hemp.boolean.or",           "||",       150,    0);
//  HEMP_USE_OPERATOR1("hemp.boolean.nor",          "!!",       250,    0);

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

    HEMP_USE_OPERATOR1("hemp.boolean.not",          "not",        0,  120);
    HEMP_USE_OPERATOR1("hemp.boolean.and",          "and",      115,    0);
    HEMP_USE_OPERATOR1("hemp.boolean.or",           "or",       110,    0);
//  HEMP_USE_OPERATOR1("hemp.boolean.nor",          "nor",      110,    0);

//    # grouping constructs...
    HEMP_USE_OPERATOR2("hemp.bracket.parens",       "(", ")",     0,    0);
    HEMP_USE_OPERATOR2("hemp.bracket.list",         "[", "]",     0,    0);
    HEMP_USE_OPERATOR2("hemp.bracket.hash",         "{", "}",     0,    0);
    HEMP_USE_OPERATOR1("hemp.terminator",           ")",          0,    0);
    HEMP_USE_OPERATOR1("hemp.terminator",           "]",          0,    0);
    HEMP_USE_OPERATOR1("hemp.terminator",           "}",          0,    0);

    HEMP_USE_OPERATOR1("hemp.delimiter",            ",",          0,    0);
    HEMP_USE_OPERATOR1("hemp.separator",            ";",          0,    0);

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

    HEMP_USE_OPERATOR1("hemp.number.autoinc",       "++",       195,  195);
    HEMP_USE_OPERATOR1("hemp.number.autodec",       "--",       195,  195);

    /* binary assignment operators */
    HEMP_USE_OPERATOR1("hemp.operator.assign.equals",  "=",      0,  130);
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
