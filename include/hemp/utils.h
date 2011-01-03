#ifndef HEMP_UTILS_H
#define HEMP_UTILS_H

#include <hemp/core.h>


/*--------------------------------------------------------------------------
 * Type definitions
 *--------------------------------------------------------------------------*/

struct hemp_md5 {
    HempU32         i[2];       /* number of _bits_ handled mod 2^64        */
    HempU32         buffer[4];  /* scratch buffer                           */
    HempChar        input[64];  /* input buffer                             */
    HempChar        digest[16]; /* actual digest after hemp_md5_final call  */
    HempChar        output[33]; /* output hex digest string                 */
};


/*--------------------------------------------------------------------------
 * Function prototypes
 *--------------------------------------------------------------------------*/

HempMD5
hemp_md5_init(
    HempMD5         md5
);

void 
hemp_md5_update(
    HempMD5         md5,
    HempString      input,
    HempSize        length
);

void 
hemp_md5_final(
    HempMD5         md5
);


void 
hemp_md5_free();

void 
hemp_todo(
    HempString      format,
    ...
);

void hemp_fatal(
    HempString      format,
    ...
);


/*--------------------------------------------------------------------------
 * Macros
 *--------------------------------------------------------------------------*/

#define hemp_md5_update_string(md5, str)      \
    hemp_md5_update(md5, str, strlen(str))


#endif /* HEMP_UTILS_H */
