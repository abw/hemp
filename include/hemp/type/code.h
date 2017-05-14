#ifndef HEMP_CODE_H
#define HEMP_CODE_H

#include <hemp/core.h>
#include <hemp/value.h>
#include <hemp/proto.h>


/*--------------------------------------------------------------------------
 * Type definitions
 *--------------------------------------------------------------------------*/

struct hemp_code {
    HempValue       body;
    HempProto       proto;
//  HempValue       args;
//  hemp_scope_t    scope;
//  hemp_element_t  source;
};


/*--------------------------------------------------------------------------
 * Function prototypes
 *--------------------------------------------------------------------------*/

HempCode
hemp_code_init();

void
hemp_code_release(
    HempCode        code
);

void
hemp_code_free(
    HempCode        code
);

HempProto
hemp_code_proto(
    HempCode        code
);


/*--------------------------------------------------------------------------
 * Type functions and methods
 *--------------------------------------------------------------------------*/

HEMP_TYPE(hemp_type_code);
HEMP_OUTPUT(hemp_type_code_text);


/*--------------------------------------------------------------------------
 * Macros
 *--------------------------------------------------------------------------*/

#define hemp_code_new()                         \
    hemp_code_init(NULL)


#endif /* HEMP_CODE_H */
