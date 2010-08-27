#ifndef HEMP_TYPE_H
#define HEMP_TYPE_H

#include <hemp/core.h>


/*--------------------------------------------------------------------------
 * data structures
 *--------------------------------------------------------------------------*/

#define HEMP_TYPE_BASE          \
    hemp_u8_t       id;         \
    hemp_cstr_p     name;

#define HEMP_TYPE_VALUE         \
    hemp_vtext_f    text;       \
    hemp_value_f    value;      \
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
 * function prototypes
 *--------------------------------------------------------------------------*/

hemp_type_p hemp_type_init(hemp_cstr_p);
void        hemp_type_free(hemp_type_p);


/*--------------------------------------------------------------------------
 * macros
 *--------------------------------------------------------------------------*/

//#define hemp_type_name(obj)     \
//    obj->type->name
//

#endif /* HEMP_TYPE_H */

