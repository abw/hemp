#ifndef HEMP_TYPES_H
#define HEMP_TYPES_H

#define HEMP_TRUE  1
#define HEMP_FALSE 0


/*--------------------------------------------------------------------------
 * generic types
 *--------------------------------------------------------------------------*/

typedef const void *              hemp_mem_t;       /* generic memory pointer   */
typedef void *                    hemp_ptr_t;       /* mutable memory pointer   */
typedef unsigned char             hemp_bool_t;      /* boolean true/false       */
typedef unsigned char             hemp_char_t;      /* single character         */
typedef char                    * hemp_cstr_t;      /* C string                 */
typedef const char              * hemp_ident_t;     /* immutable C string       */
typedef long                      hemp_flags_t;     /* bitmask                  */
typedef long                      hemp_offset_t;    /* -ve/+ve offset           */
typedef unsigned long             hemp_size_t;      /* +ve size of list         */
typedef unsigned long             hemp_pos_t;       /* position in a string     */


/*--------------------------------------------------------------------------
 * hemp data structures
 *--------------------------------------------------------------------------*/

typedef struct hemp             * hemp_t;
typedef struct hemp_dialect     * hemp_dialect_t;
typedef struct hemp_etype       * hemp_etype_t;
typedef struct hemp_element     * hemp_element_t;
typedef struct hemp_elements    * hemp_elements_t;
typedef struct hemp_hash        * hemp_hash_t;
typedef struct hemp_hash_entry  * hemp_hash_entry_t;
typedef struct hemp_pool        * hemp_pool_t;
typedef struct hemp_pnode       * hemp_pnode_t;
typedef struct hemp_ptree       * hemp_ptree_t;
typedef struct hemp_scheme      * hemp_scheme_t;
typedef struct hemp_source      * hemp_source_t;
typedef struct hemp_slab        * hemp_slab_t;
typedef struct hemp_tag         * hemp_tag_t;
typedef struct hemp_tagset      * hemp_tagset_t;
typedef struct hemp_template    * hemp_template_t;


/*--------------------------------------------------------------------------
 * function pointers
 *--------------------------------------------------------------------------*/

typedef hemp_size_t            (* hemp_hash_fn)(hemp_cstr_t); 
typedef hemp_bool_t            (* hemp_scan_fn)(hemp_template_t);
typedef hemp_cstr_t            (* hemp_source_text_fn)(hemp_source_t);
typedef hemp_bool_t            (* hemp_source_bool_fn)(hemp_source_t);
typedef hemp_bool_t            (* hemp_build_fn)(hemp_t);
typedef void                   (* hemp_tag_scan_fn)();          // TODO: args



#endif /* HEMP_TYPES_H */