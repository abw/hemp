#ifndef HEMP_DEFINES_H
#define HEMP_DEFINES_H


/* package details */
#define HEMP_AUTHOR         "Andy Wardley"
#define HEMP_NAME           PACKAGE_NAME
#define HEMP_VERSION        PACKAGE_VERSION
#define HEMP_EMAIL          PACKAGE_BUGREPORT
#define HEMP_URL            PACKAGE_URL
#define HEMP_PROMPT         HEMP_NAME

/* aliases for commonly used values, characters, etc */
#define HEMP_TRUE           1
#define HEMP_FALSE          0
#define HEMP_TAB            9
#define HEMP_NL             10
#define HEMP_LF             10
#define HEMP_CR             13
#define HEMP_NUL            '\0'
#define HEMP_DOT            '.'
#define HEMP_SQUOTE         '\''
#define HEMP_DQUOTE         '"'
#define HEMP_COMMENT        '#'
#define HEMP_BACKSLASH      '\\'
#define HEMP_UNDERSCORE     '_'

/* aliases for various short strings */
#define HEMP_STR_HEMP       "Hemp"
#define HEMP_STR_UNKNOWN    "Unknown"
#define HEMP_STR_INFINITY   "Infinity"
#define HEMP_STR_MISSING    "Missing"
#define HEMP_STR_NOTHING    "Nothing"
#define HEMP_STR_UNDEFINED  "Undefined"
#define HEMP_STR_EMPTY      "Empty"
#define HEMP_STR_FALSE      "False"
#define HEMP_STR_TRUE       "True"
#define HEMP_STR_BEFORE     "Before"
#define HEMP_STR_AFTER      "After"
#define HEMP_STR_EQUAL      "Equal"
#define HEMP_STR_TYPE       "Type"
#define HEMP_STR_VALUE      "Value"
#define HEMP_STR_NUMBER     "Number"
#define HEMP_STR_INTEGER    "Integer"
#define HEMP_STR_POINTER    "Pointer"
#define HEMP_STR_STRING     "String"
#define HEMP_STR_TEXT       "Text"
#define HEMP_STR_LIST       "List"
#define HEMP_STR_HASH       "Hash"
#define HEMP_STR_CODE       "Code"
#define HEMP_STR_PARAMS     "Params"
#define HEMP_STR_PAIRS      "Pairs"
#define HEMP_STR_OBJECT     "Object"
#define HEMP_STR_IDENTITY   "Identity"
#define HEMP_STR_RESERVED   "Reserved"
#define HEMP_STR_UNUSED     "Unused"
#define HEMP_STR_QUOTED     "quoted string"
#define HEMP_STR_NO_TEXT    "empty text"
#define HEMP_STR_BOOLEAN    "truth"
#define HEMP_STR_COMPARE    "comparison"
#define HEMP_STR_DEFINED    "defined"
#define HEMP_STR_BLANK      ""
#define HEMP_STR_SPACE      " "
#define HEMP_STR_DOT        "."

/* format strings */
#define HEMP_FMT_INT        "%d"
#define HEMP_FMT_NUM        "%g"

/* names of standard schemes, languages, dialects, libraries, etc */
// TODO: rename HEMP_NAME_XXXX
#define HEMP_HEMP           "hemp"
#define HEMP_TEXT           "text"
#define HEMP_FILE           "file"
#define HEMP_TT3            "tt3"

/* dummy arguments(s) as a fallback for generating error messages */
#define HEMP_WTF            "<?>"
#define HEMP_WTFS           HEMP_WTF, HEMP_WTF, HEMP_WTF, HEMP_WTF, HEMP_WTF

/* default setting for various internals */
#define HEMP_BUFFER_SIZE    256     /* size of uri/name lookup buffers      */
#define HEMP_MESSAGE_SIZE   2048    /* size of message/error buffers        */
#define HEMP_HASH_DENSITY   5       /* max hash density before resizing     */
#define HEMP_ELEMENTS_SIZE  32      /* # of elements allocated per slab     */
#define HEMP_OPERATORS_SIZE 32      /* # of operators allocated per slab    */
#define HEMP_TYPES_SIZE     32      /* # of data types (Nan-tagged)         */
#define HEMP_TYPES_RESERVED 16      /* # of reserved data types             */
#define HEMP_PTREE_SIZE     8       /* # of bucket chains in a ptree        */
#define HEMP_VTYPES_SIZE    8
#define HEMP_TAGSET_SIZE    8
#define HEMP_VIEW_SIZE      32
#define HEMP_TMP_POOL_SIZE  8       /* # of temporary data items to pool    */

/* 
 * Arrrr!  These be the element flags, m'hearties.  They be the bits what'll
 * tells ya what an element can be.  Be it a hunk o'whitespace?  Some static
 * text?  Or perhaps a shiny gold dubloon?  Look in ye old element->flags and 
 * ye shall find ye treasure!  Let it be known that we don't be callin'em the 
 * HEMP_IS_GOLD_DUBLOON, or whatever the case might be, in case we be gettin' 
 * ourselves confused with ye old value macros and definitions like them there 
 * HEMP_IS_NUMBER and hemp_is_text.  So now ye knows, and may that guidance 
 * serve you well on yer merry sea-faring escapades... Arrrr!
 */
#define HEMP_BE_WHITESPACE  0x00001  /* skippabled whitespace, comment, etc  */
#define HEMP_BE_DELIMITER   0x00002  /* skippable list item delimiter        */
#define HEMP_BE_SEPARATOR   0x00004  /* expression separator                 */
#define HEMP_BE_TERMINATOR  0x00008  /* non-skippable expression terminator  */
#define HEMP_BE_HIDDEN      0x00010  /* generates no output                  */
#define HEMP_BE_SOURCE      0x00020  /* source text can yield output text    */
#define HEMP_BE_FIXED       0x00040  /* element output is pre-defined        */
#define HEMP_BE_PREFIX      0x00100  /* element is a prefix operator         */
#define HEMP_BE_INFIX       0x00200  /* element is an infix operator         */
#define HEMP_BE_POSTFIX     0x00400  /* element is a postfix operator        */
#define HEMP_BE_POSTBOUND   0x00800  /* no space before postfix operator     */
#define HEMP_BE_NAMED       0x01000  /* anonymous vs named functions         */
#define HEMP_BE_ARGS        0x02000  /* functions with arguments             */
#define HEMP_BE_LVALUE      0x04000  /* element can be lvalue for assignment */
#define HEMP_BE_PAIRS       0x08000  /* element can yield pairs              */
#define HEMP_BE_BODY        0x10000  /* element is a content body            */
#define HEMP_BE_TERMINATED  0x20000  /* element body has been terminated     */
#define HEMP_BE_ALLOCATED   0x40000  /* element has memory allocated         */
#define HEMP_BE_EOF         0x80000  /* special marker for end of file/text  */


#endif /* HEMP_DEFINES_H */

