#include <hemp/element.h>


static struct hemp_symbol_s
    hemp_symbol_word = { 
        "word",                                     /* name                 */
        NULL,                                       /* start token          */
        NULL,                                       /* end token            */
        0,                                          /* flags                */
        0, 0,                                       /* l/r precedence       */
        NULL,                                       /* scanner callback     */
        NULL,                                       /* cleanup callback     */
        &hemp_element_literal_expr,                 /* parse expression     */
        &hemp_element_not_infix,                    /* parse infix expr     */
        &hemp_element_literal_source,               /* source code          */
        &hemp_element_literal_text,                 /* output text          */
        &hemp_element_literal_number,               /* numeric conversion   */
        &hemp_element_literal_integer,              /* integer conversion   */
        &hemp_element_literal_boolean,              /* boolean conversion   */
    };

hemp_symbol_p HempSymbolWord = &hemp_symbol_word;


HEMP_SYMBOL_FUNC(hemp_element_word_symbol) {
    /* these aren't right, but they'll do for now, for testing purposes */
    symbol->flags   = HEMP_IS_FIXED | HEMP_IS_STATIC;
    symbol->expr    = &hemp_element_literal_expr;
    symbol->source  = &hemp_element_literal_source;
    symbol->text    = &hemp_element_literal_text;
    symbol->number  = &hemp_element_literal_number;
    symbol->integer = &hemp_element_literal_integer;
    symbol->boolean = &hemp_element_literal_boolean;
    return symbol;
}



