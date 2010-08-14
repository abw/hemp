#include <hemp.h>
#include <getopt.h>
#include <readline/readline.h>
#include <readline/history.h>


#define HEMP_OPTION_SIG "vdhtqf:"
#define HEMP_QUIT       "quit"
#define HEMP_EXIT       "exit"

void         hemp_banner();
void         hemp_help();
void         hemp_say(char *format, ...);
void         hemp_warn(char *format, ...);
void         hemp_getopt(hemp_p hemp, int argc, char **argv);
void         hemp_interactive(hemp_p hemp);
hemp_cstr_p  hemp_prompt_init();
void         hemp_prompt_free();
hemp_cstr_p  hemp_input_read(hemp_cstr_p prompt);
void         hemp_input_free();
hemp_bool_t  hemp_cmd_help(hemp_p, hemp_cstr_p);
hemp_bool_t  hemp_cmd_quit(hemp_p, hemp_cstr_p);
hemp_bool_t  hemp_cmd_todo(hemp_p, hemp_cstr_p);
char **      hemp_completion(const char *, int, int);
char *       hemp_command_generator(const char *, int);


static int be_quiet  = 0;
static int read_text = 0;
static hemp_cstr_p hemp_input  = NULL;
static hemp_cstr_p hemp_prompt = NULL;

static struct option hemp_options[] = {
    {"quiet",   no_argument,        NULL, 'q' },
    {"verbose", no_argument,        NULL, 'v' },
    {"debug",   no_argument,        NULL, 'd' },
    {"help",    no_argument,        NULL, 'h' },
    {"file",    required_argument,  NULL, 'f' },
    {"text",    no_argument,        NULL, 't' },
    {0, 0, 0, 0}
};

typedef struct {
    hemp_bool_t   (*func)(hemp_p, hemp_cstr_p);
    hemp_cstr_p     name;
    hemp_cstr_p     about;
} hemp_command;

hemp_command hemp_commands[] = {
    { &hemp_cmd_todo, "data",   "Load data definitions" },
    { &hemp_cmd_todo, "text",   "Process some template text" },
    { &hemp_cmd_todo, "file",   "Process a template file" },
    { &hemp_cmd_help, "help",   "Help on using hemp" },
    { &hemp_cmd_quit, "quit",   "Quit using hemp"    },
    { NULL, NULL, NULL }
};



#define hemp_nl()                           \
    fprintf(stderr, "\n");

#define hemp_verbose(hemp, format, ...)     \
    if (hemp->verbose) hemp_say(format, ##__VA_ARGS__)




int main(
    int  argc, 
    char **argv, 
    char **env
) {
    hemp_p          hemp = hemp_init();
    hemp_cstr_p     filename;
    hemp_template_p template;
    hemp_text_p     input, output;
    
    hemp_prompt_init();
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
        hemp_interactive(hemp);
    }
        
    hemp_prompt_free();
    hemp_free(hemp);

    return 0;
}


hemp_command *
hemp_command_lookup(
    hemp_cstr_p name
) {
    int i;
    
    for (i = 0; hemp_commands[i].name; i++) {
        if (hemp_cstr_eq(name, hemp_commands[i].name)) {
            return &hemp_commands[i];
        }
    }

    return NULL;
}


void hemp_interactive(
    hemp_p hemp
) {
    hemp_cstr_p prompt = hemp_prompt;
    hemp_bool_t done   = HEMP_FALSE;
    hemp_list_p words;
    hemp_cstr_p input, word, args;
    hemp_command *cmd;

    rl_readline_name = HEMP_HEMP;
//    rl_attempted_completion_function = hemp_completion;
    rl_completion_entry_function = hemp_command_generator;

    while(! done) {
        input = hemp_input_read(prompt);
        
        if (! input) {
            fprintf(stderr, " %s[exit]%s\n", HEMP_ANSI_BLUE, HEMP_ANSI_RESET);
            break;
        }

        words = hemp_cstr_nwords(input, 2);
        
        if (words) {
            word = (hemp_cstr_p) hemp_list_item(words, 0);
            args = (words->length > 1)
                ? (hemp_cstr_p) hemp_list_item(words, 1)
                : NULL;
            
            cmd = hemp_command_lookup(word);
            
            if (cmd) {
                done = cmd->func(hemp, args);
            }
            else {
                hemp_warn("Unknown command: %s     (type 'help' for help)", word);
            }

            hemp_list_free(words);
        }
    }

    hemp_input_free();

    return;
}



/* TODO: colour should be a configuration/runtime option */

void hemp_banner() {
    fprintf(
        stderr, "%s%s v%s %s by %s %s%s\n",
        HEMP_ANSI_YELLOW,
        HEMP_NAME,
        HEMP_VERSION,
        HEMP_ARCHITECTURE,
        HEMP_AUTHOR,
        HEMP_EMAIL,
        HEMP_ANSI_RESET
    );
}


/*--------------------------------------------------------------------------
 * input
 *--------------------------------------------------------------------------*/

hemp_cstr_p
hemp_prompt_init() {
    if (! hemp_prompt) {
        asprintf(
            &hemp_prompt, "%s%s>%s ",
            HEMP_ANSI_CYAN, HEMP_PROMPT,HEMP_ANSI_RESET
        );
    }
    return hemp_prompt;
}

void
hemp_prompt_free() {
    if (hemp_prompt) {
        free(hemp_prompt);
        hemp_prompt = NULL;
    }
}


hemp_cstr_p
hemp_input_read(
    hemp_cstr_p prompt
) {
    hemp_input_free();

    hemp_input = readline(prompt);
    
    if (hemp_input) {
        hemp_cstr_trim(hemp_input);
        if (*hemp_input)
            add_history(hemp_input);
    }

    return hemp_input;
}


/* Attempt to complete on the contents of TEXT.  START and END bound the
   region of rl_line_buffer that contains the word to complete.  TEXT is
   the word to complete.  We can use the entire contents of rl_line_buffer
   in case we want to do some simple parsing.  Return the array of matches,
   or NULL if there aren't any. */

char **
hemp_completion(
    const char *text, 
    int start, 
    int end
) {
    return rl_completion_matches(text, &hemp_command_generator);

//    return rl_completion_matches(text, &hemp_command_generator);
//    rl_message("found some matches\n");
//    rl_set_prompt("BLAH>");
//
//
//    hemp_cstr_p *matches = NULL;
////    if (start == 0)
//    matches = rl_completion_matches(text, &hemp_command_generator);
////    rl_message("found some matches\n");
//    return matches;
}


/* Generator function for command completion.  STATE lets us know whether
   to start from scratch; without any state (i.e. STATE == 0), then we
   start at the top of the list. */

char *
hemp_command_generator(
    const char *text,
    int state
) {
    static int list_index, len;
    char *name;

    /* If this is a new word to complete, initialize now.  This includes
        saving the length of TEXT for efficiency, and initializing the index
        variable to 0. */
    if (! state) {
        list_index = 0;
        len = strlen(text);
    }

    /* Return the next name which partially matches from the command list. */
    while (name = hemp_commands[list_index].name) {
        list_index++;

        if (hemp_cstrn_eq(name, text, len))
            return hemp_cstr_copy(name);
    }

    return NULL;
}


void
hemp_input_free() {
    if (hemp_input) {
        free (hemp_input);
        hemp_input = NULL;
    }
}


void hemp_say(
    char *format, ...
) {
    va_list args;
    va_start(args, format);
    fprintf(stderr, "%s", hemp_prompt);
    vfprintf(stderr, format, args);
    fprintf(stderr, "\n");
    va_end(args);
}

void hemp_warn(
    char *format, ...
) {
    va_list args;
    va_start(args, format);
    fprintf(stderr, "%shemp> ", HEMP_ANSI_RED);
    vfprintf(stderr, format, args);
    fprintf(stderr, "%s\n", HEMP_ANSI_RESET);
    va_end(args);
}


/*--------------------------------------------------------------------------
 * commands
 *--------------------------------------------------------------------------*/

hemp_bool_t 
hemp_cmd_help(
    hemp_p      hemp, 
    hemp_cstr_p input
) {
    int i;
    hemp_command cmd;
    printf("Hemp commands:\n");
    
    for (i = 0; hemp_commands[i].name; i++) {
        cmd = hemp_commands[i];
        printf(
            "      %s%-12s %s%s%s\n", 
            HEMP_ANSI_CYAN, 
            cmd.name, 
            HEMP_ANSI_YELLOW, 
            cmd.about, 
            HEMP_ANSI_RESET
        );
    }
    return HEMP_FALSE;
}


hemp_bool_t 
hemp_cmd_todo(
    hemp_p      hemp, 
    hemp_cstr_p input
) {
    hemp_warn("Sorry, that functionality is still TODO");
    return HEMP_FALSE;
}


hemp_bool_t 
hemp_cmd_quit(
    hemp_p      hemp, 
    hemp_cstr_p input
) {
    return HEMP_TRUE;
}


/*--------------------------------------------------------------------------
 * options and help
 *--------------------------------------------------------------------------*/

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
                exit(HEMP_ERROR_HELP);

            case '?':
                fprintf(stderr, "Type '%s --help' for help\n", HEMP_NAME);
                
            default:
                exit(HEMP_ERROR_OPTION);
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

