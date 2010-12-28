#ifndef HEMP_UTILS_H
#define HEMP_UTILS_H

#include <hemp/core.h>


/*--------------------------------------------------------------------------
 * type definitions
 *--------------------------------------------------------------------------*/

struct hemp_md5 {
    hemp_u32    i[2];       /* number of _bits_ handled mod 2^64        */
    hemp_u32    buffer[4];  /* scratch buffer                           */
    hemp_char   input[64];  /* input buffer                             */
    hemp_char   digest[16]; /* actual digest after hemp_md5_final call  */
    hemp_char   output[33]; /* output hex digest string                 */
};


/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

hemp_md5
hemp_md5_init(
    hemp_md5        md5
);

void 
hemp_md5_update(
    hemp_md5        md5,
    hemp_string     input,
    hemp_size       length
);

void 
hemp_md5_final(
    hemp_md5        md5
);


void 
hemp_md5_free();

void 
hemp_todo(
    char          * format, 
    ...
);

void hemp_fatal(
    char          * format, 
    ...
);


/*--------------------------------------------------------------------------
 * macros
 *--------------------------------------------------------------------------*/

#define hemp_md5_update_string(md5, str)      \
    hemp_md5_update(md5, str, strlen(str))


#endif /* HEMP_UTILS_H */
