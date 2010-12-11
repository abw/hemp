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

typedef enum { 
    HEMP_JUMP_NONE          = 0,
    HEMP_JUMP_INTERNAL      = 1,
    HEMP_JUMP_ERROR         = 2,
    HEMP_JUMP_FLOW          = 3
} hemp_jump_type;
    
typedef enum { 
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
    HEMP_ERROR_CONVERT,
    HEMP_ERROR_UNDEF,
    HEMP_ERROR_FETCH,
    HEMP_ERROR_STORE,
    HEMP_ERROR_OPTION,
    HEMP_ERROR_HELP,
    HEMP_ERROR_MAX
} hemp_errno;


extern hemp_string hemp_errmsg[];



/*--------------------------------------------------------------------------
 * type definitions
 *--------------------------------------------------------------------------*/

struct hemp_jump {
//    hemp_jump_type  type;
    hemp_jump       parent;
    hemp_jump_buf   buffer;
    hemp_pos        depth;
};

struct hemp_error {
    /* TODO: add type pointer so we can treat errors as objects and use
     * templates to render different error types
     */
    hemp_errno      number;
    hemp_string     message;
    hemp_document   document;
    hemp_error      parent;
};


/* this is currently in string.h as hemp_str_pos, but I think we want to 
 * make it more general...
 */
struct hemp_location {
    hemp_pos        position;   /* byte offset 0 to n-1     */
    hemp_pos        line;       /* line number, 1 to n      */
    hemp_pos        column;     /* column number, 1 to n    */
    hemp_string     extract;
};




/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

hemp_error
hemp_error_new(
    hemp_errno      number
);


hemp_error
hemp_error_init(
    hemp_errno      number,
    hemp_string     message
);

hemp_error
hemp_error_initf(
    hemp_errno      number,
    hemp_string     format,
    ...
);

hemp_error
hemp_error_initfv(
    hemp_errno      number,
    hemp_string     format,
    va_list         args
);

hemp_text
hemp_error_text(
    hemp_error      error
);

hemp_error    
hemp_error_message(
    hemp_hemp, 
    hemp_errno, 
    ...
);

void
hemp_error_free(
    hemp_error      error
);


/* these are implemented in hemp.c */
hemp_string     hemp_error_format(hemp_hemp, hemp_errno);
//hemp_error      hemp_error_message(hemp_hemp, hemp_errno, ...);
hemp_error      hemp_error_document(hemp_error, hemp_document);
void            hemp_error_throw(hemp_hemp, hemp_error);


/* these macros are for testing */

#define _HEMP_TRY                           \
    do {                                    \
        jmp_buf __jump;                     \
        switch ( setjmp(__jump) ) {         \
            case 0:                         \
                /* unsafe code goes here */
#define _HEMP_THROW(e)                      \
            longjmp(__jump, e);
#define _HEMP_CATCH(e)                      \
                break;                      \
            case e:                         \
                /* exception handling code goes here */
#define _HEMP_END                           \
                break;                      \
        }                                   \
    } while (0);



/* these macros assume the presence of a visible hemp symbol */

#define HEMP_TRY                                    \
    do {                                            \
        struct hemp_jump jump;                      \
        jump.parent = hemp->jump;                   \
        jump.depth  = hemp->jump                    \
            ? hemp->jump->depth + 1                 \
            : 0;                                    \
        hemp->jump  = &jump;                        \
                                                    \
        switch ( setjmp(jump.buffer) ) {            \
            case HEMP_JUMP_NONE:                    \
                /* unsafe code goes here */

// new version
#define HEMP_JUMP(type)                             \
                longjmp(hemp->jump->buffer, type);

// old version, being replaced/changed
#define HEMP_THROW(type)                            \
                longjmp(hemp->jump->buffer, type);

#define HEMP_CATCH(e)                               \
                break;                              \
            case e:                                 \
                /* exception handling code goes here */

#define HEMP_CATCH_ALL                              \
                break;                              \
            default:                                \
                /* default exception handling code goes here */

#define HEMP_END                                    \
                break;                              \
        }                                           \
        hemp->jump = hemp->jump->parent;            \
    } while (0);


#define hemp_throw(h,...) \
    hemp_error_throw(h, hemp_error_message(h,__VA_ARGS__))


/*--------------------------------------------------------------------------
 * Macros of convenience
 *--------------------------------------------------------------------------*/

#define HEMP_THROW_UNTERM_STRING(doc, end)                          \
    hemp_document_errmsg(doc, HEMP_ERROR_UNTERM, HEMP_STR_QUOTED, end)

#define HEMP_THROW_FRAGMENT(fragment, ...) ({                       \
    hemp_document _hemp_doc = hemp_fragment_document(fragment);     \
    hemp_document_errmsg(_hemp_doc, __VA_ARGS__);                   \
})

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


#endif /* HEMP_ERROR_H */
