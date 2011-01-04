#ifndef HEMP_TYPE_H
#define HEMP_TYPE_H

#include <hemp/core.h>


#define HEMP_TYPE_BASE              \
    HempType        type;           \
    HempType        base;           \
    HempInt         id;             \
    HempString      name;           \
    HempNamespace   namespace;      \
    HempHash        methods;        \
    HempEval        value;          \
    HempEval        number;         \
    HempEval        integer;        \
    HempEval        boolean;        \
    HempEval        compare;        \
    HempEval        defined;        \
    HempInput       apply;          \
    HempInput       assign;         \
    HempOutput      text;           \
    HempOutput      values;         \
    HempOutput      pairs;          \
    HempOutput      params;         \
    HempFetch       dot;            \
    HempFetch       fetch;          \
    HempStore       store;          \
    HempClean       clean;

//  HempOutput       list;       \   /* superfluous? */
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
    HempType        type;
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
 
extern HempType HempTypeType;
extern HempType HempTypeValue;
extern HempType HempTypeNumber;
extern HempType HempTypeInteger;
extern HempType HempTypePointer;
extern HempType HempTypeString;
extern HempType HempTypeText;
extern HempType HempTypeList;
extern HempType HempTypeHash;
extern HempType HempTypeObject;
extern HempType HempTypeIdentity;
extern HempType HempTypeReserved;
extern HempType HempTypeUnused;
extern HempType hemp_global_types[HEMP_TYPES_SIZE];


/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

HempType
hemp_type_new(
    HempInt         id,
    HempString      name
);

HempType 
hemp_type_subtype(
    HempType        base,
    HempInt         id,
    HempString      name
);

void
hemp_type_free(
    HempType        type
);

HempType
hemp_use_type(
    HempString      name,
    HempTypedef     constructor
);


void 
hemp_global_types_init(
    HempGlobal      global
);

void 
hemp_global_types_free(
    HempGlobal      global
);


HEMP_TYPE(hemp_type_type);
HEMP_TYPE(hemp_type_number);
HEMP_TYPE(hemp_type_integer);
HEMP_TYPE(hemp_type_identity);
HEMP_TYPE(hemp_type_object);

HEMP_VALUE(hemp_method_value_name);
HEMP_VALUE(hemp_method_value_text);
HEMP_VALUE(hemp_method_value_number);
HEMP_VALUE(hemp_method_value_integer);
HEMP_VALUE(hemp_method_value_boolean);
HEMP_VALUE(hemp_method_value_defined);
HEMP_VALUE(hemp_method_value_type);

HEMP_OUTPUT(hemp_valueype_text);
HEMP_VALUE(hemp_method_type_name);
HEMP_VALUE(hemp_method_type_id);



#endif /* HEMP_TYPE_H */

