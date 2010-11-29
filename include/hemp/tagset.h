#ifndef HEMP_TAGSET_H
#define HEMP_TAGSET_H

#include <hemp/core.h>
#include <hemp/ptree.h>
#include <hemp/tag.h>
#include <hemp/element.h>
#include <hemp/type/hash.h>


/*--------------------------------------------------------------------------
 * data structures
 *--------------------------------------------------------------------------*/

struct hemp_tagset {
   hemp_template    template;
   hemp_symbol      text_symbol;
   hemp_hash        tags;
   hemp_ptree       inline_tags;
   hemp_ptree       outline_tags;
};


/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

hemp_tagset
hemp_tagset_new(
    hemp_template   template
);
    
void
hemp_tagset_free();

hemp_pnode
hemp_tagset_add_tag(
    hemp_tagset     tagset, 
    hemp_tag        tag
);

hemp_pnode
hemp_tagset_new_tag(
    hemp_tagset     tagset, 
    hemp_string     type,
    hemp_string     name,
    hemp_string     start,
    hemp_string     end,
    hemp_grammar    grammar
);

hemp_bool
hemp_tagset_free_tag(
    hemp_hash       tags,
    hemp_pos        index,
    hemp_slot       item
);


#endif /* HEMP_TAGS_H */
