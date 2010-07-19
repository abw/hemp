#ifndef HEMP_ERROR_H
#define HEMP_ERROR_H

#include <setjmp.h>

typedef jmp_buf hemp_jump_buf_t;
typedef struct hemp_jump hemp_jump_t;

struct hemp_jump {
    hemp_pos_t      depth;
    hemp_jump_buf_t buffer;
    hemp_jump_t     *parent;
};


typedef enum { 
    HEMP_ERROR_ONE = 1,
    HEMP_ERROR_TWO,
    HEMP_ERROR_THREE
} hemp_error_t;


#define HEMP_ERRMSG_MALLOC      "Failed to allocate memory for a new %s"

#define HEMP_MEMORY_ERROR_MSG "Failed to allocate memory for %s"


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

#define HEMP_END                                    \
                break;                              \
        }                                           \
    } while (0);


#endif /* HEMP_ERROR_H */
