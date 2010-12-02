#ifndef HEMP_TYPE_H
#define HEMP_TYPE_H

#include <hemp/core.h>


#define HEMP_TYPE_BASE              \
    hemp_type       type;           \
    hemp_type       base;           \
    hemp_int        id;             \
    hemp_string     name;           \
    hemp_namespace  namespace;      \
    hemp_hash       methods;        \
    hemp_value_f    value;          \
    hemp_value_f    number;         \
    hemp_value_f    integer;        \
    hemp_value_f    boolean;        \
    hemp_value_f    compare;        \
    hemp_value_f    defined;        \
    hemp_input_f    apply;          \
    hemp_input_f    assign;         \
    hemp_output_f   text;           \
    hemp_output_f   values;         \
    hemp_output_f   pairs;          \
    hemp_output_f   params;         \
    hemp_fetch_f    dot;            \
    hemp_fetch_f    fetch;          \
    hemp_store_f    store;          

//  hemp_output_f       list;       \   /* superfluous? */
//  hemp_method_f       method;         /* TODO: method auto-generator */




/*--------------------------------------------------------------------------
 * Data structures
 *
 * A hemp data type defines a core set of functions for manipulating the
 * data and converting it to other core data types.  A hemp object is 
 * loosely defined as any data structure starting with a type pointer.
 *--------------------------------------------------------------------------*/

struct hemp_type {
    HEMP_TYPE_BASE
};

struct hemp_object {
    hemp_type       type;
};

/* other stuff to think about adding:
    # memory management
    acquire         # allocate memory
    prepare         # initialise object
    cleanup         # cleanup object
    release         # release memory

    # data representation
    values
    pairs
*/


/*--------------------------------------------------------------------------
 * core types
 *--------------------------------------------------------------------------*/
 
extern hemp_type HempType;
extern hemp_type HempValue;
extern hemp_type HempNumber;
extern hemp_type HempInteger;
extern hemp_type HempPointer;
extern hemp_type HempString;
extern hemp_type HempText;
extern hemp_type HempList;
extern hemp_type HempHash;
extern hemp_type HempObject;
extern hemp_type HempIdentity;
extern hemp_type HempReserved;
extern hemp_type HempUnused;
extern hemp_type hemp_global_types[HEMP_TYPES_SIZE];


/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

hemp_type
hemp_type_new(
    hemp_int        id,
    hemp_string     name
);

hemp_type 
hemp_type_subtype(
    hemp_type       base,
    hemp_int        id,
    hemp_string     name
);

void
hemp_type_free(
    hemp_type       type
);

hemp_type
hemp_use_type(
    hemp_string     name,
    hemp_type_f     constructor
);


void 
hemp_global_types_init(
    hemp_global     global
);

void 
hemp_global_types_free(
    hemp_global     global
);


HEMP_TYPE_FUNC(hemp_type_type);
HEMP_TYPE_FUNC(hemp_type_number);
HEMP_TYPE_FUNC(hemp_type_integer);
HEMP_TYPE_FUNC(hemp_type_identity);
HEMP_TYPE_FUNC(hemp_type_object);

HEMP_VALUE_FUNC(hemp_method_value_name);
HEMP_VALUE_FUNC(hemp_method_value_text);
HEMP_VALUE_FUNC(hemp_method_value_number);
HEMP_VALUE_FUNC(hemp_method_value_integer);
HEMP_VALUE_FUNC(hemp_method_value_boolean);
HEMP_VALUE_FUNC(hemp_method_value_defined);
HEMP_VALUE_FUNC(hemp_method_value_type);

HEMP_OUTPUT_FUNC(hemp_valueype_text);
HEMP_VALUE_FUNC(hemp_method_type_name);
HEMP_VALUE_FUNC(hemp_method_type_id);



#endif /* HEMP_TYPE_H */

