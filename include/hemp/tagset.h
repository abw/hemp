#ifndef HEMP_TAGSET_H
#define HEMP_TAGSET_H

#include <hemp/core.h>
#include <hemp/hash.h>
#include <hemp/ptree.h>
#include <hemp/tag.h>
#include <hemp/element.h>

/*
#include "hemp/elements.h"
#include "hemp/template.h"
#include "hemp/utils.h"
*/


/*--------------------------------------------------------------------------
 * data structures
 *--------------------------------------------------------------------------*/

struct hemp_tagset_s {
   hemp_etype_p     text_element;
   hemp_hash_p      tags;
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
        hemp_tag_p      tag
    );

hemp_pnode_p
    hemp_tagset_new_tag(
        hemp_tagset_p    tagset, 
        hemp_cstr_p      name,
        hemp_tag_style_t style,
        hemp_cstr_p      start,
        hemp_cstr_p      end,
        hemp_tag_scan_f  scan,
        hemp_grammar_p   grammar
    );

hemp_bool_t
    hemp_tagset_free_tag(
        hemp_hash_p      tags,
        hemp_pos_t       index,
        hemp_hash_item_p item
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