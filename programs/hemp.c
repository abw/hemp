#include <hemp.h>
#include <getopt.h>
#include <readline/readline.h>
#include <readline/history.h>


#define HEMP_OPTION_SIG     "vdhtqf:"
#define HEMP_QUIT           "quit"
#define HEMP_EXIT           "exit"
#define HEMP_EXPR_PROMPT    HEMP_ANSI_CYAN "expr> " HEMP_ANSI_RESET

void         hemp_banner();
void         hemp_info();
void         hemp_help();
void         hemp_say(char *format, ...);
void         hemp_warn(char *format, ...);
void         hemp_getopt(hemp_hemp hemp, int argc, char **argv);
void         hemp_interactive(hemp_hemp hemp);
hemp_string   hemp_prompt_init();
void         hemp_prompt_free();
hemp_string   hemp_input_read(hemp_string prompt);
void         hemp_input_free();
char **      hemp_completion(const char *, int, int);
char *       hemp_command_generator(const char *, int);


hemp_bool  hemp_cmd_todo(hemp_hemp, hemp_string);
hemp_bool  hemp_cmd_exprs(hemp_hemp, hemp_string);
hemp_bool  hemp_cmd_expr(hemp_hemp, hemp_string);
hemp_bool  hemp_cmd_help(hemp_hemp, hemp_string);
hemp_bool  hemp_cmd_quit(hemp_hemp, hemp_string);

static int be_quiet  = 0;
static int read_text = 0;
static hemp_string hemp_input  = NULL;
static hemp_string hemp_prompt = NULL;      /* old way */


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
    hemp_bool   (*func)(hemp_hemp, hemp_string);
    hemp_string    name;
    hemp_string    about;
} hemp_command;

hemp_command hemp_commands[] = {
    { &hemp_cmd_expr,   "expr",   "Evaluate a single hemp expression"       },
    { &hemp_cmd_exprs,  "exprs",  "Evaluate hemp expressions, line by line" },
    { &hemp_cmd_todo,   "data",   "Load data definitions from a file"       },
    { &hemp_cmd_todo,   "text",   "Process some document text"              },
    { &hemp_cmd_todo,   "file",   "Process a document file"                 },
    { &hemp_cmd_help,   "help",   "Help on using hemp"                      },
    { &hemp_cmd_quit,   "quit",   "Quit using hemp"                         },
    { NULL, NULL, NULL }
};



#define hemp_nl()                           \
    fprintf(stderr, "\n");

#define hemp_verbose(hemp, format, ...)     \
    if (hemp->verbose) hemp_say(format, ##__VA_ARGS__)




/*--------------------------------------------------------------------------
 * main entry point
 *--------------------------------------------------------------------------*/

int main(
    int  argc, 
    char **argv, 
    char **env
) {
    hemp_hemp       hemp = hemp_new();
    hemp_string     filename;
    hemp_document   document;
    hemp_text       input, output;
    
    hemp_language_instance(hemp, "tt3");
    hemp_prompt_init();
    hemp_getopt(hemp, argc, argv);

    if (! be_quiet)
        hemp_banner();
    
    if (hemp->verbose)
        hemp_info();

    if (optind < argc) {
        if (read_text) {
            filename = "input text";
            input    = hemp_text_new_size(80);
            if (! input)
                hemp_fatal("could not initialise input text");

            while (optind < argc) {
                filename = argv[optind++];
                hemp_text_append_string(input, filename);
                hemp_text_append_string(input, " ");
            }
            // hemp_verbose(hemp, "loaded text: %s", input->string);
            document = hemp_document_instance(
                hemp, HEMP_TT3, HEMP_TEXT, input->string
            );
            
            if (! document)
                hemp_fatal("could not load document: %s", filename);

            output = hemp_document_render(document, NULL);
            if (! output)
                hemp_fatal("could not render document output: %s", filename);

            puts(output->string);
            
            hemp_text_free(input);
            hemp_text_free(output);
            hemp_document_free(document);
        }
        else {
            while (optind < argc) {
                filename = argv[optind++];
                hemp_verbose(hemp, "loading file: %s", filename);

                document = hemp_document_instance(hemp, HEMP_TT3, HEMP_FILE, filename);
                if (! document)
                    hemp_fatal("could not load document: %s", filename);

                output = hemp_document_render(document, NULL);
                if (! output)
                    hemp_fatal("could not render document output: %s", filename);

                puts(output->string);
            
                hemp_text_free(output);
                hemp_document_free(document);
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



/*--------------------------------------------------------------------------
 * interactive mode
 *--------------------------------------------------------------------------*/

hemp_command *
hemp_command_lookup(
    hemp_string name
) {
    int i;
    
    for (i = 0; hemp_commands[i].name; i++) {
        if (hemp_string_eq(name, hemp_commands[i].name)) {
            return &hemp_commands[i];
        }
    }

    return NULL;
}


void hemp_interactive(
    hemp_hemp hemp
) {
    hemp_string  prompt = hemp_prompt;
    hemp_bool done   = HEMP_FALSE;
    hemp_list words;
    hemp_string  input, word, args;
    hemp_command *cmd;

    rl_readline_name = HEMP_STR_HEMP;
//  rl_attempted_completion_function = hemp_completion;
    rl_completion_entry_function = (Function *) &hemp_command_generator;

    while(! done) {
        input = hemp_input_read(prompt);
        
        if (! input) {
            fprintf(stderr, " %s[exit]%s\n", HEMP_ANSI_BLUE, HEMP_ANSI_RESET);
            break;
        }

        words = hemp_string_nwords(input, 2);
        
        if (words) {
            word = hemp_val_str( hemp_list_item(words, 0) );
            args = (words->length > 1)
                ? hemp_val_str( hemp_list_item(words, 1) )
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

void hemp_info() {
    fprintf(
        stderr, "%shemp.dir: %s%s%s\n",
        HEMP_ANSI_YELLOW,
        HEMP_ANSI_CYAN,
        HEMP_DIR,
        HEMP_ANSI_RESET
    );
}


/*--------------------------------------------------------------------------
 * input
 *--------------------------------------------------------------------------*/

hemp_string
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


hemp_string
hemp_input_read(
    hemp_string prompt
) {
    hemp_input_free();

    hemp_input = readline(prompt);
    
    if (hemp_input) {
        hemp_string_trim(hemp_input);
        if (*hemp_input)
            add_history(hemp_input);
    }

    return hemp_input;
}


char **
hemp_completion(
    const char *text, 
    int start, 
    int end
) {
    return rl_completion_matches(text, &hemp_command_generator);
//    hemp_string *matches = NULL;
////    if (start == 0)
//    matches = rl_completion_matches(text, &hemp_command_generator);
}

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
    while ((name = hemp_commands[list_index].name)) {
        list_index++;

        if (hemp_stringn_eq(name, text, len))
            return hemp_string_copy(name);
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

hemp_bool 
hemp_cmd_todo(
    hemp_hemp     hemp, 
    hemp_string input
) {
    hemp_warn("Sorry, that functionality is still TODO");
    return HEMP_FALSE;
}


hemp_bool 
hemp_cmd_exprs(
    hemp_hemp     hemp, 
    hemp_string text
) {
    /* pass text along as first expression */
    if (text) {
        if (hemp_cmd_expr(hemp,text))
            return HEMP_FALSE;
    }

    /* rinse and repeat... */
    while (! hemp_cmd_expr(hemp,text)) {
        /* keep going */
    }

    return HEMP_FALSE;
}


hemp_bool 
hemp_cmd_expr(
    hemp_hemp     hemp, 
    hemp_string text
) {
    hemp_text     input, output;
    hemp_document hemplate;

    if (! text)
        text = hemp_input_read(HEMP_EXPR_PROMPT);
    
    if (! text)
        return HEMP_FALSE;
    
    hemp_string_trim(text);

    if (! *text)
        return HEMP_FALSE;

    input = hemp_text_new_format("[%% %s %%]", text);

    hemplate = NULL;
    output   = NULL;

    HEMP_TRY;
        // hemp_verbose(hemp, "loaded text: %s", input->string);
        hemplate = hemp_document_instance(
            hemp, HEMP_TT3, HEMP_TEXT, input->string
        );
        
        if (! hemplate)
            hemp_fatal("failed to create document... I think this should never happen, but need to check\n");

        output = hemp_document_render(hemplate, NULL);

        if (output)
            puts(output->string);
        else 
            hemp_fatal("could not render document output");


    HEMP_CATCH_ALL;
        hemp_warn(hemp->error->message);

    HEMP_END;

    hemp_text_free(input);

    if (output)
        hemp_text_free(output);
  
// cleaned up automatically  
//    if (hemplate)
//        hemp_document_free(hemplate);


    return HEMP_FALSE;
}


hemp_bool 
hemp_cmd_help(
    hemp_hemp     hemp, 
    hemp_string input
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


hemp_bool 
hemp_cmd_quit(
    hemp_hemp     hemp, 
    hemp_string input
) {
    return HEMP_TRUE;
}


/*--------------------------------------------------------------------------
 * options and help
 *--------------------------------------------------------------------------*/

void
hemp_getopt(
    hemp_hemp  hemp,
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

