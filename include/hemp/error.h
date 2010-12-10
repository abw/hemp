#ifndef HEMP_ERROR_H
#define HEMP_ERROR_H

#include <stdarg.h>
#include <hemp/types.h>


/*--------------------------------------------------------------------------
 * error codes and corresponding message formats
 *--------------------------------------------------------------------------*/

typedef enum { 
    HEMP_ERROR_NONE = 0,
    HEMP_ERROR_UNKNOWN,
    HEMP_ERROR_MEMORY,
    HEMP_ERROR_MISSING,
    HEMP_ERROR_INVALID,
    HEMP_ERROR_DUPLICATE,
    HEMP_ERROR_OVERFLOW,
    HEMP_ERROR_TOKEN,
    HEMP_ERROR_BADQUOTE,
    HEMP_ERROR_CONVERT,     /* 10 */
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
    hemp_jump       parent;
    hemp_jump_buf   buffer;
    hemp_pos        depth;
};

struct hemp_error {
    hemp_errno      number;
    hemp_string     message;
    hemp_document   document;
    hemp_error      parent;
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
            case 0:                                 \
                /* unsafe code goes here */

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

#endif /* HEMP_ERROR_H */
