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
   HempDocument    document;
   HempElement     text_element;
   HempHash        tags;
   HempPtree       inline_tags;
   HempPtree       outline_tags;
   HempTag         unplugged_tag;
};


/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

HempTagset
hemp_tagset_new(
    HempDocument   document
);
    
void
hemp_tagset_free();

HempTag
hemp_tagset_add_tag(
    HempTagset     tagset, 
    HempTag        tag
);

HempTag
hemp_tagset_new_tag(
    HempTagset     tagset, 
    HempString     type,
    HempString     name,
    HempString     start,
    HempString     end,
    HempGrammar    grammar
);

HempBool
hemp_tagset_free_tag(
    HempHash       tags,
    HempPos        index,
    HempSlot       item
);

HempBool
hemp_tagset_scanner(
    HempTagset     tagset,
    HempDocument   document
);


#endif /* HEMP_TAGSET_H */
