#include <hemp.h>

#define HEMP_LANGUAGE_VERSION 0.01

HEMP_LANGUAGE_FUNC(hemp_language_hemp_init);


/* basic element prototypes   TODO: move to element.h */
HEMP_ELEMENT_FUNC(hemp_element_text_symbol);
HEMP_ELEMENT_FUNC(hemp_element_space_symbol);
//HEMP_ELEMENT_FUNC(hemp_element_comment_symbol);
HEMP_ELEMENT_FUNC(hemp_element_tag_start_symbol);
HEMP_ELEMENT_FUNC(hemp_element_tag_end_symbol);
HEMP_ELEMENT_FUNC(hemp_element_number_symbol);
HEMP_ELEMENT_FUNC(hemp_element_integer_symbol);
HEMP_ELEMENT_FUNC(hemp_element_word_symbol);
HEMP_ELEMENT_FUNC(hemp_element_squote_symbol);
HEMP_ELEMENT_FUNC(hemp_element_dquote_symbol);
HEMP_ELEMENT_FUNC(hemp_element_block_symbol);
HEMP_ELEMENT_FUNC(hemp_element_eof_symbol);

/* boolean operator prototypes */
HEMP_SYMBOLS_FUNC(hemp_element_boolean_symbols);

/* number operator prototypes */
HEMP_SYMBOLS_FUNC(hemp_element_number_symbols);
HEMP_ELEMENT_FUNC(hemp_element_number_inc_symbol);
HEMP_ELEMENT_FUNC(hemp_element_number_dec_symbol);

/* text operator prototypes */
HEMP_SYMBOLS_FUNC(hemp_element_text_symbols);



/* This is the rather slow and tedious way originally implemented to get
 * something working.  The symbol constructors are registered as elements
 * with hemp lookup is by name via the hemp elements factory.
 */

static struct hemp_symbols_s hemp_symbols_hemp[] = {
    { "hemp.text",              &hemp_element_text_symbol               },
    { "hemp.space",             &hemp_element_space_symbol              },
    { "hemp.comment",           &hemp_element_comment_symbol            },
    { "hemp.tag_start",         &hemp_element_tag_start_symbol          },
    { "hemp.tag_end",           &hemp_element_tag_end_symbol            },
    { "hemp.number",            &hemp_element_number_symbol             },
    { "hemp.integer",           &hemp_element_integer_symbol            },
    { "hemp.word",              &hemp_element_word_symbol               },
    { "hemp.squote",            &hemp_element_squote_symbol             },
    { "hemp.dquote",            &hemp_element_dquote_symbol             },
    { "hemp.block",             &hemp_element_block_symbol              },
    { "hemp.eof",               &hemp_element_eof_symbol                },
    { NULL, NULL },
};

static struct hemp_symbols_s hemp_symbols_hemp_boolean[] = {
    { "hemp.boolean.not",       &hemp_element_boolean_not_symbol        },
    { "hemp.boolean.and",       &hemp_element_boolean_and_symbol        },
    { "hemp.boolean.or",        &hemp_element_boolean_or_symbol         },
    { NULL, NULL },
};

static struct hemp_symbols_s hemp_symbols_hemp_number[] = {
    { "hemp.number.inc",         &hemp_element_number_inc_symbol          },
    { "hemp.number.dec",         &hemp_element_number_dec_symbol          },
    { "hemp.number.plus",        &hemp_element_number_plus_symbol         },
    { "hemp.number.minus",       &hemp_element_number_minus_symbol        },
    { "hemp.number.power",       &hemp_element_number_power_symbol        },
    { "hemp.number.multiply",    &hemp_element_number_multiply_symbol     },
    { "hemp.number.divide",      &hemp_element_number_divide_symbol       },
    { "hemp.number.divint",      &hemp_element_number_divint_symbol       },
    { "hemp.number.modulus",     &hemp_element_number_modulus_symbol      },
    { "hemp.number.compare",     &hemp_element_number_compare_symbol      },
    { "hemp.number.equal",       &hemp_element_number_equal_symbol        },
    { "hemp.number.not_equal",   &hemp_element_number_not_equal_symbol    },
    { "hemp.number.before",      &hemp_element_number_before_symbol       },
    { "hemp.number.not_before",  &hemp_element_number_not_before_symbol   },
    { "hemp.number.after",       &hemp_element_number_after_symbol        },
    { "hemp.number.not_after",   &hemp_element_number_not_after_symbol    },
    { NULL, NULL },
};

static struct hemp_symbols_s hemp_symbols_hemp_text[] = {
    { "hemp.text.concat",     &hemp_element_text_concat_symbol      },
    { "hemp.text.compare",    &hemp_element_text_compare_symbol     },
    { "hemp.text.equal",      &hemp_element_text_equal_symbol       },
    { "hemp.text.not_equal",  &hemp_element_text_not_equal_symbol   },
    { "hemp.text.before",     &hemp_element_text_before_symbol      },
    { "hemp.text.not_before", &hemp_element_text_not_before_symbol  },
    { "hemp.text.after",      &hemp_element_text_after_symbol       },
    { "hemp.text.not_after",  &hemp_element_text_not_after_symbol   },
    { NULL, NULL },
};


hemp_grammar_p hemp_grammar_hemp_alpha(hemp_p, hemp_cstr_p);
hemp_grammar_p hemp_grammar_hemp_bravo(hemp_p, hemp_cstr_p);
void           hemp_grammar_add_hemp_alpha(hemp_grammar_p);
void           hemp_grammar_add_hemp_bravo(hemp_grammar_p);


/* When debugging is turned off, the TODO function return NULL.  It appears 
 * to be the result of a compiler optimisation gone awry.  I tried adding
 * __attribute__ ((noinline)) but it didn't help.  Adding asm("") appears to
 * do the trick. See http://tinyurl.com/279zm6a
 * This is only a temporary measure anyway, so don't sweat it.
 */

#define DONT_OPTIMISE_ME_AWAY  asm("");


HEMP_LANGUAGE_FUNC(hemp_language_hemp_init) {
    debug_call("hemp_language_hemp_init(%p, %s)\n", hemp, name);

    hemp_language_p language = hemp_language_init(
        hemp, name, HEMP_LANGUAGE_VERSION
    );

    /* register all the basic symbols */
    HEMP_ELEMENTS(hemp_symbols_hemp);

    /* register handlers for boolean, number and text operator symbols */
    HEMP_ELEMENT("hemp.boolean.*", &hemp_element_boolean_symbols);
    HEMP_ELEMENT("hemp.number.*",  &hemp_element_number_symbols);
    HEMP_ELEMENT("hemp.text.*", &hemp_element_text_symbols);

    /* register grammars */
    HEMP_GRAMMAR("hemp.alpha", &hemp_grammar_hemp_alpha);
    HEMP_GRAMMAR("hemp.bravo", &hemp_grammar_hemp_bravo);

    return language;
}



/*--------------------------------------------------------------------------
 * boolean operator elements
 *--------------------------------------------------------------------------*/

hemp_action_p
hemp_element_boolean_symbols(
    hemp_p      hemp,
    hemp_cstr_p name
) {
    /* we should detect if we've done this already and skip it */
    HEMP_ELEMENTS(hemp_symbols_hemp_boolean);

    /* now try again */
    return (hemp_action_p) hemp_hash_fetch(
        hemp->elements->constructors, name
    );
}




/*--------------------------------------------------------------------------
 * number operator elements
 *--------------------------------------------------------------------------*/

hemp_action_p
hemp_element_number_symbols(
    hemp_p      hemp,
    hemp_cstr_p name
) {
//  debug("initialising hemp number operator symbols (%s requested)\n", name);

    /* we should detect if we've done this already and skip it */
    HEMP_ELEMENTS(hemp_symbols_hemp_number);

    /* now try again */
    return (hemp_action_p) hemp_hash_fetch(
        hemp->elements->constructors, name
    );
}


HEMP_NO_INLINE hemp_symbol_p
hemp_element_number_TODO_symbol(
    hemp_p        hemp,
    hemp_symbol_p symbol
) {
    debug_todo("number constructor for %s symbol", symbol->name);
    DONT_OPTIMISE_ME_AWAY;
    return symbol;
}


hemp_symbol_p
hemp_element_number_inc_symbol(
    hemp_p        hemp,
    hemp_symbol_p symbol
) {
    return hemp_element_number_TODO_symbol(hemp, symbol);
}


hemp_symbol_p
hemp_element_number_dec_symbol(
    hemp_p        hemp,
    hemp_symbol_p symbol
) {
    return hemp_element_number_TODO_symbol(hemp, symbol);
}




/*--------------------------------------------------------------------------
 * text operator elements
 *--------------------------------------------------------------------------*/

hemp_action_p
hemp_element_text_symbols(
    hemp_p      hemp,
    hemp_cstr_p name
) {
//  debug("initialising hemp text operator symbols (%s requested)\n", name);

    /* we should detect if we've done this already and skip it */
    HEMP_ELEMENTS(hemp_symbols_hemp_text);

    /* now try again */
    return (hemp_action_p) hemp_hash_fetch(
        hemp->elements->constructors, name
    );
}




/*--------------------------------------------------------------------------
 * grammars
 *--------------------------------------------------------------------------*/


hemp_grammar_p
hemp_grammar_hemp_alpha(
    hemp_p      hemp,
    hemp_cstr_p name
) {
    debug_call("hemp_grammar_hemp_alpha(%p, %s)\n", hemp, name);
    hemp_grammar_p grammar = (hemp_grammar_p) hemp_grammar_init(hemp, name);
    hemp_grammar_add_hemp_alpha(grammar);
    return grammar;
}

hemp_grammar_p
hemp_grammar_hemp_bravo(
    hemp_p      hemp,
    hemp_cstr_p name
) {
    debug_call("hemp_grammar_hemp_bravo(%p, %s)\n", hemp, name);
    hemp_grammar_p grammar = (hemp_grammar_p) hemp_grammar_init(hemp, name);
    hemp_grammar_add_hemp_alpha(grammar);
    hemp_grammar_add_hemp_bravo(grammar);
    return grammar;
}


void
hemp_grammar_add_hemp_alpha(
    hemp_grammar_p grammar
) {
    debug_call("hemp_grammar_add_hemp_alpha(%p)\n", grammar);
    HEMP_SYMBOL1("hemp.text",    "_TEXT"    );
    HEMP_SYMBOL1("hemp.space",   "_SPACE"   );
    HEMP_SYMBOL1("hemp.comment", "#"        );
    HEMP_SYMBOL2("hemp.squote",  "'",  "'"  );
    HEMP_SYMBOL2("hemp.dquote",  "\"", "\"" );
}

void
hemp_grammar_add_hemp_bravo(
    hemp_grammar_p grammar
) {
    debug_call("hemp_grammar_add_hemp_bravo(%p)\n", grammar);

//    [ '$'       => sig_item         =>   0, 350 ],      # $foo
//    [ '@'       => sig_list         =>   0, 350 ],      # @foo
//    [ '%'       => sig_hash         =>   0, 350 ],      # %foo
//    [ '.'       => op_dot           => 340,   0 ],      # foo.bar

    HEMP_OPERATOR1("hemp.number.inc",           "++",       295,  295);
    HEMP_OPERATOR1("hemp.number.dec",           "--",       295,  295);
    HEMP_OPERATOR1("hemp.number.power",         "**",       290,    0);
    HEMP_OPERATOR1("hemp.number.plus",          "+",        275,  285);
    HEMP_OPERATOR1("hemp.number.minus",         "-",        275,  285);
    HEMP_OPERATOR1("hemp.number.multiply",      "*",        280,    0);
    HEMP_OPERATOR1("hemp.number.divide",        "/",        280,    0);
    HEMP_OPERATOR1("hemp.number.modulus",       "%",        280,    0);
    HEMP_OPERATOR1("hemp.number.divint",        "/i",       280,    0);

    HEMP_OPERATOR1("hemp.text.concat",          "~",        270,  270);
                
    HEMP_OPERATOR1("hemp.number.compare",       "<=>",      260,    0);
    HEMP_OPERATOR1("hemp.number.equal",         "==",       260,    0);
    HEMP_OPERATOR1("hemp.number.not_equal",     "!=",       260,    0);
    HEMP_OPERATOR1("hemp.number.before",        "<",        260,    0);
    HEMP_OPERATOR1("hemp.number.not_before",    ">=",       260,    0);
    HEMP_OPERATOR1("hemp.number.after",         ">",        260,    0);
    HEMP_OPERATOR1("hemp.number.not_after",     "<=",       260,    0);

    HEMP_OPERATOR1("hemp.text.compare",         "cmp",      260,    0);
    HEMP_OPERATOR1("hemp.text.equal",           "eq",       260,    0);
    HEMP_OPERATOR1("hemp.text.not_equal",       "ne",       260,    0);
    HEMP_OPERATOR1("hemp.text.before",          "lt",       260,    0);
    HEMP_OPERATOR1("hemp.text.not_before",      "ge",       260,    0);
    HEMP_OPERATOR1("hemp.text.after",           "gt",       260,    0);
    HEMP_OPERATOR1("hemp.text.not_after",       "le",       260,    0);

    HEMP_OPERATOR1("hemp.boolean.not",          "!",          0,  285);
    HEMP_OPERATOR1("hemp.boolean.and",          "&&",       255,    0);
    HEMP_OPERATOR1("hemp.boolean.or",           "||",       250,    0);
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
//    # binary assignment operators
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

    HEMP_OPERATOR1("hemp.boolean.not",          "not",        0,  190);
    HEMP_OPERATOR1("hemp.boolean.and",          "and",      180,    0);
    HEMP_OPERATOR1("hemp.boolean.or",           "or",       170,    0);
//  HEMP_OPERATOR1("hemp.boolean.nor",          "nor",      170,    0);

//    # grouping constructs...
//    [ '('       => con_parens       =>   0,   0 ],
//    [ '['       => con_list         =>   0,   0 ],
//    [ '{'       => con_hash         =>   0,   0 ],
//    
//    # ...and their respective terminators
//    [ ')'       => terminator       =>   0,   0 ],
//    [ ']'       => terminator       =>   0,   0 ],
//    [ '}'       => terminator       =>   0,   0 ],
//
//    # Other punctuation marks
//    [ ','       => separator        =>   0,   0 ],
//    [ ';'       => delimiter        =>   0,   0 ],
//    [ ':'       => terminator       =>   0,   0 ],
//    [ 'in'      => terminator       =>   0,   0 ],
//    [ '#'       => fragment         =>   0,   0 ],
//    
//    # One token to end them all and in the darkness bind them
//    [ 'end'     => end              =>   0,   0 ],
}
