#ifndef HEMP_TAGSET_H
#define HEMP_TAGSET_H

#include <hemp/core.h>
#include <hemp/ptree.h>
#include <hemp/tag.h>
#include <hemp/element.h>
#include <hemp/type/hash.h>

/*
#include "hemp/elements.h"
#include "hemp/template.h"
#include "hemp/utils.h"
*/


/*--------------------------------------------------------------------------
 * data structures
 *--------------------------------------------------------------------------*/

struct hemp_tagset_s {
   hemp_symbol    text_symbol;
   hemp_hash      tags;
   hemp_ptree_p     inline_tags;
   hemp_ptree_p     outline_tags;
};


/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

hemp_tagset_p
    hemp_tagset_init();
    
void
    hemp_tagset_free();

hemp_pnode_p
    hemp_tagset_add_tag(
        hemp_tagset_p   tagset, 
        hemp_tag      tag
    );

hemp_pnode_p
    hemp_tagset_new_tag(
        hemp_tagset_p    tagset, 
        hemp_string       name,
        hemp_tag_style_t style,
        hemp_string       start,
        hemp_string       end,
        hemp_tag_scan_f  scan,
        hemp_grammar   grammar
    );

hemp_bool
    hemp_tagset_free_tag(
        hemp_hash     tags,
        hemp_pos      index,
        hemp_slot     item
    );


/*--------------------------------------------------------------------------
 * externals
 *--------------------------------------------------------------------------*/

/*
extern hemp_tag_t HempTagInline;
extern hemp_tag_t HempTagOutline;
extern hemp_tag_t HempTagComment;
extern hemp_tag_t HempTagControl;
extern hemp_tag_t HempTagVariable;
extern hemp_tag_t HempTagEmbed;
*/

#endif /* HEMP_TAGS_H */