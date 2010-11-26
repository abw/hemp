#ifndef HEMP_TAG_H
#define HEMP_TAG_H

#include <hemp/core.h>
#include <hemp/element.h>
#include <hemp/ptree.h>
#include <hemp/grammar.h>
#include <hemp/scanner.h>

/*
#include "hemp/ptree.h"
#include "hemp/elements.h"
#include "hemp/template.h"
#include "hemp/utils.h"
*/


/*--------------------------------------------------------------------------
 * data structures
 *--------------------------------------------------------------------------*/

enum hemp_tag_style {
    HEMP_INLINE_TAG  = 1,
    HEMP_OUTLINE_TAG = 2
};



struct hemp_tag {
   hemp_string       name;
   hemp_tag_style_t style;
   hemp_string       start;
   hemp_string       end;
   hemp_tag_scan_f  scan;
   hemp_tag_skip_f  to_end_of_line;
   hemp_grammar   grammar;
};


/*--------------------------------------------------------------------------
 * function prototypes for generic tags
 *--------------------------------------------------------------------------*/

hemp_tag
    hemp_tag_init(
        hemp_string       name,
        hemp_tag_style_t style,
        hemp_string       start,
        hemp_string       end,
        hemp_tag_scan_f  scan,
        hemp_grammar   grammar
    );

hemp_tag
    hemp_tag_copy(
        hemp_tag tag
    );

void
    hemp_tag_set_name(
        hemp_tag tag,
        hemp_string name
    );

void
    hemp_tag_set_start(
        hemp_tag tag,
        hemp_string start
    );

void
    hemp_tag_set_end(
        hemp_tag tag,
        hemp_string end
    );

void
    hemp_tag_set_start_end(
        hemp_tag tag,
        hemp_string start,
        hemp_string end
    );

void
    hemp_tag_free(
        hemp_tag tag
    );


/*--------------------------------------------------------------------------
 * function protoypes for builtin tag types
 *--------------------------------------------------------------------------*/

void        hemp_outline_tag_scanner( HEMP_TAG_SCAN_ARGS );
hemp_string  hemp_outline_tag_to_end_of_line( HEMP_TAG_SKIP_ARGS );

void        hemp_inline_tag_scanner( HEMP_TAG_SCAN_ARGS );
hemp_string  hemp_inline_tag_to_end_of_line( HEMP_TAG_SKIP_ARGS );


/* These are TODO (change name/implement/cleanup) */
void hemp_scan_comment_tag  ( HEMP_TAG_SCAN_ARGS );
void hemp_scan_control_tag  ( HEMP_TAG_SCAN_ARGS );
void hemp_scan_variable_tag ( HEMP_TAG_SCAN_ARGS );
void hemp_scan_embed_tag    ( HEMP_TAG_SCAN_ARGS );


/*--------------------------------------------------------------------------
 * macros
 *--------------------------------------------------------------------------*/

#define hemp_not_newline(src)                   \
    (   *src != HEMP_LF                         \
    &&  *src != HEMP_CR                         \
    )

#define hemp_not_tag_end(src, tag_end, tag_len) \
    (  *src != *tag_end                         \
    || strncmp(src, tag_end, tag_len)           \
    )



#endif /* HEMP_TAG_H */