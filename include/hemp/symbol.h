#ifndef HEMP_SYMBOL_H
#define HEMP_SYMBOL_H

#include <hemp/type.h>
#include <hemp/text.h>


/*--------------------------------------------------------------------------
 * data structures
 *--------------------------------------------------------------------------*/

/* symbol vtable */

struct hemp_symbol_s {
    hemp_str_p      name;

    /* symbol metadata */
    hemp_str_p      start;
    hemp_str_p      end;
    hemp_flags_t    flags;
    hemp_prec_t     lprec;
    hemp_prec_t     rprec;

    /* intialisation (on scan) and cleanup methods */
    hemp_sym_scan_f scanner;
    hemp_eclean_f   cleanup;

    /* parsing methods */
    hemp_prefix_f   prefix;
    hemp_postfix_f  postfix;

    /* value methods */
    hemp_text_f     token;
    hemp_text_f     source;
    hemp_text_f     text;
    hemp_eval_f     value;
    hemp_eval_f     number;
    hemp_eval_f     integer;
    hemp_eval_f     boolean;
    hemp_eval_f     compare;
};

/* symbols collection */

struct hemp_symbols_s {
    hemp_str_p      name;
    hemp_symbol_f   constructor;
};

/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

hemp_symbol_p
hemp_symbol_init(
    hemp_str_p name,
    hemp_str_p start,
    hemp_str_p end
);


void
hemp_symbol_free(
    hemp_symbol_p symbol
);

void 
hemp_symbol_dump(
    hemp_symbol_p symbol
);


/*--------------------------------------------------------------------------
 * externals
 *--------------------------------------------------------------------------*/

//extern struct hemp_parse_s hemp_parse_binary_vtable;


#endif /* HEMP_SYMBOL_H */