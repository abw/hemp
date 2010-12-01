#ifndef HEMP_SYMBOL_H
#define HEMP_SYMBOL_H

#include <hemp/namespace.h>
#include <hemp/type.h>
#include <hemp/type/text.h>


/*--------------------------------------------------------------------------
 * tye definitions
 *--------------------------------------------------------------------------*/

typedef hemp_symbol 
(* hemp_symbol_f)(
    hemp_hemp       hemp,
    hemp_symbol     symbol
);


/* symbol vtable */

struct hemp_symbol {
    /* a symbol is a kind of object value type so we start with those members */
    HEMP_TYPE_BASE

    /* a symbol is bound to a grammar */
    hemp_grammar    grammar;

    /* symbol metadata */
    hemp_string     start;
    hemp_string     end;
    hemp_flags      flags;
    hemp_oprec      lprec;
    hemp_oprec      rprec;

    /* intialisation (on scan) and cleanup methods */
    hemp_sym_scan_f scanner;
    hemp_eclean_f   cleanup;

    /* parsing methods */
    hemp_prefix     parse_prefix;
    hemp_postfix    parse_postfix;
    hemp_prefix     parse_fixed;
    hemp_prefix     parse_params;
    hemp_fixup      parse_lvalue;
    hemp_fixup      parse_proto;

    /* fixup handlers for general parsing jiggery */
    //hemp_fixup      fixup_proto;

    /* value methods */
    hemp_output_f   token;
    hemp_output_f   source;
};


struct hemp_symbols {
    hemp_string     name;
    hemp_symbol_f   ctor;
};

/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

hemp_symbol
hemp_symbol_new(
    hemp_string     name,
    hemp_string     start,
    hemp_string     end
);


void
hemp_symbol_free(
    hemp_symbol     symbol
);

void 
hemp_symbol_dump(
    hemp_symbol     symbol
);


/*--------------------------------------------------------------------------
 * global symbols and symbol constructors
 *--------------------------------------------------------------------------*/

extern hemp_symbol HempSymbolSpace;
extern hemp_symbol HempSymbolComment;
extern hemp_symbol HempSymbolTagStart;
extern hemp_symbol HempSymbolTagEnd;
extern hemp_symbol HempSymbolBlock;
extern hemp_symbol HempSymbolText;
extern hemp_symbol HempSymbolWord;
extern hemp_symbol HempSymbolNumber;
extern hemp_symbol HempSymbolInteger;
extern hemp_symbol HempSymbolEOF;


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


#endif /* HEMP_SYMBOL_H */