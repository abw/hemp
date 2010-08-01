#ifndef HEMP_TYPES_H
#define HEMP_TYPES_H

#include <stdarg.h>


#define HEMP_TRUE  1
#define HEMP_FALSE 0


/*--------------------------------------------------------------------------
 * generic types
 *--------------------------------------------------------------------------*/

typedef unsigned char             hemp_bool_t;    /* boolean true/false       */
typedef unsigned char             hemp_char_t;    /* single character         */
typedef long                      hemp_flags_t;   /* bitmask                  */
typedef long                      hemp_offset_t;  /* -ve/+ve offset           */
typedef long                      hemp_int_t;     /* generic integer          */
typedef double                    hemp_num_t;     /* generic fp number        */
typedef unsigned long             hemp_size_t;    /* +ve size of list         */
typedef unsigned long             hemp_pos_t;     /* +ve posn. in string/list */
typedef unsigned short int        hemp_prec_t;    /* operator precedence      */
typedef const void              * hemp_mem_t;     /* generic memory pointer   */
typedef void                    * hemp_ptr_t;     /* mutable memory pointer   */
typedef void                    * hemp_item_t;    /* alias for typed data ptr */
typedef char                    * hemp_cstr_t;    /* C string                 */
typedef const char              * hemp_name_t;    /* immutable C string       */


/*--------------------------------------------------------------------------
 * hemp data structures
 *--------------------------------------------------------------------------*/

typedef struct hemp             * hemp_t;
typedef struct hemp_context     * hemp_context_t;
typedef struct hemp_dialect     * hemp_dialect_t;
typedef struct hemp_etype       * hemp_etype_t;
typedef struct hemp_element     * hemp_element_t;
typedef struct hemp_elements    * hemp_elements_t;
typedef struct hemp_filesystem  * hemp_filesystem_t;
typedef struct hemp_hash        * hemp_hash_t;
typedef struct hemp_hash_entry  * hemp_hash_entry_t;
typedef struct hemp_hub         * hemp_hub_t;
typedef struct hemp_list        * hemp_list_t;
typedef struct hemp_pool        * hemp_pool_t;
typedef struct hemp_pnode       * hemp_pnode_t;
typedef struct hemp_ptree       * hemp_ptree_t;
typedef struct hemp_scheme      * hemp_scheme_t;
typedef struct hemp_scope       * hemp_scope_t;
typedef struct hemp_source      * hemp_source_t;
typedef struct hemp_slab        * hemp_slab_t;
typedef struct hemp_tag         * hemp_tag_t;
typedef struct hemp_tagset      * hemp_tagset_t;
typedef struct hemp_template    * hemp_template_t;
typedef struct hemp_text        * hemp_text_t;
typedef struct hemp_type        * hemp_type_t;
typedef struct hemp_vtype       * hemp_vtype_t;
typedef struct hemp_vtypes      * hemp_vtypes_t;
typedef struct hemp_value       * hemp_value_t;

/* don't think these should be pointers */
typedef union  hemp_evalue      * hemp_evalue_t;
typedef struct hemp_unary       * hemp_unary_t;
typedef struct hemp_binary      * hemp_binary_t;
typedef struct hemp_block       * hemp_block_t;


/*--------------------------------------------------------------------------
 * function pointers
 *--------------------------------------------------------------------------*/

typedef hemp_size_t     (* hemp_hash_fn)(hemp_cstr_t); 
typedef hemp_bool_t     (* hemp_scan_fn)(hemp_template_t);
typedef hemp_cstr_t     (* hemp_source_text_fn)(hemp_source_t);
typedef hemp_bool_t     (* hemp_source_bool_fn)(hemp_source_t);
typedef hemp_bool_t     (* hemp_build_fn)(hemp_t);
typedef hemp_bool_t     (* hemp_onload_fn)(hemp_t);
typedef hemp_bool_t     (* hemp_list_each_fn)(hemp_list_t, hemp_pos_t, hemp_ptr_t);

//typedef hemp_value_t    (* hemp_init_fn)();
typedef void            (* hemp_free_fn)(hemp_value_t);
typedef hemp_bool_t     (* hemp_truth_fn)(hemp_value_t);
typedef hemp_value_t    (* hemp_unary_fn)(hemp_value_t);
typedef hemp_value_t    (* hemp_binary_fn)(hemp_value_t, hemp_value_t);
typedef hemp_value_t    (* hemp_ternary_fn)(hemp_value_t, hemp_value_t, hemp_value_t);

typedef hemp_text_t     (* hemp_text_fn)(hemp_element_t, hemp_text_t);

/* old object type methods, being replace by those below... */
typedef hemp_ptr_t      (* hemp_init_fn)(hemp_type_t, va_list *args);
typedef void            (* hemp_wipe_fn)(hemp_ptr_t);


/* new object type methods */
typedef hemp_item_t     (* hemp_acquire_fn)(hemp_type_t type);
typedef hemp_bool_t     (* hemp_prepare_fn)(hemp_item_t item, va_list *args);
typedef hemp_bool_t     (* hemp_cleanup_fn)(hemp_item_t item);
typedef void            (* hemp_release_fn)(hemp_item_t item);


typedef void 
    (* hemp_tag_scan_fn)(
        hemp_template_t, 
        hemp_tag_t, 
        hemp_cstr_t, 
        hemp_pos_t, 
        hemp_cstr_t *
    );

typedef hemp_element_t 
    (* hemp_skip_fn)(
        hemp_element_t *elemptr         /* pointer to element, may be advanced */
    );

typedef hemp_element_t 
    (* hemp_parse_fn)(
        hemp_element_t *elemptr,        /* pointer to element, may be advanced */
        hemp_scope_t    scope,          /* pointer to current scope            */
        hemp_prec_t     precedence,     /* operator precedence level           */
        hemp_bool_t     force           /* over-ride precedence to force parse */
    );


#endif /* HEMP_TYPES_H */