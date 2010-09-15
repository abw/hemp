#include <hemp/symbol.h>
#include <hemp/element.h>



/*--------------------------------------------------------------------------
 * symbol functions
 *--------------------------------------------------------------------------*/

hemp_symbol_p
hemp_symbol_init(
    hemp_str_p name,
    hemp_str_p start,
    hemp_str_p end
) {
    hemp_symbol_p symbol = (hemp_symbol_p) hemp_mem_alloc(
        sizeof(struct hemp_symbol_s)
    );
    if (! symbol)
        hemp_mem_fail("symbol");

    symbol->name    = name;    // should be const?
    symbol->flags   = 0;
    symbol->lprec   = 0;
    symbol->rprec   = 0;
    symbol->scanner = NULL;
    symbol->cleanup = NULL;
    symbol->prefix  = &hemp_element_not_prefix;
    symbol->postfix = &hemp_element_not_postfix;
    symbol->token   = &hemp_element_not_token;
    symbol->source  = &hemp_element_not_source;
    symbol->text    = &hemp_element_not_text;
    symbol->value   = &hemp_element_not_value;
    symbol->number  = &hemp_element_not_number;
    symbol->integer = &hemp_element_not_integer;
    symbol->boolean = &hemp_element_not_boolean;
    symbol->compare = &hemp_element_not_compare;
    symbol->fixed   = &hemp_element_decline;
    symbol->text    = &hemp_element_literal_text;       // tmp

    /* clone the start token if there is one, and the end token if there 
     * is one and it's not the same as the start token
     */
    if (start) 
        start = hemp_string_clone(start, "symbol start token");

    if (end)
        end = (start && (start == end || hemp_string_eq(start, end)))
            ? start
            : hemp_string_clone(end, "symbol end token");
        
    symbol->start   = start;
    symbol->end     = end;

    return symbol;
}



void
hemp_symbol_free(
    hemp_symbol_p symbol
) {
// No you fuckwit!  The cleanup is for cleaning up element instances, not
// the symbol entries themselves
//    if (symbol->cleanup)
//        symbol->cleanup(symbol);

    /* only free the end token if it's not the same as the start token */
    if ( symbol->end 
    && ( (! symbol->start) || (symbol->start != symbol->end) ) ) 
        hemp_mem_free(symbol->end);

    if (symbol->start)
        hemp_mem_free(symbol->start);

//  hemp_mem_free(symbol->name);                // now static
    hemp_mem_free(symbol);
}


void 
hemp_symbol_dump(
    hemp_symbol_p symbol
) {
    hemp_debug("symbol at %p\n", symbol->name, symbol);
    hemp_debug("       name: %s\n", symbol->name);
    hemp_debug("      start: %s\n", symbol->start ? symbol->start : "<none>");
    hemp_debug("        end: %s\n", symbol->end ? symbol->end : "<none>");
    hemp_debug("      flags: %04x\n", symbol->flags);
    hemp_debug("      lprec: %dx\n", symbol->lprec);
    hemp_debug("      rprec: %dx\n", symbol->rprec);
    hemp_debug("    scanner: %px\n", symbol->scanner);
    hemp_debug("    cleanup: %px\n", symbol->cleanup);
    hemp_debug("     prefix: %px\n", symbol->prefix);
    hemp_debug("    postfix: %px\n", symbol->postfix);
    hemp_debug("      token: %px\n", symbol->token);
    hemp_debug("     source: %px\n", symbol->source);
    hemp_debug("       text: %px\n", symbol->text);
    hemp_debug("      value: %px\n", symbol->value);
    hemp_debug("     number: %px\n", symbol->number);
    hemp_debug("    integer: %px\n", symbol->integer);
    hemp_debug("    boolean: %px\n", symbol->boolean);
}

