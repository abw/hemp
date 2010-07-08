#ifndef HEMP_TAGS_H
#define HEMP_TAGS_H

#include "hemp/ptree.h"
#include "hemp/element.h"

#define HEMP_DEFAULT_TAGSET_SIZE 8

typedef struct hemp_tag    *hemp_tag_t;
typedef struct hemp_tagset *hemp_tagset_t;
typedef void (* hemp_tag_scan_fn)();          // TODO: args

extern hemp_tag_t HempTagInline;
extern hemp_tag_t HempTagOutline;
extern hemp_tag_t HempTagComment;
extern hemp_tag_t HempTagControl;
extern hemp_tag_t HempTagVariable;
extern hemp_tag_t HempTagEmbed;

struct hemp_tag {
   hemp_ident_t     name;
   hemp_ident_t     start;
   hemp_ident_t     end;
   hemp_tag_scan_fn scan;
};

struct hemp_tagset {
   hemp_etype_t     text_element;
   hemp_ptree_t     inline_tags;
   hemp_ptree_t     outline_tags;
};


hemp_tagset_t   hemp_tagset_init();
void            hemp_tagset_free();

hemp_pnode_t    hemp_tagset_add_inline_tag(
    hemp_tagset_t   tagset, 
    hemp_tag_t      tag
);

hemp_pnode_t    hemp_tagset_add_outline_tag(
    hemp_tagset_t   tagset, 
    hemp_tag_t      tag
);

#endif /* HEMP_TAGS_H */