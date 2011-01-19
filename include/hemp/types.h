#ifndef HEMP_TYPES_H
#define HEMP_TYPES_H


/*--------------------------------------------------------------------------
 * Generic data types.  We use HempBlah data types internally for the sake 
 * of consistency, simplicity and to abstract away any differences between
 * platforms.
 *--------------------------------------------------------------------------*/

typedef u_int8_t                  HempU8;       /* generic 8 bit integer    */
typedef u_int16_t                 HempU16;      /* generic 16 bit integer   */
typedef u_int32_t                 HempU32;      /* generic 32 bit integer   */
typedef u_int64_t                 HempU64;      /* generic 64 bit integer   */
typedef u_int32_t                 HempFlags;    /* bitmask                  */
typedef int32_t                   HempOffset;   /* -ve/+ve offset           */
typedef int32_t                   HempInt;      /* generic integer          */
typedef double                    HempNum;      /* generic fp number        */
typedef unsigned long int         HempUint;     /* generic unsigned int     */
typedef unsigned long int         HempSize;     /* +ve size of list         */
typedef unsigned long int         HempPos;      /* +ve posn. in string/list */
typedef unsigned short int        HempPrec;     /* operator precedence      */
typedef unsigned char             HempBool;     /* boolean true/false       */
typedef unsigned char             HempChar;     /* single character         */
typedef void                    * HempMemory;   /* generic memory pointer   */
typedef const char              * HempName;     /* immutable C string       */
typedef char                    * HempString;   /* C string                 */
typedef jmp_buf                   HempJumpBuf;  /* longjmp buffer           */


/*--------------------------------------------------------------------------
 * Hemp data types
 *--------------------------------------------------------------------------*/

typedef enum   hemp_errno         HempErrno;
typedef enum   hemp_jump_type     HempJumpType;
typedef enum   hemp_tag_style     HempTagStyle;

/* values */
typedef struct hemp_unary_op      HempUnaryOp;
typedef struct hemp_binary_op     HempBinaryOp;
typedef struct hemp_block_op      HempBlockOp;
typedef union  hemp_op            HempOp;
typedef union  hemp_value         HempValue;

/* pointers to hemp data structures */
typedef struct hemp             * Hemp;
typedef struct hemp_action      * HempAction;
typedef struct hemp_cache       * HempCache;
typedef struct hemp_code        * HempCode;
typedef struct hemp_codec       * HempCodec;
typedef struct hemp_context     * HempContext;
typedef struct hemp_dialect     * HempDialect;
typedef struct hemp_dialects    * HempDialects;
typedef struct hemp_document    * HempDocument;
typedef struct hemp_element     * HempElement;
typedef struct hemp_elements    * HempElements;
typedef struct hemp_error       * HempError;
typedef struct hemp_factory     * HempFactory;
typedef struct hemp_filesystem  * HempFilesystem;
typedef struct hemp_fragment    * HempFragment;
typedef struct hemp_fragments   * HempFragments;
typedef struct hemp_frame       * HempFrame;
typedef struct hemp_global      * HempGlobal;
typedef struct hemp_grammar     * HempGrammar;
typedef struct hemp_grammars    * HempGrammars;
typedef struct hemp_hash        * HempHash;
typedef struct hemp_jump        * HempJump;
typedef struct hemp_language    * HempLanguage;
typedef struct hemp_list        * HempList;
typedef struct hemp_location    * HempLocation;
typedef struct hemp_md5         * HempMD5;
typedef struct hemp_module      * HempModule;
typedef struct hemp_namespace   * HempNamespace;
typedef struct hemp_object      * HempObject;
typedef struct hemp_params      * HempParams;
typedef struct hemp_proto       * HempProto;
typedef struct hemp_pnode       * HempPnode;
typedef struct hemp_pool        * HempPool;
typedef struct hemp_ptree       * HempPtree;
typedef struct hemp_scan_pos    * HempScanPos;
typedef struct hemp_scheme      * HempScheme;
typedef struct hemp_scope       * HempScope;
typedef struct hemp_slab        * HempSlab;
typedef struct hemp_slot        * HempSlot;
typedef struct hemp_source      * HempSource;
typedef struct hemp_stack       * HempStack;
typedef struct hemp_tag         * HempTag;
typedef struct hemp_tags        * HempTags;
typedef struct hemp_tagset      * HempTagset;
typedef struct hemp_text        * HempText;
typedef struct hemp_type        * HempType;
typedef struct hemp_uri         * HempUri;
typedef struct hemp_viewer      * HempViewer;


/*--------------------------------------------------------------------------
 * main hemp data structure
 *--------------------------------------------------------------------------*/

struct hemp {
    HempGlobal      global;             /* pointer to global data           */
    HempFactory     factory;            /* new meta-factory                 */
    HempFlags       flags;              /* runtime options                  */

    HempCache       cache;              /* cache for compiled documents     */
    HempHash        config;             /* configuration items              */
    HempContext     context;            /* default runtime context          */
    HempHash        documents;          /* OLD: documents hash (-> cache)   */
    HempFilesystem  filesystem;         /* abstraction of file system       */

    /* all these factories are being merged into the new meta-factory */
    HempFactory     codec;
    HempFactory     dialect;
    HempFactory     element;
    HempFactory     feature;
    HempFactory     grammar;
    HempFactory     language;
    HempFactory     scheme;
    HempFactory     tag;
    HempFactory     viewer;
                    

    /* TODO: create one flags item */
    HempBool        verbose;
    HempBool        debug;

    /* jump/error handling */
    HempJump        jump;
    HempError       error;
    HempString    * errmsg;
};



/*--------------------------------------------------------------------------
 * Note that HempValue is defined here and not in value.h or we end up in 
 * circular header dependency hell.
 *--------------------------------------------------------------------------*/

union hemp_value {
    HempU64         bits;
    HempNum         number;
};


/*--------------------------------------------------------------------------
 * Type definitions for function pointers
 *--------------------------------------------------------------------------*/

typedef HempMemory
(* HempActor)(
    HempMemory      argument,
    ...
);

typedef HempBool 
(* HempAutoload)(
    HempFactory     factory,
    HempString      name
);



/*--------------------------------------------------------------------------
 * Module loading and binding function pointers
 *--------------------------------------------------------------------------*/

typedef HempBool
(* HempBinder)(
    HempModule      module,
    Hemp            hemp
);

typedef HempBool
(* HempLoader)(
    HempModule      module
);


/*--------------------------------------------------------------------------
 * Dialect/document function pointers
 *--------------------------------------------------------------------------*/

typedef HempDocument 
(* hemp_doc_prep)(
    HempDocument    document        /* pointer to document to prepare       */
);

typedef HempBool
(* hemp_doc_scan)(
    HempDocument    document        /* pointer to document to scan          */
);

typedef void
(* hemp_doc_clean)(
    HempDocument    document        /* pointer to document to clean         */
);


/*--------------------------------------------------------------------------
 * Scanning function pointers.
 *--------------------------------------------------------------------------*/

typedef HempBool
(* HempScanner)(
    HempMemory      self,
    HempDocument    document
);

typedef HempString
(* HempSkipper)(
    HempTag         tag,
    HempString      src
);

typedef HempTag 
(* HempTagdef)(
    Hemp            hemp,
    HempString      type,
    HempString      name,
    HempString      start,
    HempString      end,
    HempGrammar     grammar
);



/*--------------------------------------------------------------------------
 * Parsing functions
 *--------------------------------------------------------------------------*/

typedef HempFragment
(* HempPrefix)(                    /* parse start of expression            */
    HempFragment  * fragptr,        /* pointer to current element pointer   */
    HempScope       scope,          /* current lexical scope                */
    HempPrec        precedence,     /* operator precedence level            */
    HempBool        force           /* yes, really parse something          */
);

typedef HempFragment
(* HempPostfix)(                   /* parse continuation of expression     */
    HempFragment  * fragptr,        /* pointer to current element pointer   */
    HempScope       scope,          /* current lexical scope                */
    HempPrec        precedence,     /* operator precedence level            */
    HempBool        force,          /* yes, really parse something          */
    HempFragment    lhs             /* preceding (lhs) element              */
);

typedef HempFragment
(* HempFixup)(                     /* general purpose fixup parser handler */
    HempFragment    fragptr,        /* pointer to element                   */
    HempScope       scope,          /* current lexical scope                */
    HempValue       fixative        /* optional argument                    */
);


/*--------------------------------------------------------------------------
 * Iterator functions
 *--------------------------------------------------------------------------*/

typedef HempBool
(* HempEach)(                       /* iterate over values, e.g. in list    */
    HempValue       value
);

typedef HempBool
(* hemp_hash_iter)(                 /* iterator over hash items             */
    HempHash        hash,           /* pointer to hash                      */
    HempPos         index,          /* 0-based index of item in hash        */
    HempSlot        item            /* pointer to hash item entry           */
);

typedef HempBool
(* hemp_list_iter)(                 /* iterator over list items             */
    HempList        list,           /* pointer to list                      */
    HempPos         index,          /* 0-based index of item in list        */
    HempValue       item            /* item value                           */
);

typedef HempBool
(* hemp_pool_iter)(                 /* iterate over pool items              */
    HempMemory      item            /* pointer to memory                    */
);


/*--------------------------------------------------------------------------
 * cleanup functions
 *--------------------------------------------------------------------------*/

typedef void
(* hemp_cleanup)(
    HempFragment    fragment        /* pointer to fragment to clean          */
);


/*--------------------------------------------------------------------------
 * value functions
 *--------------------------------------------------------------------------*/

typedef HempValue
(* HempEval)(
    HempValue       value,
    HempContext     context
);

typedef HempValue 
(* HempInput)(
    HempValue       value,
    HempContext     context,
    HempValue       input
);

typedef HempValue 
(* HempOutput)(
    HempValue       value,
    HempContext     context,
    HempValue       output
);

typedef HempValue 
(* hemp_view_f)(
    HempViewer      viewer,         /* pointer to viewer                    */
    HempFragment    fragment,       /* pointer to element                   */
    HempContext     context,        /* runtime context                      */
    HempValue       output          /* optional output buffer               */
);

typedef HempValue
(* HempFetch)(
    HempValue       container,      /* value to operate on                  */
    HempContext     context,        /* runtime context                      */
    HempValue       key             /* key of item to fetch                 */
);

typedef HempValue
(* HempStore)(
    HempValue       container,      /* value to operate on                  */
    HempContext     context,        /* runtime context                      */
    HempValue       key,            /* key to store item under              */
    HempValue       value           /* value to store                       */
);

typedef HempType
(* HempTypedef)(
    HempInt         id,             /* numerical id                         */
    HempString      name            /* unique type name                     */
);

typedef void
(* HempClean)(
    HempValue       value
);


//typedef HempMemory
//(* hemp_method_f)(
//    HempType        type,           /* pointer to type                      */
//    HempString      name            /* method name                          */
//);



#endif /* HEMP_TYPES_H */