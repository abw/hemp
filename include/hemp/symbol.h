#ifndef HEMP_SYMBOL_H
#define HEMP_SYMBOL_H

#include <hemp/namespace.h>
#include <hemp/type.h>
#include <hemp/type/text.h>


/*--------------------------------------------------------------------------
 * data structures
 *--------------------------------------------------------------------------*/

/* symbol vtable */

struct hemp_symbol_s {
    HEMP_TYPE_BASE

    /* symbol metadata */
    hemp_str_p      start;
    hemp_str_p      end;
    hemp_flags_t    flags;
    hemp_prec_t     lprec;
    hemp_prec_t     rprec;

    /* intialisation (on scan) and cleanup methods */
    hemp_sym_scan_f scanner;
    hemp_eclean_f   cleanup;

    /* parsing methods */
    hemp_prefix_f   prefix;
    hemp_postfix_f  postfix;
    hemp_prefix_f   fixed;
    hemp_postfix_f  parse_params;

    /* value methods */
    hemp_output_f     token;
    hemp_output_f     source;

//    hemp_text_f     text;
//    hemp_eval_f     value;
//    hemp_eval_f     number;
//    hemp_eval_f     integer;
//    hemp_eval_f     boolean;
//    hemp_eval_f     compare;
//    hemp_operate_f  assign;
//    hemp_evals_f    values;
};


//    + assign

/* symbols collection */

struct hemp_symbols_s {
    hemp_str_p      name;
    hemp_symbol_f   constructor;
};

/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

hemp_symbol_p
hemp_symbol_init(
    hemp_str_p name,
    hemp_str_p start,
    hemp_str_p end
);


void
hemp_symbol_free(
    hemp_symbol_p symbol
);

void 
hemp_symbol_dump(
    hemp_symbol_p symbol
);


/*--------------------------------------------------------------------------
 * global symbols and symbol constructors
 *--------------------------------------------------------------------------*/

extern hemp_symbol_p HempSymbolSpace;
extern hemp_symbol_p HempSymbolComment;
extern hemp_symbol_p HempSymbolTagStart;
extern hemp_symbol_p HempSymbolTagEnd;
extern hemp_symbol_p HempSymbolBlock;
extern hemp_symbol_p HempSymbolText;
extern hemp_symbol_p HempSymbolWord;
extern hemp_symbol_p HempSymbolNumber;
extern hemp_symbol_p HempSymbolInteger;
extern hemp_symbol_p HempSymbolEOF;


HEMP_GLOBAL_SYMBOL(hemp_symbol_space);
HEMP_GLOBAL_SYMBOL(hemp_symbol_comment);
HEMP_GLOBAL_SYMBOL(hemp_symbol_tag_start);
HEMP_GLOBAL_SYMBOL(hemp_symbol_tag_end);
HEMP_GLOBAL_SYMBOL(hemp_symbol_block);
HEMP_GLOBAL_SYMBOL(hemp_symbol_text);
HEMP_GLOBAL_SYMBOL(hemp_symbol_word);
HEMP_GLOBAL_SYMBOL(hemp_symbol_number);
HEMP_GLOBAL_SYMBOL(hemp_symbol_integer);
HEMP_GLOBAL_SYMBOL(hemp_symbol_eof);


/*--------------------------------------------------------------------------
 * externals
 *--------------------------------------------------------------------------*/

//extern struct hemp_parse_s hemp_parse_binary_vtable;


#endif /* HEMP_SYMBOL_H */