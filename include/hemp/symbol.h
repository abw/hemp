#ifndef HEMP_SYMBOL_H
#define HEMP_SYMBOL_H

#include <hemp/type.h>
#include <hemp/text.h>


/*--------------------------------------------------------------------------
 * data structures
 *--------------------------------------------------------------------------*/

/* vtable for skipping over whitespace, delimiters, separators, etc */

struct hemp_skip_s {
    hemp_bool_t     mutable;
    hemp_skip_f     space;
    hemp_skip_f     delimiter;
    hemp_skip_f     separator;
};

typedef struct hemp_skip_s  * hemp_skip_v;


/* vtable for parsing elements as expressions */

struct hemp_parse_s {
    hemp_parse_f    expr;
    hemp_parse_f    exprs;
    hemp_parse_f    etc;
};

typedef struct hemp_parse_s  * hemp_parse_v;


/* symbol vtable */

struct hemp_symbol_s {
    HEMP_TYPE_BASE

    /* symbol metadata */
    hemp_cstr_p     token;
    hemp_flags_t    flags;
    hemp_prec_t     lprec;
    hemp_prec_t     rprec;

    /* general methods */
    hemp_scan_f     scanner;
    hemp_clean_f    cleanup;

    /* parsing vtables */
    hemp_skip_v     skip;
    hemp_parse_v    parse;

    /* value methods */
    hemp_text_f     source;
    hemp_text_f     text;
//    hemp_num_f      number;

    // tmp - old
    hemp_parse_f    parse_expr;


    // etc
};

struct hemp_symbols_s {
    hemp_cstr_p     name;
    hemp_symbol_f   constructor;
};

/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

hemp_symbol_p
hemp_symbol_init(
    hemp_cstr_p name,
    hemp_cstr_p token
);


hemp_skip_v
hemp_symbol_skip_vtable(
    hemp_symbol_p symbol,
    hemp_skip_f   space,
    hemp_skip_f   delimiter,
    hemp_skip_f   separator
);

void
hemp_symbol_free(
    hemp_symbol_p symbol
);


/*--------------------------------------------------------------------------
 * externals
 *--------------------------------------------------------------------------*/

extern hemp_skip_v HempSkipNone;
extern hemp_skip_v HempSkipAll;
extern hemp_skip_v HempSkipDelimiter;
extern hemp_skip_v HempSkipSeparator;


#endif /* HEMP_SYMBOL_H */