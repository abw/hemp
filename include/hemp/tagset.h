#ifndef HEMP_TAGSET_H
#define HEMP_TAGSET_H

#include <hemp/core.h>
#include <hemp/ptree.h>
#include <hemp/tag.h>
#include <hemp/element.h>
#include <hemp/type/hash.h>


/*--------------------------------------------------------------------------
 * type definitions
 *--------------------------------------------------------------------------*/

struct hemp_tagset {
   hemp_template    template;
   hemp_element     text_element;
   hemp_hash        tags;
   hemp_ptree       inline_tags;
   hemp_ptree       outline_tags;
   hemp_tag         unplugged_tag;
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

hemp_tag
hemp_tagset_add_tag(
    hemp_tagset     tagset, 
    hemp_tag        tag
);

hemp_tag
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

hemp_tagset
hemp_tagset_prepare(
    hemp_template   template
);

void
hemp_tagset_cleanup(
    hemp_template template
);

HEMP_SCANNER(hemp_tagset_scanner);


#endif /* HEMP_TAGS_H */
