#include <hemp.h>

#define HEMP_LANGUAGE_VERSION 0.01

HEMP_LANGUAGE_FUNC(hemp_language_hemp_init);


/* basic element prototypes */
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
HEMP_SYMBOLS_FUNC(hemp_element_boolop_symbols);
HEMP_ELEMENT_FUNC(hemp_element_boolop_not_symbol);
HEMP_ELEMENT_FUNC(hemp_element_boolop_and_symbol);
HEMP_ELEMENT_FUNC(hemp_element_boolop_or_symbol);
HEMP_ELEMENT_FUNC(hemp_element_boolop_nor_symbol);
HEMP_ELEMENT_FUNC(hemp_element_boolop_TODO_symbol);

/* number operator prototypes */
HEMP_SYMBOLS_FUNC(hemp_element_numop_symbols);
HEMP_ELEMENT_FUNC(hemp_element_numop_inc_symbol);
HEMP_ELEMENT_FUNC(hemp_element_numop_dec_symbol);
HEMP_ELEMENT_FUNC(hemp_element_numop_plus_symbol);
HEMP_ELEMENT_FUNC(hemp_element_numop_minus_symbol);
HEMP_ELEMENT_FUNC(hemp_element_numop_power_symbol);
HEMP_ELEMENT_FUNC(hemp_element_numop_multiply_symbol);
HEMP_ELEMENT_FUNC(hemp_element_numop_divide_symbol);
HEMP_ELEMENT_FUNC(hemp_element_numop_divint_symbol);
HEMP_ELEMENT_FUNC(hemp_element_numop_modulus_symbol);
HEMP_ELEMENT_FUNC(hemp_element_numop_compare_symbol);
HEMP_ELEMENT_FUNC(hemp_element_numop_equal_symbol);
HEMP_ELEMENT_FUNC(hemp_element_numop_not_equal_symbol);
HEMP_ELEMENT_FUNC(hemp_element_numop_less_than_symbol);
HEMP_ELEMENT_FUNC(hemp_element_numop_more_than_symbol);
HEMP_ELEMENT_FUNC(hemp_element_numop_less_equal_symbol);
HEMP_ELEMENT_FUNC(hemp_element_numop_more_equal_symbol);
HEMP_ELEMENT_FUNC(hemp_element_numop_TODO_symbol);

/* text operator prototypes */
HEMP_SYMBOLS_FUNC(hemp_element_textop_symbols);
HEMP_ELEMENT_FUNC(hemp_element_textop_concat_symbol);
HEMP_ELEMENT_FUNC(hemp_element_textop_compare_symbol);
HEMP_ELEMENT_FUNC(hemp_element_textop_equal_symbol);
HEMP_ELEMENT_FUNC(hemp_element_textop_not_equal_symbol);
HEMP_ELEMENT_FUNC(hemp_element_textop_less_than_symbol);
HEMP_ELEMENT_FUNC(hemp_element_textop_more_than_symbol);
HEMP_ELEMENT_FUNC(hemp_element_textop_less_equal_symbol);
HEMP_ELEMENT_FUNC(hemp_element_textop_more_equal_symbol);
HEMP_ELEMENT_FUNC(hemp_element_textop_TODO_symbol);



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

static struct hemp_symbols_s hemp_symbols_hemp_boolop[] = {
    { "hemp.boolop.not",        &hemp_element_boolop_not_symbol         },
    { "hemp.boolop.and",        &hemp_element_boolop_and_symbol         },
    { "hemp.boolop.or",         &hemp_element_boolop_or_symbol          },
    { "hemp.boolop.nor",        &hemp_element_boolop_nor_symbol         },
    { NULL, NULL },
};

static struct hemp_symbols_s hemp_symbols_hemp_numop[] = {
    { "hemp.numop.inc",         &hemp_element_numop_inc_symbol          },
    { "hemp.numop.dec",         &hemp_element_numop_dec_symbol          },
    { "hemp.numop.plus",        &hemp_element_numop_plus_symbol         },
    { "hemp.numop.minus",       &hemp_element_numop_minus_symbol        },
    { "hemp.numop.power",       &hemp_element_numop_power_symbol        },
    { "hemp.numop.multiply",    &hemp_element_numop_multiply_symbol     },
    { "hemp.numop.divide",      &hemp_element_numop_divide_symbol       },
    { "hemp.numop.divint",      &hemp_element_numop_divint_symbol       },
    { "hemp.numop.modulus",     &hemp_element_numop_modulus_symbol      },
    { "hemp.numop.compare",     &hemp_element_numop_compare_symbol      },
    { "hemp.numop.equal",       &hemp_element_numop_equal_symbol        },
    { "hemp.numop.not_equal",   &hemp_element_numop_not_equal_symbol    },
    { "hemp.numop.less_than",   &hemp_element_numop_less_than_symbol    },
    { "hemp.numop.more_than",   &hemp_element_numop_more_than_symbol    },
    { "hemp.numop.less_equal",  &hemp_element_numop_less_equal_symbol   },
    { "hemp.numop.more_equal",  &hemp_element_numop_more_equal_symbol   },
    { NULL, NULL },
};

static struct hemp_symbols_s hemp_symbols_hemp_textop[] = {
    { "hemp.textop.concat",     &hemp_element_textop_concat_symbol      },
    { "hemp.textop.compare",    &hemp_element_textop_compare_symbol     },
    { "hemp.textop.equal",      &hemp_element_textop_equal_symbol       },
    { "hemp.textop.not_equal",  &hemp_element_textop_not_equal_symbol   },
    { "hemp.textop.less_than",  &hemp_element_textop_less_than_symbol   },
    { "hemp.textop.more_than",  &hemp_element_textop_more_than_symbol   },
    { "hemp.textop.less_equal", &hemp_element_textop_less_equal_symbol  },
    { "hemp.textop.more_equal", &hemp_element_textop_more_equal_symbol  },
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
    HEMP_ELEMENT("hemp.boolop.*", &hemp_element_boolop_symbols);
    HEMP_ELEMENT("hemp.numop.*",  &hemp_element_numop_symbols);
    HEMP_ELEMENT("hemp.textop.*", &hemp_element_textop_symbols);

    /* register grammars */
    HEMP_GRAMMAR("hemp.alpha", &hemp_grammar_hemp_alpha);
    HEMP_GRAMMAR("hemp.bravo", &hemp_grammar_hemp_bravo);

    return language;
}



/*--------------------------------------------------------------------------
 * boolean operator elements
 *--------------------------------------------------------------------------*/

hemp_action_p
hemp_element_boolop_symbols(
    hemp_p      hemp,
    hemp_cstr_p name
) {
//  debug("initialising hemp boolean operator symbols (%s requested)\n", name);

    /* we should detect if we've done this already and skip it */
    HEMP_ELEMENTS(hemp_symbols_hemp_boolop);

    /* now try again */
    return (hemp_action_p) hemp_hash_fetch(
        hemp->elements->constructors, name
    );
}


HEMP_NO_INLINE hemp_symbol_p
hemp_element_boolop_TODO_symbol(
    hemp_p        hemp,
    hemp_symbol_p symbol
) {
    debug_todo("boolop constructor for %s symbol", symbol->name);
    DONT_OPTIMISE_ME_AWAY;
    return symbol;
}


hemp_symbol_p
hemp_element_boolop_not_symbol(
    hemp_p        hemp,
    hemp_symbol_p symbol
) {
    return hemp_element_boolop_TODO_symbol(hemp, symbol);
}


hemp_symbol_p
hemp_element_boolop_and_symbol(
    hemp_p        hemp,
    hemp_symbol_p symbol
) {
    return hemp_element_boolop_TODO_symbol(hemp, symbol);
}


hemp_symbol_p
hemp_element_boolop_or_symbol(
    hemp_p        hemp,
    hemp_symbol_p symbol
) {
    return hemp_element_boolop_TODO_symbol(hemp, symbol);
}


hemp_symbol_p
hemp_element_boolop_nor_symbol(
    hemp_p        hemp,
    hemp_symbol_p symbol
) {
    return hemp_element_boolop_TODO_symbol(hemp, symbol);
}




/*--------------------------------------------------------------------------
 * number operator elements
 *--------------------------------------------------------------------------*/

hemp_action_p
hemp_element_numop_symbols(
    hemp_p      hemp,
    hemp_cstr_p name
) {
//  debug("initialising hemp number operator symbols (%s requested)\n", name);

    /* we should detect if we've done this already and skip it */
    HEMP_ELEMENTS(hemp_symbols_hemp_numop);

    /* now try again */
    return (hemp_action_p) hemp_hash_fetch(
        hemp->elements->constructors, name
    );
}


HEMP_NO_INLINE hemp_symbol_p
hemp_element_numop_TODO_symbol(
    hemp_p        hemp,
    hemp_symbol_p symbol
) {
    debug_todo("numop constructor for %s symbol", symbol->name);
    DONT_OPTIMISE_ME_AWAY;
    return symbol;
}


hemp_symbol_p
hemp_element_numop_inc_symbol(
    hemp_p        hemp,
    hemp_symbol_p symbol
) {
    return hemp_element_numop_TODO_symbol(hemp, symbol);
}


hemp_symbol_p
hemp_element_numop_dec_symbol(
    hemp_p        hemp,
    hemp_symbol_p symbol
) {
    return hemp_element_numop_TODO_symbol(hemp, symbol);
}



hemp_symbol_p hemp_element_numop_binary_symbol(
    hemp_p        hemp,
    hemp_symbol_p symbol
) {
    symbol->expr   = &hemp_element_not_expr;
    symbol->infix  = &hemp_element_parse_infix_left;
    symbol->source = &hemp_element_binary_text;
    symbol->text   = &hemp_element_binary_text;
    return symbol;
}

hemp_symbol_p hemp_element_numop_power_symbol(
    hemp_p        hemp,
    hemp_symbol_p symbol
) {
    return hemp_element_numop_binary_symbol(hemp, symbol);
}

hemp_symbol_p hemp_element_numop_divint_symbol(
    hemp_p        hemp,
    hemp_symbol_p symbol
) {
    return hemp_element_numop_binary_symbol(hemp, symbol);
}


hemp_symbol_p hemp_element_numop_modulus_symbol(
    hemp_p        hemp,
    hemp_symbol_p symbol
) {
    return hemp_element_numop_binary_symbol(hemp, symbol);
}


hemp_symbol_p hemp_element_numop_compare_symbol(
    hemp_p        hemp,
    hemp_symbol_p symbol
) {
    return hemp_element_numop_binary_symbol(hemp, symbol);
}


hemp_symbol_p hemp_element_numop_equal_symbol(
    hemp_p        hemp,
    hemp_symbol_p symbol
) {
    return hemp_element_numop_binary_symbol(hemp, symbol);
}


hemp_symbol_p hemp_element_numop_not_equal_symbol(
    hemp_p        hemp,
    hemp_symbol_p symbol
) {
    return hemp_element_numop_binary_symbol(hemp, symbol);
}


hemp_symbol_p hemp_element_numop_less_than_symbol(
    hemp_p        hemp,
    hemp_symbol_p symbol
) {
    return hemp_element_numop_binary_symbol(hemp, symbol);
}


hemp_symbol_p hemp_element_numop_more_than_symbol(
    hemp_p        hemp,
    hemp_symbol_p symbol
) {
    return hemp_element_numop_binary_symbol(hemp, symbol);
}


hemp_symbol_p hemp_element_numop_less_equal_symbol(
    hemp_p        hemp,
    hemp_symbol_p symbol
) {
    return hemp_element_numop_binary_symbol(hemp, symbol);
}


hemp_symbol_p hemp_element_numop_more_equal_symbol(
    hemp_p        hemp,
    hemp_symbol_p symbol
) {
    return hemp_element_numop_binary_symbol(hemp, symbol);
}



/*--------------------------------------------------------------------------
 * text operator elements
 *--------------------------------------------------------------------------*/

hemp_action_p
hemp_element_textop_symbols(
    hemp_p      hemp,
    hemp_cstr_p name
) {
//  debug("initialising hemp text operator symbols (%s requested)\n", name);

    /* we should detect if we've done this already and skip it */
    HEMP_ELEMENTS(hemp_symbols_hemp_textop);

    /* now try again */
    return (hemp_action_p) hemp_hash_fetch(
        hemp->elements->constructors, name
    );
}


HEMP_NO_INLINE hemp_symbol_p
hemp_element_textop_TODO_symbol(
    hemp_p        hemp,
    hemp_symbol_p symbol
) {
    debug_todo("textop constructor for %s symbol", symbol->name);
    DONT_OPTIMISE_ME_AWAY;
    return symbol;
}


hemp_symbol_p
hemp_element_textop_concat_symbol(
    hemp_p        hemp,
    hemp_symbol_p symbol
) {
    return hemp_element_textop_TODO_symbol(hemp, symbol);
}


hemp_symbol_p
hemp_element_textop_compare_symbol(
    hemp_p        hemp,
    hemp_symbol_p symbol
) {
    return hemp_element_textop_TODO_symbol(hemp, symbol);
}


hemp_symbol_p hemp_element_textop_equal_symbol(
    hemp_p        hemp,
    hemp_symbol_p symbol
) {
    return hemp_element_textop_TODO_symbol(hemp, symbol);
}


hemp_symbol_p hemp_element_textop_not_equal_symbol(
    hemp_p        hemp,
    hemp_symbol_p symbol
) {
    return hemp_element_textop_TODO_symbol(hemp, symbol);
}


hemp_symbol_p hemp_element_textop_less_than_symbol(
    hemp_p        hemp,
    hemp_symbol_p symbol
) {
    return hemp_element_textop_TODO_symbol(hemp, symbol);
}


hemp_symbol_p hemp_element_textop_more_than_symbol(
    hemp_p        hemp,
    hemp_symbol_p symbol
) {
    return hemp_element_textop_TODO_symbol(hemp, symbol);
}


hemp_symbol_p hemp_element_textop_less_equal_symbol(
    hemp_p        hemp,
    hemp_symbol_p symbol
) {
    return hemp_element_textop_TODO_symbol(hemp, symbol);
}


hemp_symbol_p hemp_element_textop_more_equal_symbol(
    hemp_p        hemp,
    hemp_symbol_p symbol
) {
    return hemp_element_textop_TODO_symbol(hemp, symbol);
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

    HEMP_OPERATOR1("hemp.numop.inc",           "++",       295,  295);
    HEMP_OPERATOR1("hemp.numop.dec",           "--",       295,  295);
    HEMP_OPERATOR1("hemp.numop.power",         "**",       290,    0);
    HEMP_OPERATOR1("hemp.numop.plus",          "+",        275,  285);
    HEMP_OPERATOR1("hemp.numop.minus",         "-",        275,  285);
    HEMP_OPERATOR1("hemp.numop.multiply",      "*",        280,    0);
    HEMP_OPERATOR1("hemp.numop.divide",        "/",        280,    0);
    HEMP_OPERATOR1("hemp.numop.modulus",       "mod",      280,    0);
//  HEMP_OPERATOR1("hemp.numop.divint",        "div",      280,    0);

    HEMP_OPERATOR1("hemp.textop.concat",       "~",        270,  270);
                
    HEMP_OPERATOR1("hemp.numop.compare",       "<=>",      260,    0);
    HEMP_OPERATOR1("hemp.numop.equal",         "==",       260,    0);
    HEMP_OPERATOR1("hemp.numop.not_equal",     "!=",       260,    0);
    HEMP_OPERATOR1("hemp.numop.less_than",     "<",        260,    0);
    HEMP_OPERATOR1("hemp.numop.more_than",     ">",        260,    0);
    HEMP_OPERATOR1("hemp.numop.less_equal",    "<=",       260,    0);
    HEMP_OPERATOR1("hemp.numop.more_equal",    ">=",       260,    0);

    HEMP_OPERATOR1("hemp.textop.compare",      "cmp",      260,    0);
    HEMP_OPERATOR1("hemp.textop.equal",        "eq",       260,    0);
    HEMP_OPERATOR1("hemp.textop.not_equal",    "ne",       260,    0);
    HEMP_OPERATOR1("hemp.textop.less_than",    "lt",       260,    0);
    HEMP_OPERATOR1("hemp.textop.more_than",    "gt",       260,    0);
    HEMP_OPERATOR1("hemp.textop.less_equal",   "le",       260,    0);
    HEMP_OPERATOR1("hemp.textop.more_equal",   "ge",       260,    0);

    HEMP_OPERATOR1("hemp.boolop.not",          "!",          0,  285);
    HEMP_OPERATOR1("hemp.boolop.and",          "&&",       255,    0);
    HEMP_OPERATOR1("hemp.boolop.or",           "||",       250,    0);
    HEMP_OPERATOR1("hemp.boolop.nor",          "!!",       250,    0);

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

    HEMP_OPERATOR1("hemp.boolop.not",          "not",        0,  190);
    HEMP_OPERATOR1("hemp.boolop.and",          "and",      180,    0);
    HEMP_OPERATOR1("hemp.boolop.or",           "or",       170,    0);
    HEMP_OPERATOR1("hemp.boolop.nor",          "nor",      170,    0);

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
