#include <hemp/symbol.h>
#include <hemp/element.h>



/*--------------------------------------------------------------------------
 * symbol functions
 *--------------------------------------------------------------------------*/

hemp_symbol_p
hemp_symbol_init(
    hemp_cstr_p name,
    hemp_cstr_p start,
    hemp_cstr_p end
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
    symbol->expr    = &hemp_element_not_expr;
    symbol->infix   = &hemp_element_not_infix;
    symbol->source  = &hemp_element_not_source;
    symbol->text    = &hemp_element_not_text;
    symbol->number  = &hemp_element_not_number;
    symbol->integer = &hemp_element_not_integer;
    symbol->boolean = &hemp_element_not_boolean;
    symbol->text    = &hemp_element_literal_text;       // tmp

    /* clone the start token if there is one, and the end token if there 
     * is one and it's not the same as the start token
     */
    if (start) 
        start = hemp_cstr_clone(start, "symbol start token");

    if (end)
        end = (start && (start == end || hemp_cstr_eq(start, end)))
            ? start
            : hemp_cstr_clone(end, "symbol end token");
        
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
    debug("symbol at %p\n", symbol->name, symbol);
    debug("       name: %s\n", symbol->name);
    debug("      start: %s\n", symbol->start ? symbol->start : "<none>");
    debug("        end: %s\n", symbol->end ? symbol->end : "<none>");
    debug("      flags: %04x\n", symbol->flags);
    debug("      lprec: %dx\n", symbol->lprec);
    debug("      rprec: %dx\n", symbol->rprec);
    debug("    scanner: %px\n", symbol->scanner);
    debug("    cleanup: %px\n", symbol->cleanup);
    debug("       expr: %px\n", symbol->expr);
    debug("      infix: %px\n", symbol->infix);
    debug("     source: %px\n", symbol->source);
    debug("       text: %px\n", symbol->text);
    debug("     number: %px\n", symbol->number);
    debug("    integer: %px\n", symbol->integer);
    debug("    boolean: %px\n", symbol->boolean);
}

