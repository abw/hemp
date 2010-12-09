#ifndef HEMP_TAG_H
#define HEMP_TAG_H

#include <hemp/core.h>
#include <hemp/element.h>
#include <hemp/ptree.h>
#include <hemp/grammar.h>
#include <hemp/scanner.h>


/*--------------------------------------------------------------------------
 * type definitions
 *--------------------------------------------------------------------------*/

enum hemp_tag_style {
    HEMP_INLINE_TAG    = 1,
    HEMP_OUTLINE_TAG   = 2,
    HEMP_UNPLUGGED_TAG = 3
};

typedef hemp_tag 
(* hemp_tag_f)(
    hemp_hemp       hemp,
    hemp_string     type,
    hemp_string     name,
    hemp_string     start,
    hemp_string     end,
    hemp_grammar    grammar
);

struct hemp_tag {
    hemp_tagset     tagset;
    hemp_string     type;
    hemp_string     name;
    hemp_tag_style  style;
    hemp_string     start;
    hemp_string     end;
    hemp_grammar    grammar;
    hemp_scanner    scanner;
    hemp_skipper    to_eol;
};

struct hemp_tags {
    hemp_string     name;
    hemp_tag_f      ctor;
};


/*--------------------------------------------------------------------------
 * function prototypes for generic tags
 *--------------------------------------------------------------------------*/

hemp_tag
hemp_tag_new(
    hemp_string     type,
    hemp_string     name,
    hemp_string     start,
    hemp_string     end,
    hemp_grammar    grammar
);

//    hemp_tag_style  style,
//    hemp_tag_scan_f scan,

hemp_tag
hemp_tag_copy(
    hemp_tag        tag
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

hemp_memory hemp_tag_scanner(
    hemp_actor      self,
    hemp_document   document
);

HEMP_SKIPPER(hemp_tag_to_eol);
HEMP_SKIPPER(hemp_tag_inline_to_eol);

HEMP_TAG(hemp_tag_inline);
HEMP_TAG(hemp_tag_outline);
HEMP_TAG(hemp_tag_comment);
HEMP_TAG(hemp_tag_control);
HEMP_TAG(hemp_tag_unplugged);

HEMP_SCANNER(hemp_tag_inline_scanner);
HEMP_SCANNER(hemp_tag_outline_scanner);
HEMP_SCANNER(hemp_tag_control_scanner);
HEMP_SCANNER(hemp_tag_comment_scanner);

HEMP_AUTOLOAD(hemp_tag_autoload);


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