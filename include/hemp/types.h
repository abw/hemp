#ifndef HEMP_TYPES_H
#define HEMP_TYPES_H


/*--------------------------------------------------------------------------
 * Generic data types.  We use hemp_XXX data types internally for the sake 
 * of consistency, simplicity and to abstract away any differences between
 * platforms.
 *--------------------------------------------------------------------------*/

typedef u_int8_t                hemp_u8;        /* generic 8 bit integer    */
typedef u_int16_t               hemp_u16;       /* generic 16 bit integer   */
typedef u_int32_t               hemp_u32;       /* generic 32 bit integer   */
typedef u_int64_t               hemp_u64;       /* generic 64 bit integer   */
typedef u_int32_t               hemp_flags;     /* bitmask                  */
typedef int32_t                 hemp_offset;    /* -ve/+ve offset           */
typedef int32_t                 hemp_int;       /* generic integer          */
typedef double                  hemp_num;       /* generic fp number        */
typedef unsigned long int       hemp_uint;      /* generic unsigned int     */
typedef unsigned long int       hemp_size;      /* +ve size of list         */
typedef unsigned long int       hemp_pos;       /* +ve posn. in string/list */
typedef unsigned short int      hemp_oprec;     /* operator precedence      */
typedef unsigned char           hemp_bool;      /* boolean true/false       */
typedef unsigned char           hemp_char;      /* single character         */
typedef void                  * hemp_memory;    /* generic memory pointer   */
typedef const char            * hemp_name;      /* immutable C string       */
typedef char                  * hemp_string;    /* C string                 */
typedef jmp_buf                 hemp_jump_buf;  /* longjmp buffer           */


/*--------------------------------------------------------------------------
 * hemp data structures
 *--------------------------------------------------------------------------*/

/* values */
typedef enum  hemp_tag_style      hemp_tag_style;
typedef union hemp_value          hemp_value;

/* pointers */
typedef struct hemp_hemp        * hemp_hemp;
typedef struct hemp_action      * hemp_action;
typedef struct hemp_code        * hemp_code;
typedef struct hemp_codec       * hemp_codec;
typedef struct hemp_context     * hemp_context;
typedef struct hemp_dialect     * hemp_dialect;
typedef struct hemp_dialects    * hemp_dialects;
typedef struct hemp_document    * hemp_document;
typedef struct hemp_element     * hemp_element;
typedef struct hemp_elements    * hemp_elements;
typedef struct hemp_error       * hemp_error;
typedef struct hemp_factory     * hemp_factory;
typedef struct hemp_filesystem  * hemp_filesystem;
typedef struct hemp_fragment    * hemp_fragment;
typedef struct hemp_fragments   * hemp_fragments;
typedef struct hemp_frame       * hemp_frame;
typedef struct hemp_global      * hemp_global;
typedef struct hemp_grammar     * hemp_grammar;
typedef struct hemp_grammars    * hemp_grammars;
typedef struct hemp_hash        * hemp_hash;
typedef struct hemp_jump        * hemp_jump;
typedef struct hemp_language    * hemp_language;
typedef struct hemp_list        * hemp_list;
typedef struct hemp_location    * hemp_location;
typedef struct hemp_module      * hemp_module;
typedef struct hemp_namespace   * hemp_namespace;
typedef struct hemp_object      * hemp_object;
typedef struct hemp_params      * hemp_params;
typedef struct hemp_proto       * hemp_proto;
typedef struct hemp_pnode       * hemp_pnode;
typedef struct hemp_pool        * hemp_pool;
typedef struct hemp_ptree       * hemp_ptree;
typedef struct hemp_scan_pos    * hemp_scan_pos;
typedef struct hemp_scheme      * hemp_scheme;
typedef struct hemp_scope       * hemp_scope;
typedef struct hemp_slab        * hemp_slab;
typedef struct hemp_slot        * hemp_slot;
typedef struct hemp_source      * hemp_source;
typedef struct hemp_stack       * hemp_stack;
typedef struct hemp_tag         * hemp_tag;
typedef struct hemp_tags        * hemp_tags;
typedef struct hemp_tagset      * hemp_tagset;
typedef struct hemp_text        * hemp_text;
typedef struct hemp_type        * hemp_type;
typedef struct hemp_uri         * hemp_uri;
typedef struct hemp_viewer      * hemp_viewer;



/*--------------------------------------------------------------------------
 * main hemp data structure
 *--------------------------------------------------------------------------*/

struct hemp_hemp {
    hemp_global     global;

    hemp_factory    codec;
    hemp_factory    dialect;
    hemp_factory    element;
    hemp_factory    grammar;
    hemp_factory    language;
    hemp_factory    scheme;
    hemp_factory    tag;
    hemp_factory    viewer;
                    
    hemp_context    context;
    hemp_hash       config;

//  hemp_hash       tags;
    hemp_hash       documents;
//  hemp_dialect    dialect;
                    
    hemp_bool       verbose;
    hemp_bool       debug;
    hemp_jump       jump;
    hemp_error      error;
    hemp_string   * errmsg;
};



/*--------------------------------------------------------------------------
 * Note that hemp_value is defined here and not in value.h or we end up in 
 * circular header dependency hell.
 *--------------------------------------------------------------------------*/

union hemp_value {
    hemp_u64        bits;
    hemp_num        number;
};


/*--------------------------------------------------------------------------
 * Type definitions for function pointers
 *--------------------------------------------------------------------------*/

typedef hemp_memory
(* hemp_actor)(
    hemp_memory     argument,
    ...
);

typedef hemp_bool
(* hemp_binder)(
    hemp_module     module,
    hemp_hemp       hemp
);

typedef hemp_bool
(* hemp_loader)(
    hemp_module     module
);

typedef hemp_bool
(* hemp_scanner)(
    hemp_memory     self,
    hemp_document   document
);

typedef hemp_string
(* hemp_skipper)(
    hemp_tag        tag,
    hemp_string     src
);




/*--------------------------------------------------------------------------
 * Iterator functions
 *--------------------------------------------------------------------------*/

typedef hemp_bool     
(* hemp_hash_iter)(                 /* iterator over hash items             */
    hemp_hash       hash,           /* pointer to hash                      */
    hemp_pos        index,          /* 0-based index of item in hash        */
    hemp_slot       item            /* pointer to hash item entry           */
);

typedef hemp_bool     
(* hemp_list_iter)(                 /* iterator over list items             */
    hemp_list       list,           /* pointer to list                      */
    hemp_pos        index,          /* 0-based index of item in list        */
    hemp_value      item            /* item value                           */
);

typedef hemp_bool     
(* hemp_pool_iter)(                 /* iterate over pool items              */
    hemp_memory     item            /* pointer to memory                    */
);


/*--------------------------------------------------------------------------
 * Parsing functions
 *--------------------------------------------------------------------------*/

typedef hemp_fragment
(* hemp_prefix)(                    /* parse start of expression            */
    hemp_fragment * fragptr,        /* pointer to current element pointer   */
    hemp_scope      scope,          /* current lexical scope                */
    hemp_oprec      precedence,     /* operator precedence level            */
    hemp_bool       force           /* yes, really parse something          */
);

typedef hemp_fragment  
(* hemp_postfix)(                   /* parse continuation of expression     */
    hemp_fragment * fragptr,        /* pointer to current element pointer   */
    hemp_scope      scope,          /* current lexical scope                */
    hemp_oprec      precedence,     /* operator precedence level            */
    hemp_bool       force,          /* yes, really parse something          */
    hemp_fragment   lhs         /* preceding (lhs) element              */
);

typedef hemp_fragment
(* hemp_fixup)(                     /* general purpose fixup parser handler */
    hemp_fragment   fragptr,        /* pointer to element                   */
    hemp_scope      scope,          /* current lexical scope                */
    hemp_value      fixative        /* optional argument                    */
);


/*--------------------------------------------------------------------------
 * cleanup functions
 *--------------------------------------------------------------------------*/

typedef void
    (* hemp_cleanup)(
        hemp_fragment  fragment   /* pointer to fragment to clean          */
    );


/*--------------------------------------------------------------------------
 * TODO: in the process of cleaning up everything below this line.
 *--------------------------------------------------------------------------*/

// TODO: rename this to hemp_document_f
typedef hemp_document 
    (* hemp_prep_f)(
        hemp_document document    /* pointer to document to prepare       */
    );

typedef hemp_bool 
    (* hemp_scan_f)(
        hemp_document document    /* pointer to document to scan          */
    );

typedef void
    (* hemp_clean_f)(
        hemp_document document    /* pointer to document to clean         */
    );


typedef void
    (* hemp_dclean_f)(
        hemp_dialect  dialect     /* pointer to dialect to clean          */
    );



/*--------------------------------------------------------------------------
 * value functions
 *--------------------------------------------------------------------------*/

typedef hemp_value
    (* hemp_value_f)(
        hemp_value    value,
        hemp_context  context
    );

typedef hemp_value 
    (* hemp_input_f)(
        hemp_value    value,
        hemp_context  context,
        hemp_value    input
    );

typedef hemp_value 
    (* hemp_output_f)(
        hemp_value    value,
        hemp_context  context,
        hemp_value    output
    );






typedef hemp_value 
    (* hemp_view_f)(
        hemp_viewer   view,       /* pointer to viewer                    */
        hemp_fragment fragment,   /* pointer to element                   */
        hemp_context  context,    /* runtime context                      */
        hemp_value    output      /* optional output buffer               */
    );

typedef hemp_value
    (* hemp_fetch_f)(
        hemp_value    container,  /* value to operate on                  */
        hemp_context  context,    /* runtime context                      */
        hemp_value    key         /* key of item to fetch                 */
    );

typedef hemp_value
    (* hemp_store_f)(
        hemp_value    container,  /* value to operate on                  */
        hemp_context  context,    /* runtime context                      */
        hemp_value    key,        /* key to store item under              */
        hemp_value    value       /* value to store                       */
    );

typedef hemp_value
    (* hemp_unop_f)(
        hemp_context  context,    /* runtime context                      */
        hemp_value    expr        /* single value                         */
    );

typedef hemp_value
    (* hemp_binop_f)(
        hemp_context  context,    /* runtime context                      */
        hemp_value    lhs,        /* expression on left hand side         */
        hemp_value    rhs         /* expression on right hand side        */
    );

typedef hemp_type 
    (* hemp_type_f)(
        hemp_int      id,         /* numerical id                         */
        hemp_string      name        /* unique type name                     */
    );

typedef hemp_memory 
    (* hemp_method_f)(
        hemp_type     type,       /* pointer to type                      */
        hemp_string      name        /* method name                          */
    );


#endif /* HEMP_TYPES_H */