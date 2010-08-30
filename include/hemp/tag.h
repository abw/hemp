#ifndef HEMP_TAG_H
#define HEMP_TAG_H

#include <hemp/core.h>
#include <hemp/element.h>

/*
#include "hemp/ptree.h"
#include "hemp/elements.h"
#include "hemp/template.h"
#include "hemp/utils.h"
*/


/*--------------------------------------------------------------------------
 * data structures
 *--------------------------------------------------------------------------*/

enum hemp_tag_style_e {
    HEMP_INLINE_TAG  = 1,
    HEMP_OUTLINE_TAG = 2
};



struct hemp_tag_s {
   hemp_str_p       name;
   hemp_tag_style_t style;
   hemp_str_p       start;
   hemp_str_p       end;
   hemp_tag_scan_f  scan;
   hemp_grammar_p   grammar;
};


/*--------------------------------------------------------------------------
 * function prototypes for generic tags
 *--------------------------------------------------------------------------*/

hemp_tag_p
    hemp_tag_init(
        hemp_str_p       name,
        hemp_tag_style_t style,
        hemp_str_p       start,
        hemp_str_p       end,
        hemp_tag_scan_f  scan,
        hemp_grammar_p   grammar
    );

hemp_tag_p
    hemp_tag_copy(
        hemp_tag_p tag
    );

void
    hemp_tag_set_name(
        hemp_tag_p tag,
        hemp_str_p name
    );

void
    hemp_tag_set_start(
        hemp_tag_p tag,
        hemp_str_p start
    );

void
    hemp_tag_set_end(
        hemp_tag_p tag,
        hemp_str_p end
    );

void
    hemp_tag_set_start_end(
        hemp_tag_p tag,
        hemp_str_p start,
        hemp_str_p end
    );

void
    hemp_tag_free(
        hemp_tag_p tag
    );


/*--------------------------------------------------------------------------
 * function protoypes for builtin tag types
 *--------------------------------------------------------------------------*/

//    hemp_template_t     tmpl,   \

void hemp_scan_inline_tag   ( HEMP_TAG_SCAN_ARGS );
void hemp_scan_outline_tag  ( HEMP_TAG_SCAN_ARGS );
void hemp_scan_comment_tag  ( HEMP_TAG_SCAN_ARGS );
void hemp_scan_control_tag  ( HEMP_TAG_SCAN_ARGS );
void hemp_scan_variable_tag ( HEMP_TAG_SCAN_ARGS );
void hemp_scan_embed_tag    ( HEMP_TAG_SCAN_ARGS );



#endif /* HEMP_TAG_H */