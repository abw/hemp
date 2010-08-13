#include <hemp/utils.h>


void hemp_todo(char *format, ...) {
    va_list args;
    va_start(args, format);
    fprintf(stderr, "%sTODO: %s", ANSI_RED, ANSI_YELLOW);
    vfprintf(stderr, format, args);
    fprintf(stderr, "%s\n", ANSI_RESET);
    va_end(args);
}


void hemp_fatal(char *format, ...) {
    va_list args;
    va_start(args, format);
    fprintf(stderr, "%shemp fatal error: %s", ANSI_RED, ANSI_YELLOW);
    vfprintf(stderr, format, args);
    fprintf(stderr, "%s\n", ANSI_RESET);
    va_end(args);
    exit(1);
}



/*--------------------------------------------------------------------------
 * MD5 - derived from the RSA default implementation
 *--------------------------------------------------------------------------*/

/*
 **********************************************************************
 ** Copyright (C) 1990, RSA Data Security, Inc. All rights reserved. **
 **                                                                  **
 ** License to copy and use this software is granted provided that   **
 ** it is identified as the "RSA Data Security, Inc. MD5 Message     **
 ** Digest Algorithm" in all material mentioning or referencing this **
 ** software or this function.                                       **
 **                                                                  **
 ** License is also granted to make and use derivative works         **
 ** provided that such works are identified as "derived from the RSA **
 ** Data Security, Inc. MD5 Message Digest Algorithm" in all         **
 ** material mentioning or referencing the derived work.             **
 **                                                                  **
 ** RSA Data Security, Inc. makes no representations concerning      **
 ** either the merchantability of this software or the suitability   **
 ** of this software for any particular purpose.  It is provided "as **
 ** is" without express or implied warranty of any kind.             **
 **                                                                  **
 ** These notices must be retained in any copies of any part of this **
 ** documentation and/or software.                                   **
 **********************************************************************
 */

static hemp_char_t HEMP_MD5_PADDING[64] = {
  0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

/* F, G and H are basic MD5 functions: selection, majority, parity */
#define F(x, y, z) (((x) & (y)) | ((~x) & (z)))
#define G(x, y, z) (((x) & (z)) | ((y) & (~z)))
#define H(x, y, z) ((x) ^ (y) ^ (z))
#define I(x, y, z) ((y) ^ ((x) | (~z))) 

/* ROTATE_LEFT rotates x left n bits */
#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32-(n))))

/* FF, GG, HH, and II transformations for rounds 1, 2, 3, and 4 */
/* Rotation is separate from addition to prevent recomputation */
#define FF(a, b, c, d, x, s, ac) \
  {(a) += F ((b), (c), (d)) + (x) + (hemp_uint_t)(ac); \
   (a) = ROTATE_LEFT ((a), (s)); \
   (a) += (b); \
  }
#define GG(a, b, c, d, x, s, ac) \
  {(a) += G ((b), (c), (d)) + (x) + (hemp_uint_t)(ac); \
   (a) = ROTATE_LEFT ((a), (s)); \
   (a) += (b); \
  }
#define HH(a, b, c, d, x, s, ac) \
  {(a) += H ((b), (c), (d)) + (x) + (hemp_uint_t)(ac); \
   (a) = ROTATE_LEFT ((a), (s)); \
   (a) += (b); \
  }
#define II(a, b, c, d, x, s, ac) \
  {(a) += I ((b), (c), (d)) + (x) + (hemp_uint_t)(ac); \
   (a) = ROTATE_LEFT ((a), (s)); \
   (a) += (b); \
  }


/* Initialise an MD5 data structure */

hemp_md5_p 
hemp_md5_init(
    hemp_md5_p md5
) {
    if (! md5) {
        md5 = hemp_mem_alloc(
            sizeof(struct hemp_md5_s)
        );
        if (! md5)
            hemp_mem_fatal("md5");
        debug("allocated md5 at %p\n", md5);
    }
    
        
    md5->i[0] = md5->i[1] = (hemp_uint_t) 0;

    /* Load magic initialization constants */
    md5->buffer[0] = (hemp_uint_t) 0x67452301;
    md5->buffer[1] = (hemp_uint_t) 0xefcdab89;
    md5->buffer[2] = (hemp_uint_t) 0x98badcfe;
    md5->buffer[3] = (hemp_uint_t) 0x10325476;
    
    return md5;
}


/* Update the MD5 with input data */

void 
hemp_md5_update(
    hemp_md5_p  md5,
    hemp_cstr_p input,
    hemp_size_t length
) {
    hemp_uint_t in[16];
    int mdi;
    unsigned int i, ii;

    /* compute number of bytes mod 64 */
    mdi = (int)((md5->i[0] >> 3) & 0x3F);

    /* update number of bits */
    if ((md5->i[0] + ((hemp_uint_t) length << 3)) < md5->i[0])
        md5->i[1]++;

    md5->i[0] += ((hemp_uint_t) length << 3);
    md5->i[1] += ((hemp_uint_t) length >> 29);

    while (length--) {
        /* add new character to buffer, increment mdi */
        md5->input[mdi++] = *input++;

        /* transform if necessary */
        if (mdi == 0x40) {
            for (i = 0, ii = 0; i < 16; i++, ii += 4)
                in[i] = (((hemp_uint_t) md5->input[ii+3]) << 24) |
                        (((hemp_uint_t) md5->input[ii+2]) << 16) |
                        (((hemp_uint_t) md5->input[ii+1]) << 8 ) |
                         ((hemp_uint_t) md5->input[ii]);

            hemp_md5_transform(md5->buffer, in);
            mdi = 0;
        }
    }
}


/* Finalise the MD5 to compute digest */

void 
hemp_md5_final(
    hemp_md5_p md5
) {
    hemp_uint_t in[16];
    hemp_cstr_p str = md5->output;
    int mdi;
    unsigned int i, ii;
    unsigned int padlen;

    /* save number of bits */
    in[14] = md5->i[0];
    in[15] = md5->i[1];

    /* compute number of bytes mod 64 */
    mdi = (int)((md5->i[0] >> 3) & 0x3F);

    /* pad out to 56 mod 64 */
    padlen = (mdi < 56) ? (56 - mdi) : (120 - mdi);
    hemp_md5_update(md5, HEMP_MD5_PADDING, padlen);

    /* append length in bits and transform */
    for (i = 0, ii = 0; i < 14; i++, ii += 4)
        in[i] = (((hemp_uint_t) md5->input[ii+3]) << 24) |
                (((hemp_uint_t) md5->input[ii+2]) << 16) |
                (((hemp_uint_t) md5->input[ii+1]) << 8 ) |
                 ((hemp_uint_t) md5->input[ii]);
        hemp_md5_transform(md5->buffer, in);

    /* store buffer in digest */
    for (i = 0, ii = 0; i < 4; i++, ii += 4) {
        md5->digest[ii  ] = (hemp_char_t) ( md5->buffer[i]        & 0xFF);
        md5->digest[ii+1] = (hemp_char_t) ((md5->buffer[i] >> 8 ) & 0xFF);
        md5->digest[ii+2] = (hemp_char_t) ((md5->buffer[i] >> 16) & 0xFF);
        md5->digest[ii+3] = (hemp_char_t) ((md5->buffer[i] >> 24) & 0xFF);
    }

    for (i = 0; i < 16; i++) {
        sprintf(str, "%02x", md5->digest[i]);
        str += 2;
    }
    *str = HEMP_NUL;
}


/* Basic MD5 step. Transform buffer based on input. */

static void 
hemp_md5_transform(
    hemp_uint_t *buffer,
    hemp_uint_t *input
) {
    hemp_uint_t a = buffer[0], 
                b = buffer[1], 
                c = buffer[2], 
                d = buffer[3];

    /* Round 1 */
    #define S11 7
    #define S12 12
    #define S13 17
    #define S14 22

    FF ( a, b, c, d, input[ 0], S11, 3614090360); /* 1 */
    FF ( d, a, b, c, input[ 1], S12, 3905402710); /* 2 */
    FF ( c, d, a, b, input[ 2], S13,  606105819); /* 3 */
    FF ( b, c, d, a, input[ 3], S14, 3250441966); /* 4 */
    FF ( a, b, c, d, input[ 4], S11, 4118548399); /* 5 */
    FF ( d, a, b, c, input[ 5], S12, 1200080426); /* 6 */
    FF ( c, d, a, b, input[ 6], S13, 2821735955); /* 7 */
    FF ( b, c, d, a, input[ 7], S14, 4249261313); /* 8 */
    FF ( a, b, c, d, input[ 8], S11, 1770035416); /* 9 */
    FF ( d, a, b, c, input[ 9], S12, 2336552879); /* 10 */
    FF ( c, d, a, b, input[10], S13, 4294925233); /* 11 */
    FF ( b, c, d, a, input[11], S14, 2304563134); /* 12 */
    FF ( a, b, c, d, input[12], S11, 1804603682); /* 13 */
    FF ( d, a, b, c, input[13], S12, 4254626195); /* 14 */
    FF ( c, d, a, b, input[14], S13, 2792965006); /* 15 */
    FF ( b, c, d, a, input[15], S14, 1236535329); /* 16 */

    /* Round 2 */
    #define S21 5
    #define S22 9
    #define S23 14
    #define S24 20
    GG ( a, b, c, d, input[ 1], S21, 4129170786); /* 17 */
    GG ( d, a, b, c, input[ 6], S22, 3225465664); /* 18 */
    GG ( c, d, a, b, input[11], S23,  643717713); /* 19 */
    GG ( b, c, d, a, input[ 0], S24, 3921069994); /* 20 */
    GG ( a, b, c, d, input[ 5], S21, 3593408605); /* 21 */
    GG ( d, a, b, c, input[10], S22,   38016083); /* 22 */
    GG ( c, d, a, b, input[15], S23, 3634488961); /* 23 */
    GG ( b, c, d, a, input[ 4], S24, 3889429448); /* 24 */
    GG ( a, b, c, d, input[ 9], S21,  568446438); /* 25 */
    GG ( d, a, b, c, input[14], S22, 3275163606); /* 26 */
    GG ( c, d, a, b, input[ 3], S23, 4107603335); /* 27 */
    GG ( b, c, d, a, input[ 8], S24, 1163531501); /* 28 */
    GG ( a, b, c, d, input[13], S21, 2850285829); /* 29 */
    GG ( d, a, b, c, input[ 2], S22, 4243563512); /* 30 */
    GG ( c, d, a, b, input[ 7], S23, 1735328473); /* 31 */
    GG ( b, c, d, a, input[12], S24, 2368359562); /* 32 */

    /* Round 3 */
    #define S31 4
    #define S32 11
    #define S33 16
    #define S34 23
    HH ( a, b, c, d, input[ 5], S31, 4294588738); /* 33 */
    HH ( d, a, b, c, input[ 8], S32, 2272392833); /* 34 */
    HH ( c, d, a, b, input[11], S33, 1839030562); /* 35 */
    HH ( b, c, d, a, input[14], S34, 4259657740); /* 36 */
    HH ( a, b, c, d, input[ 1], S31, 2763975236); /* 37 */
    HH ( d, a, b, c, input[ 4], S32, 1272893353); /* 38 */
    HH ( c, d, a, b, input[ 7], S33, 4139469664); /* 39 */
    HH ( b, c, d, a, input[10], S34, 3200236656); /* 40 */
    HH ( a, b, c, d, input[13], S31,  681279174); /* 41 */
    HH ( d, a, b, c, input[ 0], S32, 3936430074); /* 42 */
    HH ( c, d, a, b, input[ 3], S33, 3572445317); /* 43 */
    HH ( b, c, d, a, input[ 6], S34,   76029189); /* 44 */
    HH ( a, b, c, d, input[ 9], S31, 3654602809); /* 45 */
    HH ( d, a, b, c, input[12], S32, 3873151461); /* 46 */
    HH ( c, d, a, b, input[15], S33,  530742520); /* 47 */
    HH ( b, c, d, a, input[ 2], S34, 3299628645); /* 48 */

    /* Round 4 */
    #define S41 6
    #define S42 10
    #define S43 15
    #define S44 21
    II ( a, b, c, d, input[ 0], S41, 4096336452); /* 49 */
    II ( d, a, b, c, input[ 7], S42, 1126891415); /* 50 */
    II ( c, d, a, b, input[14], S43, 2878612391); /* 51 */
    II ( b, c, d, a, input[ 5], S44, 4237533241); /* 52 */
    II ( a, b, c, d, input[12], S41, 1700485571); /* 53 */
    II ( d, a, b, c, input[ 3], S42, 2399980690); /* 54 */
    II ( c, d, a, b, input[10], S43, 4293915773); /* 55 */
    II ( b, c, d, a, input[ 1], S44, 2240044497); /* 56 */
    II ( a, b, c, d, input[ 8], S41, 1873313359); /* 57 */
    II ( d, a, b, c, input[15], S42, 4264355552); /* 58 */
    II ( c, d, a, b, input[ 6], S43, 2734768916); /* 59 */
    II ( b, c, d, a, input[13], S44, 1309151649); /* 60 */
    II ( a, b, c, d, input[ 4], S41, 4149444226); /* 61 */
    II ( d, a, b, c, input[11], S42, 3174756917); /* 62 */
    II ( c, d, a, b, input[ 2], S43,  718787259); /* 63 */
    II ( b, c, d, a, input[ 9], S44, 3951481745); /* 64 */

    buffer[0] += a;
    buffer[1] += b;
    buffer[2] += c;
    buffer[3] += d;
}


/* Cleanup MD5 data structure and release memory */

void
hemp_md5_free(
    hemp_md5_p md5
) {
    debug("freeing MD5 data at %p\n", md5);
    hemp_mem_free(md5);
}

