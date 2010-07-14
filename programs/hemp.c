#include <stdio.h>
#include <stdarg.h>
#include <getopt.h>
#include "config.h"
#include "hemp.h"
#include "hemp/debug.h"

#define HEMP_OPTION_SIG "vdhf:"

static int    wibble = 0;                       // just testing getopt_long
static struct option hemp_options[] = {
    {"verbose", no_argument,        NULL,   'v' },
    {"debug",   no_argument,        NULL,   'd' },
    {"help",    no_argument,        NULL,   'h' },
    {"file",    required_argument,  NULL,   'f' },
    {"wibble",  optional_argument,  &wibble, 99 },
    {"wobble",  optional_argument,  &wibble, 101},
    {0, 0, 0, 0}
};

#define hemp_nl()                           \
    fprintf(stderr, "\n");

#define hemp_verbose(hemp, format, ...)     \
    if (hemp->verbose) hemp_say(format, ##__VA_ARGS__)


void hemp_banner();
void hemp_prompt();
void hemp_help();
void hemp_say(char *format, ...);
void hemp_getopt(hemp_t hemp, int argc, char **argv);



int main(int argc, char **argv, char **env) {
    hemp_t          hemp = hemp_init();
    hemp_cstr_t     filename;
    hemp_template_t template;
    hemp_text_t     output;
    
    hemp_banner();
    hemp_getopt(hemp, argc, argv);

    if (optind < argc) {
        while (optind < argc) {
            filename = argv[optind++];
            hemp_verbose(hemp, "loading file: %s", filename);

            template = hemp_template_init(HEMP_FILE, filename, NULL);
            if (! template)
                hemp_fatal("could not load template: %s", filename);

            output = hemp_template_render(template);
            if (! output)
                hemp_fatal("could not render template output: %s", filename);

            puts(output->string);
            
            hemp_text_free(output);
            hemp_template_free(template);
        }
    }
    else {
        hemp_prompt();
        hemp_nl();
        hemp_todo("interactive mode");
    }
        
    hemp_free(hemp);
    return 0;
}



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
        ANSI_GREEN, HEMP_PROMPT,
        ANSI_YELLOW, ANSI_RESET
    );
}


void hemp_say(char *format, ...)
{
    va_list args;
    va_start(args, format);
    hemp_prompt();
    vfprintf(stderr, format, args);
    fprintf(stderr, "\n");
    va_end(args);
}



void
hemp_getopt(
    hemp_t  hemp,
    int     argc,
    char    **argv
) {
    int opt;
    int ndx = 0;

    while (1) {
        opt = getopt_long(argc, argv, HEMP_OPTION_SIG, hemp_options, &ndx);

        if (opt == -1)
            break;

        switch (opt) {
            case 0:
                printf ("option %s", hemp_options[ndx].name);
                if (optarg)
                    printf (" with arg %s", optarg);
                printf ("\n");
                break;

            case 'v':
                hemp->verbose = HEMP_TRUE;
                break;

            case 'd':
                hemp->debug = HEMP_TRUE;
                break;

            case 'f':
                hemp_todo("option -f %s", optarg);
                break;

            case 'h':
                hemp_help();
                exit(HEMP_ERRNO_HELP);

            case '?':
            default:
                exit(HEMP_ERRNO_GETOPT);
                break;
        }
    }
}

void hemp_help() {
    fprintf(
        stderr,
        "\nUsage: %s [options] file1 file2 ... file<n>\n", 
        HEMP_NAME
    );
    fprintf(
        stderr,
        "\nOptions: \n"
        "\n"
        "   -v      --verbose           enable verbose messages\n"
        "   -d      --debug             enable debugging messages\n"
        "   -h      --help              this help\n"
    );
}

