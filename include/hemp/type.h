#ifndef HEMP_TYPE_H
#define HEMP_TYPE_H

#include <hemp/core.h>


/*--------------------------------------------------------------------------
 * data structures
 *--------------------------------------------------------------------------*/

#define HEMP_TYPE_BASE          \
    hemp_int_t      id;         \
    hemp_str_p      name;       \
    hemp_type_p     base;       \
    hemp_hash_p     methods;

#define HEMP_TYPE_VALUE         \
    hemp_value_f    value;      \
    hemp_vtext_f    text;       \
    hemp_value_f    number;     \
    hemp_value_f    integer;    \
    hemp_value_f    boolean;    \
    hemp_value_f    compare;    \
    hemp_value_f    defined;

struct hemp_type_s {
    HEMP_TYPE_BASE
    HEMP_TYPE_VALUE
};


/* other stuff to add:
    # memory management
    acquire         # allocate memory
    prepare         # initialise object
    cleanup         # cleanup object
    release         # release memory
    dot/get/set
    pairs
    apply     # function application
*/


/*--------------------------------------------------------------------------
 * core types
 *
 * We have a global list of 32 vtables, each of which contains a core set
 * of pseudo-methods that can be called against different data types.
 * The first 16 entries correspond to the core value types: number, integer, 
 * C string, text, etc.  The next 16 provide vtables for the singleton 
 * identity values: HempMissing, HempNothing, HempFalse, HempTrue, 
 * HempBefore, HempEqual and HempAfter.
  *--------------------------------------------------------------------------*/
 
extern hemp_type_p HempValue;
extern hemp_type_p HempNumber;
extern hemp_type_p HempInteger;
extern hemp_type_p HempString;
extern hemp_type_p HempText;
extern hemp_type_p HempIdentity;
extern hemp_type_p HempReserved;
extern hemp_type_p HempUnused;
extern hemp_type_p hemp_global_types[HEMP_TYPES_SIZE];


/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

hemp_type_p
    hemp_type_init(
        hemp_int_t id,
        hemp_str_p name
    );

hemp_type_p 
    hemp_type_subtype(
        hemp_type_p base,
        hemp_int_t  id,
        hemp_str_p  name
    );

void
    hemp_type_free(
        hemp_type_p
    );

void hemp_global_types_init();
void hemp_global_types_free();

HEMP_TYPE_FUNC(hemp_type_number);
HEMP_TYPE_FUNC(hemp_type_integer);
HEMP_TYPE_FUNC(hemp_type_string);
HEMP_TYPE_FUNC(hemp_type_text);
HEMP_TYPE_FUNC(hemp_type_identity);
HEMP_TYPE_FUNC(hemp_type_reserved);
HEMP_TYPE_FUNC(hemp_type_unused);

HEMP_TYPE_METHOD(hemp_method_value_name);


#endif /* HEMP_TYPE_H */

