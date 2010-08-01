#ifndef HEMP_TAGS_H
#define HEMP_TAGS_H

#include <ctype.h>
#include <string.h>
#include "hemp/ptree.h"
#include "hemp/element.h"
#include "hemp/elements.h"
#include "hemp/template.h"
#include "hemp/utils.h"


#define HEMP_DEFAULT_TAGSET_SIZE 8


/*--------------------------------------------------------------------------
 * data structures
 *--------------------------------------------------------------------------*/

enum hemp_tag_line {
    HEMP_INLINE_TAG     = 1,
    HEMP_OUTLINE_TAG    = 2
};

typedef enum hemp_tag_line hemp_tag_line_t;


struct hemp_tag {
   hemp_tag_line_t  line;
   hemp_name_t      name;
   hemp_name_t      start;
   hemp_name_t      end;
   hemp_tag_scan_fn scan;
};

struct hemp_tagset {
   hemp_etype_t     text_element;
   hemp_ptree_t     inline_tags;
   hemp_ptree_t     outline_tags;
};


/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

hemp_tagset_t
    hemp_tagset_init();
    
void
    hemp_tagset_free();

hemp_pnode_t
    hemp_tagset_add_inline_tag(
        hemp_tagset_t   tagset, 
        hemp_tag_t      tag
    );

hemp_pnode_t
    hemp_tagset_add_outline_tag(
        hemp_tagset_t   tagset, 
        hemp_tag_t      tag
    );

#define HEMP_SCAN_TAG_ARGS      \
    hemp_template_t     tmpl,   \
    hemp_tag_t          tag,    \
    hemp_cstr_t         tagtok, \
    hemp_pos_t          pos,    \
    hemp_cstr_t         *srcptr

void hemp_scan_inline_tag   ( HEMP_SCAN_TAG_ARGS );
void hemp_scan_outline_tag  ( HEMP_SCAN_TAG_ARGS );
void hemp_scan_comment_tag  ( HEMP_SCAN_TAG_ARGS );
void hemp_scan_control_tag  ( HEMP_SCAN_TAG_ARGS );
void hemp_scan_variable_tag ( HEMP_SCAN_TAG_ARGS );
void hemp_scan_embed_tag    ( HEMP_SCAN_TAG_ARGS );


/*--------------------------------------------------------------------------
 * macros
 *--------------------------------------------------------------------------*/

#define hemp_tagset_null(t)             \
    hemp_tagset_free(t);                \
    t = NULL


/*--------------------------------------------------------------------------
 * externals
 *--------------------------------------------------------------------------*/

extern hemp_tag_t HempTagInline;
extern hemp_tag_t HempTagOutline;
extern hemp_tag_t HempTagComment;
extern hemp_tag_t HempTagControl;
extern hemp_tag_t HempTagVariable;
extern hemp_tag_t HempTagEmbed;


#endif /* HEMP_TAGS_H */