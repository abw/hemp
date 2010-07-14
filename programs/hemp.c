#include <stdio.h>
#include <stdarg.h>
#include "config.h"
#include "hemp.h"
#include "hemp/debug.h"


/* TODO: colour should be a configuration/runtime option */

void hemp_banner() {
    fprintf(
        stderr, "%s%s v%s by %s %s%s\n",
        ANSI_GREEN,
        HEMP_NAME,
        HEMP_VERSION,
        HEMP_AUTHOR,
        HEMP_EMAIL,
        ANSI_RESET
    );
}


void hemp_prompt() {
    fprintf(
        stderr, "%s%s%s >%s ",
        ANSI_GREEN, PACKAGE_NAME,
        ANSI_YELLOW, ANSI_RESET
    );
}

#define hemp_nl()                           \
    fprintf(stderr, "\n");



int main(int argc, char **argv, char **env) {
    hemp_t hemp = hemp_init();
    hemp_banner();
    hemp_prompt();
    hemp_nl();
    hemp_free(hemp);
    return 0;
}



