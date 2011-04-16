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

struct hemp_tag {
    HempTagset      tagset;
    HempString      type;
    HempString      name;
    HempTagStyle    style;
    HempString      start;
    HempString      end;
    HempGrammar     grammar;
    HempScanner     scanner;
    HempSkipper     to_eol;
};

struct hemp_tags {
    HempString      name;
    HempTagdef      ctor;
};


/*--------------------------------------------------------------------------
 * Tag factory
 *--------------------------------------------------------------------------*/

HEMP_FACTORY(hemp_tag_factory);
HEMP_HASH_ITERATOR(hemp_tag_cleaner);


/*--------------------------------------------------------------------------
 * function prototypes for generic tags
 *--------------------------------------------------------------------------*/

HempTag
hemp_tag_new(
    HempString      type,
    HempString      name,
    HempString      start,
    HempString      end,
    HempGrammar     grammar
);

HempTag
hemp_tag_copy(
    HempTag         tag
);

void
hemp_tag_set_name(
    HempTag         tag,
    HempString      name
);

void
hemp_tag_set_start(
    HempTag         tag,
    HempString      start
);

void
hemp_tag_set_end(
    HempTag         tag,
    HempString      end
);

void
hemp_tag_set_start_end(
    HempTag         tag,
    HempString      start,
    HempString      end
);

void
hemp_tag_free(
    HempTag         tag
);



/*--------------------------------------------------------------------------
 * function protoypes for builtin tag types
 *--------------------------------------------------------------------------*/

HempMemory hemp_tag_scanner(
    HempActor       self,
    HempDocument    document
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