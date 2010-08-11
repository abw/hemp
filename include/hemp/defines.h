#ifndef HEMP_DEFINES_H
#define HEMP_DEFINES_H


/* package details */
#define HEMP_AUTHOR         "Andy Wardley"
#define HEMP_NAME           PACKAGE_NAME
#define HEMP_VERSION        PACKAGE_VERSION
#define HEMP_EMAIL          PACKAGE_BUGREPORT
#define HEMP_URL            PACKAGE_URL
#define HEMP_PROMPT         HEMP_NAME

/* aliases for commonly used values, characters, strings, etc */
#define HEMP_TRUE           1
#define HEMP_FALSE          0
#define HEMP_TAB            9
#define HEMP_NL             10
#define HEMP_LF             10
#define HEMP_CR             13
#define HEMP_NUL            '\0'
#define HEMP_SQUOTE         '\''
#define HEMP_DQUOTE         '"'
#define HEMP_COMMENT        '#'
#define HEMP_BACKSLASH      '\\'
#define HEMP_UNDERSCORE     '_'

/* short strings */
#define HEMP_STR_BLANK      ""
#define HEMP_STR_SPACE      " "
#define HEMP_STR_DOT        "."

/* names of standard schemes, languages, dialects, libraries, etc */
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
#define HEMP_PTREE_SIZE     8       /* # of bucket chains in a ptree        */
#define HEMP_VTYPES_SIZE    8
#define HEMP_TAGSET_SIZE    8

/* Arrrr!  These be the element flags, m'hearties */
#define HEMP_BE_WHITESPACE  0x0001
#define HEMP_BE_SEPARATOR   0x0002
#define HEMP_BE_TERMINATOR  0x0004
#define HEMP_BE_HIDDEN      0x0008
#define HEMP_BE_FIXED       0x0010
#define HEMP_BE_STATIC      0x0020
#define HEMP_BE_EOF         0x8000



#endif /* HEMP_DEFINES_H */

