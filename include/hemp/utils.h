#ifndef HEMP_UTILS_H
#define HEMP_UTILS_H

#include <hemp/core.h>

struct hemp_md5_s {
  hemp_uint_t   i[2];           /* number of _bits_ handled mod 2^64        */
  hemp_uint_t   buffer[4];      /* scratch buffer                           */
  hemp_char_t   input[64];      /* input buffer                             */
  hemp_char_t   digest[16];     /* actual digest after hemp_md5_final call  */
  hemp_char_t   output[33];     /* output hex digest string                 */
};

typedef struct hemp_md5_s   hemp_md5_t;
typedef struct hemp_md5_s * hemp_md5_p;

hemp_md5_p
    hemp_md5_init(
        hemp_md5_p md5
    );

void 
    hemp_md5_update(
        hemp_md5_p  md5,
        hemp_cstr_p input,
        hemp_size_t length
    );

void 
    hemp_md5_final(
        hemp_md5_p md5
    );

static void 
    hemp_md5_transform(
        hemp_uint_t *buffer,
        hemp_uint_t *input
        );

void 
    hemp_md5_free();

#define hemp_md5_update_cstr(md5, str)      \
    hemp_md5_update(md5, str, strlen(str))

void hemp_todo(char *format, ...);
void hemp_fatal(char *format, ...);


#endif /* HEMP_UTILS_H */
