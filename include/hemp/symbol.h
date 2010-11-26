#ifndef HEMP_SYMBOL_H
#define HEMP_SYMBOL_H

#include <hemp/namespace.h>
#include <hemp/type.h>
#include <hemp/type/text.h>


/*--------------------------------------------------------------------------
 * data structures
 *--------------------------------------------------------------------------*/

/* symbol vtable */

struct hemp_symbol {
    /* a symbol is a kind of object value type so we start with those members */
    HEMP_TYPE_BASE

    /* a symbol is bound to a grammar */
    hemp_grammar        grammar;

    /* symbol metadata */
    hemp_string         start;
    hemp_string         end;
    hemp_flags          flags;
    hemp_oprec          lprec;
    hemp_oprec          rprec;

    /* intialisation (on scan) and cleanup methods */
    hemp_sym_scan_f     scanner;
    hemp_eclean_f       cleanup;

    /* parsing methods */
    hemp_prefix_f       parse_prefix;             // TODO: add 'parse_' prefix
    hemp_postfix_f      parse_postfix;
    hemp_prefix_f       parse_fixed;
    hemp_postfix_f      parse_params;
    hemp_compile_f      lvalue_param;

    /* value methods */
    hemp_output_f       token;
    hemp_output_f       source;
};


/* symbols collection */

struct hemp_symbols {
    hemp_string         name;
    hemp_symbol_f       constructor;
};

/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

hemp_symbol
hemp_symbol_new(
    hemp_string         name,
    hemp_string         start,
    hemp_string         end
);


void
hemp_symbol_free(
    hemp_symbol         symbol
);

void 
hemp_symbol_dump(
    hemp_symbol         symbol
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