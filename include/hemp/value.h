#ifndef HEMP_VALUE_H
#define HEMP_VALUE_H

#include <string.h>
#include <stdlib.h>
#include "hemp/types.h"
#include "hemp/defaults.h"
#include "hemp/memory.h"
#include "hemp/hash.h"


typedef union hemp_data    hemp_data_t;
typedef hemp_text_t     (* hemp_text_vfn)(hemp_value_t, hemp_context_t, hemp_text_t);
typedef hemp_value_t    (* hemp_dot_vfn)(hemp_value_t, hemp_context_t, hemp_cstr_t);
typedef void            (* hemp_init_vfn)(hemp_value_t);
typedef void            (* hemp_wipe_vfn)(hemp_value_t);



/* The hemp_data union can hold any basic data type */

union hemp_data {
    hemp_cstr_t         cstr;
    hemp_text_t         text;
    hemp_int_t          integer;
    hemp_num_t          number;
    hemp_hash_t         hash;
    hemp_list_t         list;
};


/* A hemp_value represents a typed data value being used in some expression.
 * It contains a reference to its value type, the parent value (i.e. container)
 * from where it originated, the local (variable) name for the value, and a 
 * hemp_data union containing the raw data value.  For example, foo.bar would
 * create a 'foo' value originated from the root variables stash, and a 'bar'
 * value with the parent pointer set to the 'foo' value.  The type of 'foo'
 * would presumably be a hash or object, and bar could be any value type.
 */
 
struct hemp_value {
    hemp_vtype_t  vtype;
    hemp_cstr_t   name;             // TODO: symbol?  dup?  element?  text?
    hemp_data_t   data;
    hemp_value_t  parent;
//  TODO: flags?, next?
};


/* A hemp_vtype defines a lookup table (aka vtable) of functions (aka methods)
 * which perform the basic data operations on values of different types.
 * e.g. the 'dot' method for accessing a dotted part of a value, e.g. foo.bar.
 * It also contains a value type name and a reference to the hemp_types 
 * collection of which it is a part.  e.g. the "hemp" hemp_vtypes collection
 * contains hemp_vtype definitions for values using the builtin hemp_text,
 * hemp_list, hemp_hash, etc., data structures (aka objects).  A some point 
 * in the future a "perl" hemp_vtypes collection can provide hemp_vtype 
 * definitions for accessing Perl scalars, hash array and arrays.
 */

struct hemp_vtype {
    hemp_vtypes_t   vtypes;
    hemp_cstr_t     name;
    hemp_init_vfn   init;
    hemp_wipe_vfn   wipe;
    hemp_text_vfn   text;
    hemp_dot_vfn    dot;
/*
    set
    dot
    text
    value
    pairs
    dot_set   ??
    apply     # function application
    name fullname
*/
};




/* The hemp_vtypes data structure defines a collection of hemp_vtype value
 * types.  It is essentially a factory for creating value instances of 
 * different types.  The builtin data types (text, list, hash, etc) have
 * hard-coded entries in the data structure that can be accessed directly 
 * for the sake of efficiency.  Any other custom value types can be defined
 * in the types hash table which offers unlimited extensibility at the cost
 * of slower lookup time.
 */

struct hemp_vtypes {
    hemp_cstr_t     name;
    /* builtin value types */
    hemp_vtype_t    text;
    hemp_vtype_t    list;
    hemp_vtype_t    hash;
    /* hash table for additional types */
    hemp_hash_t     types;
};




/* each value has a pointer to its type and the value iteself */
//#define HEMP_VALUE_BASE     \
//    hemp_type_t  type;      \
//    hemp_flags_t flags;     \
//    hemp_value_t next;
    
/* From TT3:

    # variable slots
    META    => '=0',
    CONTEXT => '=1',
    NAME    => '=2',
    VALUE   => '=3',
    PARENT  => '=4',

    # variable metadata slots
    CONFIG  => '=0',
    VARS    => '=1',
    METHODS => '=2',

    We want to unify CONTEXT/PARENT to be one and the same.
    
    foo       ===> [.] <-- [foo]             # parent/context is the root stash 
    bar.baz   ===> [.] <-- [bar] <-- [baz]
*/


/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

hemp_vtypes_t   hemp_vtypes_init();
void            hemp_vtypes_free(hemp_vtypes_t);
hemp_vtype_t    hemp_vtypes_new_type(hemp_vtypes_t, hemp_cstr_t);
void            hemp_vtypes_free_vtype(hemp_hash_entry_t entry);
hemp_vtype_t    hemp_vtypes_type(hemp_vtypes_t, hemp_cstr_t);

hemp_vtype_t    hemp_vtype_init(hemp_vtypes_t, hemp_cstr_t);
void            hemp_vtype_free();

hemp_value_t    hemp_value_init(hemp_vtype_t, hemp_cstr_t, hemp_data_t, hemp_value_t);
void            hemp_value_free(hemp_value_t);


#endif /* HEMP_VALUE_H */
