#include <hemp/element.h>

static struct hemp_symbol_s
    hemp_symbol_text = { 
        "text",                                     /* name                 */
        NULL,                                       /* start token          */
        NULL,                                       /* end token            */
        HEMP_IS_FIXED   |                           /* flags                */
        HEMP_IS_STATIC,
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

hemp_symbol_p HempSymbolText = &hemp_symbol_text;


HEMP_SYMBOL_FUNC(hemp_element_text_symbol) {
    symbol->flags   = HEMP_IS_FIXED | HEMP_IS_STATIC;
    symbol->expr    = &hemp_element_literal_expr,
    symbol->source  = &hemp_element_literal_source;
    symbol->text    = &hemp_element_literal_text;
    symbol->number  = &hemp_element_literal_number;
    symbol->integer = &hemp_element_literal_integer;
    symbol->boolean = &hemp_element_literal_boolean;
    return symbol;
}


/* TODO: perhaps move this as it only applies to FIXED literal text
 * like squote and dquote 
 */

void
hemp_element_text_clean(
    hemp_element_p element
) {
    debug_call("hemp_element_text_clean(%p)\n", element);

    /* squote and dquote elements may have allocated memory to accomodate
     * translated escape sequences, e.g. \n \\, etc
     */
    if (hemp_not_flag(element, HEMP_IS_FIXED)) {
        hemp_mem_free((hemp_mem_p) HEMP_VAL_STR(element->args.value));
    }
}

