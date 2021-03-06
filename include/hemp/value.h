#ifndef HEMP_VALUE_H
#define HEMP_VALUE_H

#include <hemp/core.h>
//#include <hemp/macros.h>
//#include <hemp/type.h>
//#include <hemp/type/text.h>
//#include <hemp/type/hash.h>


/*--------------------------------------------------------------------------
 * global identity (singleton) values
 *--------------------------------------------------------------------------*/

extern const HempValue HempMissing;
extern const HempValue HempNothing;
extern const HempValue HempEmpty;
extern const HempValue HempFalse;
extern const HempValue HempTrue;
extern const HempValue HempBefore;
extern const HempValue HempEqual;
extern const HempValue HempAfter;



/*--------------------------------------------------------------------------
 * Definitions and macros for manipulating NaN-boxed values
 *
 * Hemp values are represented using 64 bit words (one machine word on 64 bit
 * architectures, 2 words on 32 bit) that can store a double precision
 * floating point number, a full-width 32 bit integer, a short sequence of
 * characters, or a memory pointer.  This technique is known as "NaN-boxing".

 * As per IEEE-754 double precision numbers, the first bit is the sign bit,
 * the next 11 bits are the exponent and the remaining 52 are used to encode
 * the mantissa.  If all of the first 12 (sign + exponent) bits are 1 then
 * the value is deemed to be outside the range of tangible numbers that can
 * be represented.  If all the mantissa bits are 0 then it is +/- Infinity.
 * If any other mantissa bit is set then it Not A Number (NaN).  We set the
 * first (highest) mantissa bit to represent the canonical NaN value (this
 * appears to be common practice, if not mandated by IEEE-754).  All the
 * remaining bit patterns from 0xFFF0000000000001 to 0xFFF7FFFFFFFFFFFF,
 * and 0xFFF8000000000001 to 0xFFFFFFFFFFFFFFFF are therefore unused to
 * represent double precision floating point numbers and can be repurposed
 * to carry other payloads.
 *
 * In those 52 bits we can easily store a 32 bit integer.  We can also store
 * a memory pointer.  On 32 bit architectures that also requires 32 bits.  On
 * all widely used (at the time of writing) 64-bit platforms, (up to) 47 bits
 * are used to store memory adddresses (don't worry, that still gives you an
 * addressable memory space of 128Tb - that should be enough for anyone...)
 * That leaves us 5 bits spare which we can use to encode up to 32 different
 * data type tags.
 *
 * So we use the first 5 (high) mantissa bits to encode a type tag.  This is
 * necessarily set to 0 (0b00000) for Infinity and 16 (0x80 / 0b10000) for
 * NaN values, both of which have all the other mantissa bits set to 0.
 * However, this address space is not entirely wasted as we can use either
 * of these tag types to encode further identity values as long as the
 * mantissa has at least one bit set.  For example, we can create unique
 * bit patterns (that can't be generated by any "natural" hemp process) to
 * represent values like Missing, Nothing (both variants of "undefined"),
 * False, True, Before, Equal, After, Finished, and so on.  These are known
 * as "identity values" and are encoded as the values NaN + n.
 *
 * We currently use the lowest 6 bits to define identity values.  Each bit
 * encodes a flag.
 *
 *   000001  ==  0x01  ==  01       Finite (i.e. not the Infinity value)
 *   000010  ==  0x02  ==  02       Found (i.e. not a missing value)
 *   000100  ==  0x04  ==  04       Defined (i.e. not undef, void, null, etc)
 *   001000  ==  0x08  ==  08       Truth (0=false, 1=true)
 *   010000  ==  0x10  ==  16       Not After (less than or equal to)
 *   100000  ==  0x20  ==  32       Not Before (greater than or equal to)
 *
 * These bits are combined to express specific identity values.  For example,
 * the boolean values True and False each have the Defined, Found and Finite
 * bits set and vary in the setting of the Truth bit.  The comparison values
 * Before, Equal and After are tri-state values used by operators like 'cmp'
 * and '<=>' and are analogous to the -1, 0, +1 values typically returned.
 * Before and After are both considered True (in the sense of comparison, it
 * is true that the two values differ) and thus have the same bits set as for
 * the True identity value, while Equal is considered false. We set the
 * Not After bit on Equal and Before, and the Not Before bit on Equal and
 * After.  Thus, Equal can be considered to be neither before nor after.
 * It is admittedly a rather clumsy way of expressing it, but the obvious
 * approach of having "After" and "Before" bits instead of "Not After" and
 * "Not Before" would result in the Equal value having no additional bits
 * set over the False value, and thus there would be no way to tell them
 * apart (which perhaps doesn't really matter in the long run, but this is
 * how it is for now).
 *
 * UPDATE: this next bit isn't true any more, or at the moment....
 *
 * The top two bits gives us a vtable entry as an offset from entry
 * HEMP_TYPE_MASK (16) in the global vtable list.
 *
 *   00     Undefined values: HempMissing, HempNothing
 *   01     Boolean values: HempTrue, HempFalse
 *   10     Comparison values: HempBefore, HempEqual, HempAfter
 *
 * UPDATE: list of types is growing/changing, so don't trust the next bit.
 *
 * The other 30 values that can be encoded in those 5 bits represent different
 * builtin data types.
 *
 *    00 00000  Infinity and other identity values
 *    01 00001  32 bit integers
 *    02 00010  untyped C pointer (e.g. a generic void *)
 *    03 00011  heap allocated C string
 *    ...etc...
 *
 * Double:       [sign x 1][exponent x 11][mantissa x 57]
 * Tagged:       [sign x 1][exponent x 11][type x 5][payload x 47]
 * Pointer(64):  [sign x 1][exponent x 11][type x 5][pointer x 47]
 * Pointer(32):  [sign x 1][exponent x 11][type x 5][empty x 15][pointer x 32]
 * Integer:      [sign x 1][exponent x 11][type x 5][empty x 15][integer x 32]
 *--------------------------------------------------------------------------*/

/* tag types */
#define HEMP_NUMBER_ID          0x00        /* 64 bit double                */
#define HEMP_INTEGER_ID         0x01        /* 32 bit integer               */
#define HEMP_POINTER_ID         0x02        /* memory pointer/string        */
#define HEMP_STRING_ID          0x03        /* memory pointer/string        */
#define HEMP_TEXT_ID            0x04        /* HempText pointer            */
#define HEMP_LIST_ID            0x05        /* HempList pointer            */
#define HEMP_HASH_ID            0x06        /* HempHash pointer            */
#define HEMP_CODE_ID            0x07        /* HempCode pointer            */
#define HEMP_PARAMS_ID          0x08        /* HempParams pointer          */
#define HEMP_OBJECT_ID          0x0F        /* HempObject pointer          */
#define HEMP_IDENTITY_ID        0x10        /* identity values (NaN + n)    */

/* out-of-band values used to mark unused/reserved type slots */
#define HEMP_UBER_ID            0x20        /* type uber type               */
#define HEMP_VALUE_ID           0x21        /* value uber type              */
#define HEMP_RESERVED_ID        0x30        /* reserved for future hemp use */
#define HEMP_UNUSED_ID          0x40        /* available for custom types   */

/* bits to twiddle to encode meaning into identity values */
#define HEMP_FINITE_BIT         0x01        /* not infinity                 */
#define HEMP_FOUND_BIT          0x02        /* not missing                  */
#define HEMP_DEFINED_BIT        0x04        /* not undefined                */
#define HEMP_VALUED_BIT         0x08        /* has a value (not empty)      */
#define HEMP_TRUE_BIT           0x10        /* boolean true/false value     */
#define HEMP_NOT_AFTER_BIT      0x20        /* less than or equal to        */
#define HEMP_NOT_BEFORE_BIT     0x40        /* greater than or equal to     */
#define HEMP_COMPARE_BITS       (HEMP_NOT_BEFORE_BIT | HEMP_NOT_AFTER_BIT)

/* twiddle dem bits to define identity values */
#define HEMP_IDENT_MISSING      (HEMP_FINITE_BIT)                         /* 0000001 */
#define HEMP_IDENT_NOTHING      (HEMP_FINITE_BIT | HEMP_FOUND_BIT)        /* 0000011 */
#define HEMP_IDENT_EMPTY        (HEMP_IDENT_NOTHING | HEMP_DEFINED_BIT)   /* 0000111 */
#define HEMP_IDENT_VALUED       (HEMP_IDENT_EMPTY   | HEMP_VALUED_BIT)    /* 0001111 */
#define HEMP_IDENT_FALSE        (HEMP_IDENT_VALUED)                       /* 0001111 */
#define HEMP_IDENT_TRUE         (HEMP_IDENT_FALSE | HEMP_TRUE_BIT)        /* 0011111 */
#define HEMP_IDENT_BEFORE       (HEMP_IDENT_TRUE  | HEMP_NOT_AFTER_BIT)   /* 0111111 */
#define HEMP_IDENT_AFTER        (HEMP_IDENT_TRUE  | HEMP_NOT_BEFORE_BIT)  /* 1011111 */
#define HEMP_IDENT_EQUAL        (HEMP_IDENT_FALSE | HEMP_COMPARE_BITS)    /* 1111111 */

/* special identity values */
#define HEMP_INFINITY           0xFFF0000000000000ULL
#define HEMP_NAN                0xFFF8000000000000ULL
#define HEMP_IDENTITY           0xFFF8000000000001ULL

/* The type tag is 5 bits wide, offset 47 bits from the LSB */
#define HEMP_TAG_SHIFT          47
#define HEMP_TAG_MASK           0x1F
#define HEMP_TAG_UP(t)          ((HempU64) (t & HEMP_TAG_MASK) << HEMP_TAG_SHIFT)
#define HEMP_TAG_DOWN(v)        ((HempU8)  (v >> HEMP_TAG_SHIFT) & HEMP_TAG_MASK)
#define HEMP_TAG_MAKE(t)        (HEMP_INFINITY | HEMP_TAG_UP(t))
#define HEMP_TAG_TYPE(v)        HEMP_TAG_DOWN(v.bits)
#define HEMP_TAG_VALID(v)       ((HempBool) ((v.bits & HEMP_INFINITY) == HEMP_INFINITY))

/* internal macros to fetch/test the type identifier in a tagged value */
#define HEMP_TYPE_ID(v)         (HEMP_TAG_VALID(v) ? HEMP_TAG_TYPE(v) : HEMP_NUMBER_ID)
#define HEMP_TYPE_IS(v,t)       (HEMP_TYPE_ID(v) == t)
#define HEMP_TYPE_MAX(v,t)      (HEMP_TYPE_ID(v) <= t)
#define HEMP_TYPE_BELOW(v,t)    (HEMP_TYPE_ID(v) < t)

/* macros for manipulating identity values */
#define HEMP_IDENT_MASK         0xFF
#define HEMP_IDENT_MAKE(t)      ((HempValue) (HEMP_IDENTITY | t))

/* TODO: this isn't right - it doesn't check that the lower 4 bits of the
 * identity tag are 0.  It works for now because we don't have any type
 * tags >= 0xFFF8000000000000
 */
#define HEMP_IDENT_VALID(v)     ((HempBool) ((v.bits & HEMP_IDENTITY) == HEMP_IDENTITY))
#define HEMP_IDENT_VALUE(v)     ((HempU8) (v.bits & HEMP_IDENT_MASK))
#define HEMP_IDENT_ID(v)        ((HempU8) HEMP_IDENT_VALID(v) ? HEMP_IDENT_VALUE(v) : 0)
#define HEMP_IDENT_IS(v,i)      ((HempBool) (v.bits == (HEMP_IDENTITY | i)))
#define HEMP_IDENT_MAX(v,b)     ((HempBool) (HEMP_IDENT_ID(v) ^ b))
#define HEMP_IDENT_ANY(v,b)     ((HempBool) (HEMP_IDENT_ID(v) & b))
#define HEMP_IDENT_ALL(v,b)     ((HempBool) (HEMP_IDENT_ANY(v,b) == b))
#define HEMP_IDENT_NOT(v,b)     ((HempBool) (HEMP_IDENT_VALID(v) && (HEMP_IDENT_ANY(v,b) == 0)))

/* Integers are represented in the lower 32 bits */
#define HEMP_INTEGER_BITS       32
#define HEMP_INTEGER_MASK       0xFFFFFFFFL

/* Pointers use the lower 32 bits on 32 bit machines, 47 bits on 64 bit */
#if HEMP_WORD_LENGTH == 32
    #define HEMP_POINTER_BITS   32
    #define HEMP_POINTER_MASK   0xFFFFFFFFL
    #define HEMP_POINTER(v)     ((HempMemory)((HempU32) v.bits))
    #define HEMP_POINTER_UP(p)  ((HempU64) ((HempU32) p & HEMP_POINTER_MASK));
#elif HEMP_WORD_LENGTH == 64
    #define HEMP_POINTER_BITS   47
    #define HEMP_POINTER_MASK   0x00007FFFFFFFFFFFLL
    #define HEMP_POINTER(v)     ((HempMemory)(v.bits & HEMP_POINTER_MASK))
    #define HEMP_POINTER_UP(p)  ((HempU64) p & HEMP_POINTER_MASK);
#else
    #error "Invalid word length"
#endif

/* type tags */
#define HEMP_INTEGER_TAG        HEMP_TAG_MAKE(HEMP_INTEGER_ID)
#define HEMP_POINTER_TAG        HEMP_TAG_MAKE(HEMP_POINTER_ID)
#define HEMP_STRING_TAG         HEMP_TAG_MAKE(HEMP_STRING_ID)
#define HEMP_TEXT_TAG           HEMP_TAG_MAKE(HEMP_TEXT_ID)
#define HEMP_LIST_TAG           HEMP_TAG_MAKE(HEMP_LIST_ID)
#define HEMP_HASH_TAG           HEMP_TAG_MAKE(HEMP_HASH_ID)
#define HEMP_CODE_TAG           HEMP_TAG_MAKE(HEMP_CODE_ID)
#define HEMP_PARAMS_TAG         HEMP_TAG_MAKE(HEMP_PARAMS_ID)
#define HEMP_OBJECT_TAG         HEMP_TAG_MAKE(HEMP_OBJECT_ID)
#define HEMP_IDENTITY_TAG       HEMP_TAG_MAKE(HEMP_IDENTITY_ID)


/*--------------------------------------------------------------------------
 * public-facing macros for general use
 *--------------------------------------------------------------------------*/

#define hemp_is_tagged(v)       HEMP_TAG_VALID(v)
#define hemp_is_number(v)       ((HempU64) v.bits < HEMP_INFINITY)
#define hemp_is_numeric(v)      HEMP_TYPE_MAX(v, HEMP_INTEGER_ID)
#define hemp_is_integer(v)      HEMP_TYPE_IS(v, HEMP_INTEGER_ID)
#define hemp_is_pointer(v)      HEMP_TYPE_IS(v, HEMP_POINTER_ID)
#define hemp_is_string(v)       HEMP_TYPE_IS(v, HEMP_STRING_ID)
#define hemp_is_text(v)         HEMP_TYPE_IS(v, HEMP_TEXT_ID)
#define hemp_is_list(v)         HEMP_TYPE_IS(v, HEMP_LIST_ID)
#define hemp_is_hash(v)         HEMP_TYPE_IS(v, HEMP_HASH_ID)
#define hemp_is_code(v)         HEMP_TYPE_IS(v, HEMP_CODE_ID)
#define hemp_is_params(v)       HEMP_TYPE_IS(v, HEMP_PARAMS_ID)
#define hemp_is_object(v)       HEMP_TYPE_IS(v, HEMP_OBJECT_ID)
#define hemp_is_identity(v)     HEMP_TYPE_IS(v, HEMP_IDENTITY_ID)
#define hemp_is_missing(v)      HEMP_IDENT_NOT(v, HEMP_FOUND_BIT)
#define hemp_is_found(v)        (! hemp_is_missing(v))
#define hemp_is_nothing(v)      HEMP_IDENT_IS(v, HEMP_IDENT_NOTHING)
#define hemp_is_undefined(v)    HEMP_IDENT_NOT(v, HEMP_DEFINED_BIT)
#define hemp_is_defined(v)      (! hemp_is_undefined(v))        /* HEMP_IDENT_ANY(v, HEMP_DEFINED_BIT) */
#define hemp_is_undef(v)        HEMP_IDENT_NOT(v, HEMP_DEFINED_BIT)
#define hemp_is_empty(v)        HEMP_IDENT_IS(v, HEMP_IDENT_EMPTY)
#define hemp_is_valued(v)       HEMP_IDENT_ANY(v, HEMP_VALUED_BIT)
#define hemp_is_boolean(v)      HEMP_IDENT_ANY(v, HEMP_FINITE_BIT)
#define hemp_is_true(v)         HEMP_IDENT_ANY(v, HEMP_TRUE_BIT)
#define hemp_is_false(v)        HEMP_IDENT_NOT(v, HEMP_TRUE_BIT)
#define hemp_is_compare(v)      HEMP_IDENT_ANY(v, HEMP_COMPARE_BITS)
#define hemp_is_before(v)       HEMP_IDENT_IS(v, HEMP_IDENT_BEFORE)
#define hemp_is_after(v)        HEMP_IDENT_IS(v, HEMP_IDENT_AFTER)
#define hemp_is_equal(v)        HEMP_IDENT_IS(v, HEMP_IDENT_EQUAL)

/* hgtype:  hemp global type,
 * obtype: value object type (value contains pointer to object with ->type)
 * pobtype: pointer to object type (ditto)
 */
#define hemp_hgtype(v)          (hemp_global_types[HEMP_TYPE_ID(v)])
#define hemp_obtype(v)          (((HempObject) HEMP_POINTER(v))->type)
#define hemp_pobtype(p)         (p->type)
#define hemp_vtype(v)           (hemp_is_object(v) ? hemp_obtype(v) : hemp_hgtype(v))
#define hemp_obcall(v,n,...)    (hemp_obtype(v)->n(v,__VA_ARGS__))
#define hemp_pobcall(v,n,...)   (hemp_pobtype(v)->n(v,__VA_ARGS__))
#define hemp_tfunc(v,n)         (hemp_vtype(v)->n)
#define hemp_call(v,n,...)      (hemp_tfunc(v,n)(v,__VA_ARGS__))
#define hemp_dot(v,c,k)         (hemp_tfunc(v,dot)(v,c,k))
#define hemp_vtext(v,c,o)       (hemp_tfunc(v,text)(v,c,o))
#define hemp_values(v,c,o)      (hemp_tfunc(v,values)(v,c,o))
#define hemp_fetch(v,c,k)       (hemp_tfunc(v,fetch)(v,c,k))
#define hemp_store(v,c,k,i)     (hemp_tfunc(v,store)(v,c,k,i))
#define hemp_type_name(v)       (hemp_vtype(v)->name)
#define hemp_type_method(t,m)   ((HempEval) hemp_hash_fetch_pointer(t->methods, m))
#define hemp_type_extend(t,m,f) hemp_hash_store_pointer((t)->methods, m, f)
#define hemp_object_method(o,m) hemp_type_method(hemp_vtype(o), m)
#define hemp_send(o,m,c) ({                                 \
    HempEval _hemp_method = hemp_object_method(o, m);       \
    _hemp_method                                            \
        ? _hemp_method(o,c)                                 \
        : HempMissing;                                      \
})


//#define hemp_ident_name(v)      (hemp_identity_name(HEMP_IDENT_ID(v)))
//#define hemp_type_class(v)      (hemp_vtype(v).name)

#define hemp_to_number(v,c)     (hemp_is_number(v)  ? v : hemp_call(v,number,c))
#define hemp_to_integer(v,c)    (hemp_is_integer(v) ? v : hemp_call(v,integer,c))
#define hemp_to_boolean(v,c)    (hemp_is_boolean(v) ? v : hemp_call(v,boolean,c))
#define hemp_to_compare(v,c)    (hemp_is_compare(v) ? v : hemp_call(v,compare,c))
#define hemp_to_text(v,c)       (hemp_is_text(v)    ? v : hemp_vtext(v,c,HempNothing))
#define hemp_to_string(v,c)     hemp_value_to_string(v,c)
#define hemp_onto_text(v,c,o)   hemp_vtext(v,c,o)
#define hemp_blank()            hemp_str_val(HEMP_STR_BLANK)

/*
#define hemp_to_string(v,c) ({  \
    HempValue _v = hemp_is_string(v)
        ? v
        : hemp_vtext(v,c,HempNothing)->string;
    _v->string;
->string)
*/

//#define hemp_is_text(v)         HEMP_TYPE_IS(v, HEMP_TYPE_TEXT_ID)


/*
typedef HempText        (* hemp_text_vfn)(HempValue, HempContext, HempText);
typedef HempValue       (* hemp_dot_vfn)(HempValue, HempContext, HempString);
typedef void            (* hemp_init_vfn)(HempValue);
typedef void            (* hemp_wipe_vfn)(HempValue);
*/


/*--------------------------------------------------------------------------
 * inline functions to encode native values as tagged values
 *--------------------------------------------------------------------------*/

HEMP_INLINE HempValue
hemp_num_val(HempNum n) {
    HempValue v;
    v.number = n;
    return v;
}

HEMP_INLINE HempValue
hemp_int_val(HempInt i) {
    HempValue v;
    v.bits = HEMP_INTEGER_TAG | ((HempU64) i & HEMP_INTEGER_MASK);
    return v;
}

HEMP_INLINE HempValue
hemp_ptr_val(HempMemory p) {
    HempValue v;
    v.bits = HEMP_POINTER_TAG | HEMP_POINTER_UP(p);
    return v;
}

HEMP_INLINE HempValue
hemp_str_val(HempString s) {
    HempValue v;
    v.bits = HEMP_STRING_TAG | HEMP_POINTER_UP(s);
    return v;
}

HEMP_INLINE HempValue
hemp_text_val(HempText t) {
    HempValue v;
    v.bits = HEMP_TEXT_TAG | HEMP_POINTER_UP(t);
    return v;
}

HEMP_INLINE HempValue
hemp_list_val(HempList t) {
    HempValue v;
    v.bits = HEMP_LIST_TAG | HEMP_POINTER_UP(t);
    return v;
}

HEMP_INLINE HempValue
hemp_hash_val(HempHash t) {
    HempValue v;
    v.bits = HEMP_HASH_TAG | HEMP_POINTER_UP(t);
    return v;
}

HEMP_INLINE HempValue
hemp_code_val(HempCode c) {
    HempValue v;
    v.bits = HEMP_CODE_TAG | HEMP_POINTER_UP(c);
    return v;
}

HEMP_INLINE HempValue
hemp_params_val(HempParams p) {
    HempValue v;
    v.bits = HEMP_PARAMS_TAG | HEMP_POINTER_UP(p);
    return v;
}

HEMP_INLINE HempValue
hemp_obj_val(HempObject t) {
    HempValue v;
    v.bits = HEMP_OBJECT_TAG | HEMP_POINTER_UP(t);
    return v;
}

HEMP_INLINE HempValue
hemp_ident_val(HempU8 i) {
    HempValue v;
    v.bits = HEMP_IDENTITY_TAG | ((HempU64) i & HEMP_IDENT_MASK);
    return v;
}

HEMP_INLINE HempValue
hemp_bool_val(HempBool b) {
    return b
        ? HempTrue
        : HempFalse;
}

HEMP_INLINE HempValue
hemp_frag_val(HempFragment f) {
    return hemp_obj_val((HempObject) f);
}

HEMP_INLINE HempValue
hemp_type_val(
    HempType type,
    HempMemory  ptr
) {
    HempValue v;
    v.bits = HEMP_TAG_MAKE(type->id) | HEMP_POINTER_UP(ptr);
    return v;
}


/*--------------------------------------------------------------------------
 * inline functions to decode tagged values to native values
 *--------------------------------------------------------------------------*/

HEMP_INLINE HempNum
hemp_val_num(HempValue v) {
    return v.number;
}

HEMP_INLINE HempInt
hemp_val_int(HempValue v) {
    return (HempInt)(v.bits & HEMP_INTEGER_MASK);
}

HEMP_INLINE HempMemory
hemp_val_ptr(HempValue v) {
    return (HempMemory) HEMP_POINTER(v);
}

HEMP_INLINE HempString
hemp_val_str(HempValue v) {
    return (HempString) HEMP_POINTER(v);
}

HEMP_INLINE HempText
hemp_val_text(HempValue v) {
    return (HempText) HEMP_POINTER(v);
}

HEMP_INLINE HempList
hemp_val_list(HempValue v) {
    return (HempList) HEMP_POINTER(v);
}

HEMP_INLINE HempHash
hemp_val_hash(HempValue v) {
    return (HempHash) HEMP_POINTER(v);
}

HEMP_INLINE HempCode
hemp_val_code(HempValue v) {
    return (HempCode) HEMP_POINTER(v);
}

HEMP_INLINE HempParams
hemp_val_params(HempValue v) {
    return (HempParams) HEMP_POINTER(v);
}

HEMP_INLINE HempObject
hemp_val_obj(HempValue v) {
    return (HempObject) HEMP_POINTER(v);
}

HEMP_INLINE HempFragment
hemp_val_frag(HempValue v) {
    return (HempFragment) HEMP_POINTER(v);
}

HEMP_INLINE HempBool
hemp_val_bool(HempValue v) {
    return hemp_is_true(v)
        ? HEMP_TRUE
        : HEMP_FALSE;
}

/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

void
hemp_value_free(
    HempValue       value
);

HempString
hemp_value_to_string(
    HempValue       value,
    HempContext     context
);

HempString
hemp_identity_name(
    HempValue       value
);


/* generic value evalaution */
extern HEMP_VALUE(hemp_value_no_op);
extern HEMP_VALUE(hemp_value_self);
extern HEMP_VALUE(hemp_value_true);
extern HEMP_VALUE(hemp_value_false);
extern HEMP_FETCH(hemp_value_dot);

extern HEMP_VALUE(hemp_value_apply);

extern HEMP_OUTPUT(hemp_value_values);
extern HEMP_OUTPUT(hemp_value_params);
extern HEMP_OUTPUT(hemp_value_pairs);

/* default "cannot convert to X" function */
extern HEMP_OUTPUT(hemp_value_not_text);
extern HEMP_OUTPUT(hemp_value_not_pairs);
extern HEMP_VALUE(hemp_value_not_number);
extern HEMP_VALUE(hemp_value_not_integer);
extern HEMP_VALUE(hemp_value_not_boolean);
extern HEMP_VALUE(hemp_value_not_compare);
extern HEMP_VALUE(hemp_value_not_defined);
extern HEMP_FETCH(hemp_value_not_fetch);
extern HEMP_STORE(hemp_value_not_store);
extern HEMP_FETCH(hemp_value_not_dot);

/* number -> xxx conversion */
extern HEMP_OUTPUT(hemp_type_number_text);
extern HEMP_VALUE(hemp_type_number_integer);
extern HEMP_VALUE(hemp_type_number_boolean);
extern HEMP_VALUE(hemp_type_number_compare);

/* integer -> xxx conversion */
extern HEMP_OUTPUT(hemp_type_integer_text);
extern HEMP_VALUE(hemp_type_integer_number);
extern HEMP_VALUE(hemp_type_integer_boolean);
extern HEMP_VALUE(hemp_type_integer_compare);

/* identity -> xxx conversions */
extern HEMP_OUTPUT(hemp_type_identity_text);
extern HEMP_VALUE(hemp_type_identity_number);
extern HEMP_VALUE(hemp_type_identity_integer);
extern HEMP_VALUE(hemp_type_identity_defined);
extern HEMP_VALUE(hemp_type_identity_boolean);
extern HEMP_VALUE(hemp_type_identity_compare);

/* prototypes for text, list and hash are in the respective type/XXX.h files */

/* debugging functions */
void hemp_dump_u64(HempU64 value);
void hemp_dump_64(HempU64 value);
void hemp_dump_32(HempU32 value);
void hemp_dump_value(HempValue value);



#endif /* HEMP_VALUE_H */
