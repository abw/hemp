#ifndef HEMP_SYMBOL_H
#define HEMP_SYMBOL_H

#include <hemp/namespace.h>
#include <hemp/type.h>
#include <hemp/type/text.h>

/* NOT USED */

struct hemp_symbols {
    HempString     name;
    hemp_symbol_f   ctor;
};

/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

hemp_symbol
hemp_symbol_new(
    HempString     name,
    HempString     start,
    HempString     end
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

extern hemp_symbol HempElementSpace;
extern hemp_symbol HempElementComment;
extern hemp_symbol HempElementTagStart;
extern hemp_symbol HempElementTagEnd;
extern hemp_symbol HempElementBlock;
extern hemp_symbol HempElementText;
extern hemp_symbol HempElementWord;
extern hemp_symbol HempElementNumber;
extern hemp_symbol HempElementInteger;
extern hemp_symbol HempElementEOF;


HEMP_GLOBAL_ELEMENT(hemp_symbol_space);
HEMP_GLOBAL_ELEMENT(hemp_symbol_comment);
HEMP_GLOBAL_ELEMENT(hemp_symbol_tag_start);
HEMP_GLOBAL_ELEMENT(hemp_symbol_tag_end);
HEMP_GLOBAL_ELEMENT(hemp_symbol_block);
HEMP_GLOBAL_ELEMENT(hemp_symbol_text);
HEMP_GLOBAL_ELEMENT(hemp_symbol_word);
HEMP_GLOBAL_ELEMENT(hemp_symbol_number);
HEMP_GLOBAL_ELEMENT(hemp_symbol_integer);
HEMP_GLOBAL_ELEMENT(hemp_symbol_eof);


#endif /* HEMP_SYMBOL_H */