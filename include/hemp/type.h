#ifndef HEMP_TYPE_H
#define HEMP_TYPE_H

#include <hemp/core.h>


/*--------------------------------------------------------------------------
 * Data structures
 *
 * A hemp data type defines a core set of functions for manipulating the
 * data and converting it to other core data types.  A hemp object is 
 * loosely defined as any data structure starting with a type pointer.
 *--------------------------------------------------------------------------*/

struct hemp_type_s {
    hemp_type_p     type;       /* allows types to be treated like objects  */
    hemp_int_t      id;
    hemp_str_p      name;
    hemp_type_p     base;
    hemp_hash_p     methods;
//    hemp_method_f   method;     /* TODO: method auto-generator */
    hemp_vtext_f    text;
    hemp_vlist_f    list;
//    hemp_value_f    value;
    hemp_value_f    number;
    hemp_value_f    integer;
    hemp_value_f    boolean;
    hemp_value_f    compare;
    hemp_value_f    defined;
    hemp_value_f    apply;
    hemp_values_f   values;
    hemp_fetch_f    fetch;
    hemp_store_f    store;
    hemp_fetch_f    dot;
};

struct hemp_object_s {
    hemp_type_p     type;
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
 
extern hemp_type_p HempType;
extern hemp_type_p HempValue;
extern hemp_type_p HempNumber;
extern hemp_type_p HempInteger;
extern hemp_type_p HempString;
extern hemp_type_p HempText;
extern hemp_type_p HempList;
extern hemp_type_p HempHash;
extern hemp_type_p HempObject;
extern hemp_type_p HempIdentity;
extern hemp_type_p HempReserved;
extern hemp_type_p HempUnused;
extern hemp_type_p hemp_global_types[HEMP_TYPES_SIZE];


/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

hemp_type_p
    hemp_type_init(
        hemp_int_t      id,
        hemp_str_p      name
    );

hemp_type_p 
    hemp_type_subtype(
        hemp_type_p     base,
        hemp_int_t      id,
        hemp_str_p      name
    );

void
    hemp_type_free(
        hemp_type_p     type
    );

hemp_type_p
    hemp_use_type(
        hemp_str_p      name,
        hemp_type_f     constructor
    );


void 
    hemp_global_types_init(
        hemp_global_p   global
    );

void hemp_global_types_free(
        hemp_global_p   global
    );


HEMP_TYPE_FUNC(hemp_type_type);
HEMP_TYPE_FUNC(hemp_type_number);
HEMP_TYPE_FUNC(hemp_type_integer);
//HEMP_TYPE_FUNC(hemp_type_string);
//HEMP_TYPE_FUNC(hemp_type_text);
//HEMP_TYPE_FUNC(hemp_type_list);
//HEMP_TYPE_FUNC(hemp_type_hash);
HEMP_TYPE_FUNC(hemp_type_identity);
HEMP_TYPE_FUNC(hemp_type_object);

HEMP_VALUE_FUNC(hemp_method_value_name);
HEMP_VALUE_FUNC(hemp_method_value_text);
HEMP_VALUE_FUNC(hemp_method_value_number);
HEMP_VALUE_FUNC(hemp_method_value_integer);
HEMP_VALUE_FUNC(hemp_method_value_boolean);
HEMP_VALUE_FUNC(hemp_method_value_defined);
HEMP_VALUE_FUNC(hemp_method_value_type);

HEMP_VTEXT_FUNC(hemp_value_type_text);
HEMP_VALUE_FUNC(hemp_method_type_name);
HEMP_VALUE_FUNC(hemp_method_type_id);



#endif /* HEMP_TYPE_H */

