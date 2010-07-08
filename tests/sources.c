#include <stdio.h>
#include "hemp.h"

void test_sources();

int
main(int argc, char **argv, char **env)
{
    test_sources();
    return 0;
}


void test_sources() {
    debug("test_sources()\n");
    hemp_scheme_t scheme;
    hemp_source_t source;
    hemp_text_t   text;
    
    (source = hemp_source(HEMP_TEXT, "source/text.html"))
        ? pass("created text source")
        : fail("could not create text source");
    
    (text = hemp_source_read(source))
        ? pass("read text: %s", text)
        : fail("could not read text");

    /* second time around the text should be cached in source->text */
    (text = hemp_source_read(source))
        ? pass("read text again: %s", text)
        : fail("could not read text again");
    
    hemp_source_free(source);


    (source = hemp_source(HEMP_FILE, "source/file.html"))
        ? pass("created file source")
        : fail("could not create file source");
    
    (text = hemp_source_read(source))
        ? pass("read file: %s", text)
        : fail("could not read file");

    (text = hemp_source_read(source))
        ? pass("read file again: %s", text)
        : fail("could not read file again");

    hemp_source_free(source);
}
