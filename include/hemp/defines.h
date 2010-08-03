#ifndef HEMP_DEFINES_H
#define HEMP_DEFINES_H


/* package details */
#define HEMP_HEMP           "hemp"
#define HEMP_AUTHOR         "Andy Wardley"
#define HEMP_NAME           PACKAGE_NAME
#define HEMP_VERSION        PACKAGE_VERSION
#define HEMP_EMAIL          PACKAGE_BUGREPORT
#define HEMP_URL            PACKAGE_URL
#define HEMP_PROMPT         HEMP_NAME

/* aliases for commonly used values */
#define HEMP_TRUE           1
#define HEMP_FALSE          0
#define HEMP_BLANK          ""
#define HEMP_SPACE          " "
#define HEMP_TEXT           "text"
#define HEMP_FILE           "file"


/* default setting for various internals */
#define HEMP_ELEMENTS_SIZE  32
#define HEMP_VTYPES_SIZE    8
#define HEMP_TAGSET_SIZE    8
#define HEMP_PTREE_SIZE     8
#define HEMP_HASH_DENSITY   5    /* max entries/width before increasing width */

/* error codes (work in progress) */
#define HEMP_ERRNO_HELP     1
#define HEMP_ERRNO_GETOPT   2

/* error messages (ditto) */
#define HEMP_ERRMSG_MALLOC      "Failed to allocate memory for a new %s"


#endif /* HEMP_DEFINES_H */

