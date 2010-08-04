#ifndef HEMP_ERROR_H
#define HEMP_ERROR_H

#include <setjmp.h>
#include <hemp/types.h>

typedef jmp_buf                 hemp_jump_b;
typedef struct hemp_jump_s      hemp_jump_t;
typedef struct hemp_jump_s    * hemp_jump_p;

typedef enum { 
    HEMP_ERROR_NONE = 0,
    HEMP_ERROR_UNKNOWN,
    HEMP_ERROR_MEMORY,
    HEMP_ERROR_MISSING,
    HEMP_ERROR_INVALID,
    HEMP_ERROR_MAX
} hemp_errno_e;


static hemp_cstr_p hemp_errmsg[] = {
    "No error",
    "Unknown error",
    "Memory allocation failed",
    "No %s specified",
    "Invalid %s specified: %s"
};

struct hemp_jump_s {
    hemp_pos_t  depth;
    hemp_jump_b buffer;
    hemp_jump_p parent;
};

struct hemp_error_s {
    hemp_errno_e  number;
    hemp_cstr_p   message;
    hemp_error_p  parent;
};


hemp_error_p
    hemp_error_new(
        hemp_errno_e number
    );


hemp_error_p
    hemp_error_init(
        hemp_errno_e errno,
        hemp_cstr_p  message
    );

hemp_error_p
    hemp_error_initf(
        hemp_errno_e errno,
        hemp_cstr_p  format,
        ...
    );

hemp_error_p
    hemp_error_initfv(
        hemp_errno_e errno,
        hemp_cstr_p  format,
        va_list     *args
    );

void
    hemp_error_free(
        hemp_error_p error
    );

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
        hemp_jump_t jump;                           \
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


#endif /* HEMP_ERROR_H */
