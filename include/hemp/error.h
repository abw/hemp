#ifndef HEMP_ERROR_H
#define HEMP_ERROR_H

#include <stdarg.h>
#include <hemp/types.h>


/*--------------------------------------------------------------------------
 * error codes and corresponding message formats
 *--------------------------------------------------------------------------*/


typedef enum {
    /* NOT USED... changed my mind, but keeping for reference */
    /* general error types */
    HEMP_ERRTYPE_NONE       = 0x01,
    HEMP_ERRTYPE_UNKNOWN    = 0x02,
    HEMP_ERRTYPE_INTERNAL   = 0x03,

    /* compile time errors */
    HEMP_ERRTYPE_LOCATE     = 0x10,
    HEMP_ERRTYPE_PREPARE    = 0x11,
    HEMP_ERRTYPE_SCAN       = 0x12,
    HEMP_ERRTYPE_PARSE      = 0x13,
    HEMP_ERRTYPE_COMPILE    = 0x14,

    /* run time errors */
    HEMP_ERRTYPE_DATA       = 0x20,
} hemp_error_type;

enum hemp_jump_type {
    HEMP_JUMP_NONE          = 0,
    HEMP_JUMP_INTERNAL      = 1,
    HEMP_JUMP_ERROR         = 2,
    HEMP_JUMP_FLOW          = 3
};

enum hemp_errno {
    HEMP_ERROR_NONE = 0,
    HEMP_ERROR_UNKNOWN,
    HEMP_ERROR_MEMORY,
    HEMP_ERROR_MISSING,
    HEMP_ERROR_INVALID,
    HEMP_ERROR_DUPLICATE,
    HEMP_ERROR_OVERFLOW,
    HEMP_ERROR_TOKEN,
    HEMP_ERROR_UNTERM,
    HEMP_ERROR_NOEXPR,
    HEMP_ERROR_NOBODY, /* 10 */
    HEMP_ERROR_NOEND,
    HEMP_ERROR_NOFRAG,
    HEMP_ERROR_FRAGMATCH,
    HEMP_ERROR_UNEXPECTED,
    HEMP_ERROR_CONVERT,
    HEMP_ERROR_UNDEF,
    HEMP_ERROR_FETCH,
    HEMP_ERROR_STORE,
    HEMP_ERROR_OPTION,
    HEMP_ERROR_HELP,
    HEMP_ERROR_MAX
};


extern HempString hemp_errmsg[];



/*--------------------------------------------------------------------------
 * type definitions
 *--------------------------------------------------------------------------*/

struct hemp_jump {
//  hemp_jump_type  type;
    HempJump        parent;
    HempJumpBuf     buffer;
    HempPos         depth;
};

struct hemp_location {
    HempPos         position;   /* byte offset 0 to n-1                     */
    HempPos         line;       /* line number, 1 to n                      */
    HempPos         column;     /* column number, 1 to n                    */
    HempString      extract;    /* extract of source code for current line  */
};

struct hemp_error {
    /* TODO: add type pointer so we can treat errors as objects and use
     * templates to render different error types
     */
    HempErrno       number;
    HempString      message;
    HempDocument    document;
    HempError       parent;
    HempLocation    location;
};


/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

HempLocation
hemp_location_new();

void
hemp_location_free(
    HempLocation    location
);


HempError
hemp_error_new(
    HempErrno       number
);


HempError
hemp_error_init(
    HempErrno       number,
    HempString      message
);

HempError
hemp_error_initf(
    HempErrno       number,
    HempString      format,
    ...
);

HempError
hemp_error_initfv(
    HempErrno       number,
    HempString      format,
    va_list         args
);

HempText
hemp_error_text(
    HempError       error
);

HempError
hemp_error_message(
    Hemp            hemp,
    HempErrno       number,
    ...
);

void
hemp_error_free(
    HempError       error
);


HempError
hemp_error_document(
    HempError       error,
    HempDocument    document
);


HempError
hemp_error_location(
    HempError       error,
    HempString      source,
    HempString      marker
);


HempError
hemp_error_document_location(
    HempError       error,
    HempDocument    document,
    HempString      marker
);


/* these are implemented in hemp.c */
HempString          hemp_error_format(Hemp, HempErrno);
void                hemp_error_throw(Hemp, HempError);
//HempError      hemp_error_message(Hemp, HempErrno, ...);
//HempError      hemp_error_document(HempError, HempDocument);


/* these macros assume the presence of a visible hemp symbol */

#define HEMP_TRY                                    \
    do {                                            \
        struct hemp_jump jump;                      \
        volatile int _hemp_errno;                   \
        jump.parent = hemp->jump;                   \
        jump.depth  = hemp->jump                    \
            ? hemp->jump->depth + 1                 \
            : 0;                                    \
        hemp->jump  = &jump;                        \
                                                    \
        _hemp_errno = setjmp(jump.buffer);          \
        if (_hemp_errno == HEMP_JUMP_NONE) {        \
            /* unsafe code goes here */

// new version
#define HEMP_JUMP(type)                             \
            longjmp(hemp->jump->buffer, type);

// old version, being replaced/changed
#define HEMP_THROW(type)                            \
            longjmp(hemp->jump->buffer, type);

#define HEMP_RETHROW                                \
            hemp->jump = hemp->jump->parent;        \
            longjmp(hemp->jump->buffer, hemp->error->number)

#define HEMP_CATCH(e)                               \
        }                                           \
        else if (_hemp_errno == e) {                \
            /* exception handling code goes here */

#define HEMP_CATCH_ALL                              \
        }                                           \
        else {                                      \
            /* default exception handling code goes here */

#define HEMP_FINALLY                                \
        }                                           \
        if (1) {                                    \
            /* code to be run regardless goes here */

#define HEMP_END                                    \
        }                                           \
        hemp->jump = hemp->jump->parent;            \
    } while (0);


#define hemp_throw(h,...) \
    hemp_error_throw(h, hemp_error_message(h,__VA_ARGS__))



/*--------------------------------------------------------------------------
 * Macros of convenience
 *--------------------------------------------------------------------------*/

/* raise an error for a document at a particular source location */

#define HEMP_THROW_DOCLOC(doc, loc, ...) ({                             \
    HempError _hemp_err = hemp_error_message(                           \
        doc->dialect->hemp,                                             \
        __VA_ARGS__                                                     \
    );                                                                  \
    hemp_error_document_location(_hemp_err, doc, loc);                  \
    hemp_error_throw(doc->dialect->hemp, _hemp_err);                    \
})


/* raise an error for a particular fragment */

#define HEMP_THROW_FRAGMENT(fragment, ...) ({                           \
    HempDocument _hemp_doc = hemp_fragment_document(fragment);          \
    HEMP_THROW_DOCLOC(_hemp_doc, fragment->token, __VA_ARGS__);         \
})

#define HEMP_SCAN_ERROR(doc, ...)                                       \
    HEMP_THROW_DOCLOC(doc, doc->scantok, __VA_ARGS__)

#define HEMP_PARSE_ERROR(frag, ...)                                     \
    HEMP_THROW_FRAGMENT(frag, __VA_ARGS__)


#define HEMP_THROW_TOKEN(fragment)              \
    HEMP_THROW_FRAGMENT(                        \
        fragment, HEMP_ERROR_TOKEN,             \
        fragment->type->start                   \
    );

#define HEMP_THROW_NOEXPR(fragment)             \
    HEMP_THROW_FRAGMENT(                        \
        fragment, HEMP_ERROR_NOEXPR,            \
        fragment->type->start                   \
    );

#define HEMP_THROW_NOBODY(fragment)             \
    HEMP_THROW_FRAGMENT(                        \
        fragment, HEMP_ERROR_NOBODY,            \
        fragment->type->start                   \
    );

#define HEMP_THROW_NOEND(fragment)              \
    HEMP_THROW_FRAGMENT(                        \
        fragment, HEMP_ERROR_NOEND,             \
        fragment->type->start,                  \
        fragment->type->end                     \
    );

#define HEMP_THROW_UNDEF(fragment)              \
    HEMP_THROW_FRAGMENT(                        \
        fragment, HEMP_ERROR_UNDEF              \
    );


#endif /* HEMP_ERROR_H */
