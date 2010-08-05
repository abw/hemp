#include <hemp/symbol.h>
#include <hemp/element.h>


/*--------------------------------------------------------------------------
 * static symbol vtables - can be shared between different symbol instances
 *--------------------------------------------------------------------------*/

/* Most elements aren't whitespace, delimiters or separators so they ignore
 * any skip messages during parsing
 */

static struct hemp_skip_s
    hemp_skip_none_vtable = { 
        HEMP_FALSE,
        &hemp_element_dont_skip,
        &hemp_element_dont_skip,
        &hemp_element_dont_skip
    };

/* Whitespace, comments, etc, are completely ignored, so we can safely skip
 * over them by forwarding the message to the next element 
 */

static struct hemp_skip_s
    hemp_skip_all_vtable = { 
        HEMP_FALSE,
        &hemp_element_next_skip_space,
        &hemp_element_next_skip_delimiter,
        &hemp_element_next_skip_separator
    };

/* Delimiters like commas may be used to separate items in lists. They can 
 * usually be skipped over, but are sometimes required to disambiguate one 
 * item from another so they're not quite as ignorable as whitespace
 */

static struct hemp_skip_s
    hemp_skip_delimiter_vtable = { 
        HEMP_FALSE,
        &hemp_element_dont_skip,
        &hemp_element_next_skip_delimiter,
        &hemp_element_dont_skip,
    };

/* Separators are used to separate one expression from another.  This includes
 * things like end of tag markers (e.g. '%]') and semi-colons ';'.  They 
 * indicate "stop and check" points in the parsing process that require 
 * special handling.
 */

static struct hemp_skip_s
    hemp_skip_separator_vtable = { 
        HEMP_FALSE,
        &hemp_element_dont_skip,
        &hemp_element_dont_skip,
        &hemp_element_next_skip_separator,
    };


hemp_skip_v HempSkipNone      = &hemp_skip_none_vtable;
hemp_skip_v HempSkipAll       = &hemp_skip_all_vtable;
hemp_skip_v HempSkipDelimiter = &hemp_skip_delimiter_vtable;
hemp_skip_v HempSkipSeparator = &hemp_skip_separator_vtable;



/*--------------------------------------------------------------------------
 * symbol functions
 *--------------------------------------------------------------------------*/

hemp_symbol_p
hemp_symbol_init(
    hemp_cstr_p name,
    hemp_cstr_p token
) {
    hemp_symbol_p symbol = (hemp_symbol_p) hemp_mem_alloc(
        sizeof(struct hemp_symbol_s)
    );
    if (! symbol)
        hemp_mem_fail("symbol");

//  symbol->name    = hemp_cstr_clone(name, "symbol name");
//  symbol->token   = hemp_cstr_clone(name, "symbol token");
    symbol->name    = name;    // should be const  
    symbol->token   = token ? hemp_cstr_clone(name, "symbol token") : NULL;
    symbol->flags   = 0;
    symbol->lprec   = 0;
    symbol->rprec   = 0;
    symbol->scanner = NULL;
    symbol->cleanup = NULL;
    symbol->source  = NULL;
    symbol->text    = NULL;
//    symbol->number  = NULL;
    symbol->skip    = HempSkipNone;
    symbol->parse   = NULL;             /* TODO */

    return symbol;
}


hemp_skip_v
hemp_symbol_skip_vtable(
    hemp_symbol_p symbol,
    hemp_skip_f   space,
    hemp_skip_f   delimiter,
    hemp_skip_f   separator
) {
    hemp_skip_v   skip = symbol->skip;

    /* create a new mutable skip vtable if we're using a default shared set */
    if (! skip->mutable) {
        skip = symbol->skip = hemp_mem_alloc( sizeof( struct hemp_skip_s ) );

        if (! skip)
            hemp_mem_fail("symbol skip vtable");

        skip->mutable = HEMP_TRUE;
    }
    
    skip->space     = space;
    skip->delimiter = delimiter;
    skip->separator = separator;

    return skip;
}


void
hemp_symbol_free(
    hemp_symbol_p symbol
) {
    if (symbol->cleanup)
        symbol->cleanup(symbol);

    if (symbol->skip->mutable)
        hemp_mem_free(symbol->skip);

    if (symbol->token)
        hemp_mem_free(symbol->token);

//  hemp_mem_free(symbol->name);                // now static
    hemp_mem_free(symbol);
}

