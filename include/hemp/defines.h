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

#define HEMP_BLANK          ""
#define HEMP_SPACE          " "
#define HEMP_DOT            "."


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

/* error codes (work in progress) */
#define HEMP_ERRNO_HELP     1
#define HEMP_ERRNO_GETOPT   2

/* error messages (ditto) */
#define HEMP_ERRMSG_MALLOC  "Failed to allocate memory for a new %s"



#endif /* HEMP_DEFINES_H */

