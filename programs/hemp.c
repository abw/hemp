#include <hemp.h>
#include <getopt.h>

#define HEMP_OPTION_SIG "vdhtqf:"

static int    be_quiet  = 0;
static int    read_text = 0;
static struct option hemp_options[] = {
    {"quiet",   no_argument,        NULL, 'q' },
    {"verbose", no_argument,        NULL, 'v' },
    {"debug",   no_argument,        NULL, 'd' },
    {"help",    no_argument,        NULL, 'h' },
    {"file",    required_argument,  NULL, 'f' },
    {"text",    no_argument,        NULL, 't' },
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
void hemp_getopt(hemp_p hemp, int argc, char **argv);



int main(int argc, char **argv, char **env) {
    hemp_p          hemp = hemp_init();
    hemp_cstr_p     filename;
    hemp_template_p template;
    hemp_text_p     input, output;
    
    hemp_getopt(hemp, argc, argv);

    if (! be_quiet)
        hemp_banner();

    if (optind < argc) {
        if (read_text) {
            filename = "input text";
            input    = hemp_text_init(80);
            if (! input)
                hemp_fatal("could not initialise input text");

            while (optind < argc) {
                filename = argv[optind++];
                hemp_text_append_cstr(input, filename);
                hemp_text_append_cstr(input, " ");
            }
            // hemp_verbose(hemp, "loaded text: %s", input->string);
            template = hemp_template(
                hemp, HEMP_TT3, HEMP_TEXT, input->string
            );
            
            if (! template)
                hemp_fatal("could not load template: %s", filename);

            output = hemp_template_render(template);
            if (! output)
                hemp_fatal("could not render template output: %s", filename);

            puts(output->string);
            
            hemp_text_free(input);
            hemp_text_free(output);
            hemp_template_free(template);
        }
        else {
            while (optind < argc) {
                filename = argv[optind++];
                hemp_verbose(hemp, "loading file: %s", filename);

                template = hemp_template(hemp, HEMP_TT3, HEMP_FILE, filename);
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
        stderr, "%s%s v%s %s by %s %s%s\n",
        ANSI_GREEN,
        HEMP_NAME,
        HEMP_VERSION,
        HEMP_ARCHITECTURE,
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
    hemp_p  hemp,
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

            case 't':
                read_text = 1;
                break;

            case 'q':
                be_quiet = 1;
                break;

            case 'f':
                hemp_todo("option -f %s", optarg);
                break;

            case 'h':
                hemp_banner();
                hemp_help();
                exit(HEMP_ERRNO_HELP);

            case '?':
                fprintf(stderr, "Type '%s --help' for help\n", HEMP_NAME);
                
            default:
                exit(HEMP_ERRNO_GETOPT);
                break;
        }
    }
}

void hemp_help() {
    fprintf(
        stderr,
        "\nUsage:\n"
        "    %s [options] file1 file2 ... file<n>\n", 
        HEMP_NAME
    );
    fprintf(
        stderr,
        "\nOptions:\n"
        "    -t      --text              read text from comment line arguments\n"
        "    -q      --quiet             quiet mode - no messages\n"
        "    -v      --verbose           enable verbose messages\n"
        "    -d      --debug             enable debugging messages\n"
        "    -h      --help              this help\n"
    );
}

