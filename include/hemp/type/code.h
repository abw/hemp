#ifndef HEMP_CODE_H
#define HEMP_CODE_H

#include <hemp/core.h>
#include <hemp/value.h>
#include <hemp/proto.h>


/*--------------------------------------------------------------------------
 * type definitions
 *--------------------------------------------------------------------------*/

struct hemp_code {
    HempValue      body;
    HempProto      proto;
//    HempValue      args;
//  hemp_scope_t    scope;
//  hemp_element_t  source;
}; 



/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

HEMP_INLINE HempCode
hemp_code_init();

HEMP_INLINE void
hemp_code_release(
    HempCode       code
);

HEMP_INLINE void
hemp_code_free(
    HempCode       code
);

HEMP_INLINE HempProto
hemp_code_proto(
    HempCode       code
);

/*--------------------------------------------------------------------------
 * type functions and methods
 *--------------------------------------------------------------------------*/

HEMP_TYPE_FUNC(hemp_type_code);
HEMP_OUTPUT(hemp_type_code_text);



/*--------------------------------------------------------------------------
 * macros
 *--------------------------------------------------------------------------*/

#define hemp_code_new()                         \
    hemp_code_init(NULL)


#endif /* HEMP_CODE_H */
