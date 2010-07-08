#ifndef HEMP_TYPES_H
#define HEMP_TYPES_H

#define HEMP_TRUE  1
#define HEMP_FALSE 0

typedef const void *          hemp_mem_t;       /* generic memory pointer   */
typedef void *                hemp_ptr_t;       /* mutable memory pointer   */
typedef unsigned char         hemp_bool_t;      /* boolean true/false       */
typedef unsigned char         hemp_char_t;      /* single character         */
typedef char                * hemp_text_t;      /* C string                 */
typedef const char          * hemp_ident_t;     /* immutable C string       */
typedef long                  hemp_flags_t;     /* bitmask                  */
typedef long                  hemp_offset_t;    /* -ve/+ve offset into list */
typedef unsigned long         hemp_size_t;      /* +ve size of list         */
typedef unsigned long         hemp_pos_t;       /* position in a string     */
//typedef time_t                hemp_time_t;    
  /* seconds since epoch      */

#endif /* HEMP_TYPES_H */