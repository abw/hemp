#ifndef HEMP_CODE_H
#define HEMP_CODE_H

#include <hemp/core.h>
#include <hemp/value.h>


/*--------------------------------------------------------------------------
 * data structures
 *--------------------------------------------------------------------------*/

struct hemp_code_s {
//    hemp_scope_t    scope;
//    hemp_element_t  source;
    hemp_value_t    body;
}; 



/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

HEMP_INLINE hemp_code_p
    hemp_code_init();

HEMP_INLINE void
    hemp_code_release(
        hemp_code_p code
    );

HEMP_INLINE void
    hemp_code_free(
        hemp_code_p code
    );


/*--------------------------------------------------------------------------
 * type functions and methods
 *--------------------------------------------------------------------------*/

HEMP_TYPE_FUNC(hemp_type_code);
HEMP_OUTPUT_FUNC(hemp_type_code_text);



/*--------------------------------------------------------------------------
 * macros
 *--------------------------------------------------------------------------*/

#define hemp_code_new()                         \
    hemp_code_init(NULL)

#define hemp_code_allocate(name)                \
    hemp_allocate_type_name(code, name)


#endif /* HEMP_CODE_H */
