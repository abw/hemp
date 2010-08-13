#ifndef HEMP_TYPE_H
#define HEMP_TYPE_H

#include <hemp/core.h>


/*--------------------------------------------------------------------------
 * data structures
 *--------------------------------------------------------------------------*/

#define HEMP_TYPE_BASE          \
    hemp_cstr_p name;

struct hemp_type_s {
    HEMP_TYPE_BASE
};



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

