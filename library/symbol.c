#include <hemp/symbol.h>
#include <hemp/element.h>



/*--------------------------------------------------------------------------
 * symbol functions
 *--------------------------------------------------------------------------*/

hemp_symbol
hemp_symbol_new(
    hemp_string name,
    hemp_string start,
    hemp_string end
) {
    hemp_symbol symbol;
    HEMP_ALLOCATE(symbol);

    symbol->name            = name;    // should be const?
    symbol->namespace       = hemp_namespace_instance(name);
    symbol->flags           = 0;
    symbol->lprec           = 0;
    symbol->rprec           = 0;
    symbol->grammar         = NULL;
    symbol->scanner         = NULL;
    symbol->cleanup         = NULL;
    symbol->parse_prefix    = NULL;
    symbol->parse_postfix   = NULL;
    symbol->parse_fixed     = NULL;
    symbol->parse_params    = NULL;
    symbol->parse_lvalue    = NULL; //&hemp_element_not_lvalue;
    symbol->parse_body      = &hemp_element_parse_body;     // default to NULL?
    symbol->parse_proto     = &hemp_element_not_proto;
    symbol->token           = &hemp_element_not_token;
    symbol->source          = &hemp_element_not_source;
    symbol->text            = &hemp_element_not_text;
    symbol->value           = &hemp_element_not_value;
//  symbol->values          = &hemp_element_not_values;
//  symbol->params          = &hemp_element_not_params;
    symbol->number          = &hemp_element_not_number;
    symbol->integer         = &hemp_element_not_integer;
    symbol->boolean         = &hemp_element_not_boolean;
    symbol->compare         = &hemp_element_not_compare;
    symbol->assign          = &hemp_element_not_assign;
//  symbol->parse_fixed     = &hemp_element_decline;
    symbol->text            = &hemp_element_literal_text;       // tmp
    
    // hmmm... can we set the same defaults as we do for value types?
    symbol->values          = &hemp_value_values;
    symbol->params          = &hemp_value_params;
    symbol->apply           = &hemp_value_apply;

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
    hemp_symbol symbol
) {
    /* only free the end token if it's not the same as the start token */
    if ( symbol->end 
    && ( (! symbol->start) || (symbol->start != symbol->end) ) ) 
        hemp_mem_free(symbol->end);

    if (symbol->start)
        hemp_mem_free(symbol->start);

//  hemp_mem_free(symbol->name);                // now static
//    hemp_debug_msg("freeing symbol: %s at %p\n", symbol->name, symbol);
    hemp_mem_free(symbol);
//    hemp_debug_msg("done\n");
}



/*--------------------------------------------------------------------------
 * debugging
 *--------------------------------------------------------------------------*/

void 
hemp_symbol_dump(
    hemp_symbol symbol
) {
    hemp_debug("symbol at %p\n", symbol->name, symbol);
    hemp_debug("       name: %s\n", symbol->name);
    hemp_debug("      start: %s\n", symbol->start ? symbol->start : "<none>");
    hemp_debug("        end: %s\n", symbol->end ? symbol->end : "<none>");
    hemp_debug("      flags: %04x\n", symbol->flags);
    hemp_debug("      lprec: %d\n", symbol->lprec);
    hemp_debug("      rprec: %d\n", symbol->rprec);
    hemp_debug("    scanner: %p\n", symbol->scanner);
    hemp_debug("    cleanup: %p\n", symbol->cleanup);
    hemp_debug("     prefix: %p\n", symbol->parse_prefix);
    hemp_debug("    postfix: %p\n", symbol->parse_postfix);
    hemp_debug("      token: %p\n", symbol->token);
    hemp_debug("     source: %p\n", symbol->source);
    hemp_debug("       text: %p\n", symbol->text);
    hemp_debug("      value: %p\n", symbol->value);
    hemp_debug("     number: %p\n", symbol->number);
    hemp_debug("    integer: %p\n", symbol->integer);
    hemp_debug("    boolean: %p\n", symbol->boolean);
}

