#include <hemp.h>
#include <getopt.h>
#include <readline/readline.h>
#include <readline/history.h>


#define HEMP_OPTION_SIG     "vdhtqr:f:e:"
#define HEMP_QUIT           "quit"
#define HEMP_EXIT           "exit"
#define HEMP_EXPR_PROMPT    HEMP_ANSI_CYAN "expr> " HEMP_ANSI_RESET
#define HEMP_TEST_PROMPT    HEMP_ANSI_CYAN "test> " HEMP_ANSI_RESET
#define HEMP_BAD_COMMAND    "Unknown command: %s     (type 'help' for help)"

void        hemp_banner();
void        hemp_info(Hemp);
void        hemp_help();
void        hemp_say(char *format, ...);
void        hemp_warn(char *format, ...);
void        hemp_error_report(Hemp);
void        hemp_getopt(Hemp hemp, int argc, char **argv);
void        hemp_interactive(Hemp hemp);
HempString  hemp_prompt_init();
void        hemp_prompt_free();
HempString  hemp_input_read(HempString prompt);
void        hemp_input_free();
char **     hemp_completion(const char *, int, int);
char *      hemp_command_generator(const char *, int);

HempBool   hemp_cmd_todo(Hemp, HempString);
HempBool   hemp_cmd_exprs(Hemp, HempString);
HempBool   hemp_cmd_expr(Hemp, HempString);
HempBool   hemp_cmd_test(Hemp, HempString);
HempBool   hemp_cmd_help(Hemp, HempString);
HempBool   hemp_cmd_quit(Hemp, HempString);

static int be_quiet  = 0;
static int read_text = 0;
static HempString hemp_input  = NULL;
static HempString hemp_prompt = NULL;      /* old way */


static struct option hemp_options[] = {
    {"execute", required_argument,  NULL, 'e' },
    {"quiet",   no_argument,        NULL, 'q' },
    {"verbose", no_argument,        NULL, 'v' },
    {"debug",   no_argument,        NULL, 'd' },
    {"help",    no_argument,        NULL, 'h' },
    {"file",    required_argument,  NULL, 'f' },
    {"text",    no_argument,        NULL, 't' },
    {"root",    required_argument,  NULL, 'r' },
    {0, 0, 0, 0}
};

typedef struct {
    HempBool   (*func)(Hemp, HempString);
    HempString    name;
    HempString    about;
} hemp_command;

hemp_command hemp_commands[] = {
    { &hemp_cmd_expr,   "expr",   "Evaluate a single hemp expression"       },
    { &hemp_cmd_exprs,  "exprs",  "Evaluate hemp expressions, line by line" },
    { &hemp_cmd_todo,   "data",   "Load data definitions from a file"       },
    { &hemp_cmd_todo,   "text",   "Process some document text"              },
    { &hemp_cmd_test,   "test",   "Run a hemp test script"                  },
    { &hemp_cmd_todo,   "file",   "Process a document file"                 },
    { &hemp_cmd_help,   "help",   "Help on using hemp"                      },
    { &hemp_cmd_quit,   "quit",   "Quit using hemp"                         },
    { NULL, NULL, NULL }
};

hemp_command * execute = NULL;
int exit_val = 0;

hemp_command *hemp_command_lookup_or_die(HempString name);
hemp_command *hemp_command_lookup(HempString name);



#define hemp_nl()                               \
    fprintf(stderr, "\n");

#define hemp_verbose(hemp)                      \
    (hemp_has_flag(hemp, HEMP_OPT_VERBOSE))

#define hemp_verbose_msg(hemp, format, ...)     \
    if (hemp_verbose(hemp))                     \
        hemp_say(format, ##__VA_ARGS__)




/*--------------------------------------------------------------------------
 * main entry point
 *--------------------------------------------------------------------------*/

int main(
    int  argc, 
    char **argv, 
    char **env
) {
    Hemp            hemp = hemp_new();
    HempString      filename;
    HempDocument    document;
    HempText        input, output;
    int             result = 0;
    
    hemp_language(hemp, "tt3");
    hemp_prompt_init();
    hemp_getopt(hemp, argc, argv);

    if (! be_quiet)
        hemp_banner();
    
    if (hemp_verbose(hemp))
        hemp_info(hemp);

    if (execute) {
        // hemp_debug_msg("run %s command with arg: %s\n", execute->name, optind < argc ? argv[optind] : "NULL");
        execute->func(hemp, optind < argc ? argv[optind++] : NULL);
        result = exit_val;   /* hack to allow tests to define exit value */
    }
    else if (optind < argc) {
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
            hemp_verbose_msg(hemp, "loaded text: %s", input->string);
            document = hemp_document(
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
                hemp_verbose_msg(hemp, "loading file: %s", filename);

                document = hemp_document(hemp, HEMP_TT3, HEMP_FILE, filename);
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

    return result;
}



/*--------------------------------------------------------------------------
 * interactive mode
 *--------------------------------------------------------------------------*/

hemp_command *
hemp_command_lookup_or_die(
    HempString name
) {
    hemp_command * cmd = hemp_command_lookup(name);
    if (! cmd) {
        hemp_warn(HEMP_BAD_COMMAND, name);
        exit(1);
    }
    return cmd;
}



hemp_command *
hemp_command_lookup(
    HempString name
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
    Hemp hemp
) {
    HempString prompt = hemp_prompt;
    HempBool   done   = HEMP_FALSE;
    HempString input, word, args;
    HempList   words;
    hemp_command *cmd;

    rl_readline_name = HEMP_STR_HEMP;
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
            
            cmd  = hemp_command_lookup_or_die(word);
            done = cmd->func(hemp, args);

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

void hemp_info(
    Hemp hemp
) {
    HempString dir = hemp_config_get_string(hemp, HEMP_CONFIG_DIR);
    HempValue  doc = hemp_config_get(hemp, HEMP_CONFIG_DOCPATH);
    
    if (! dir)
        dir = HEMP_DIR;

    fprintf(
        stderr, "%shemp.dir: %s%s%s\n",
        HEMP_ANSI_YELLOW,
        HEMP_ANSI_CYAN,
        dir,
        HEMP_ANSI_RESET
    );

    if (hemp_is_found(doc)) {
        if (hemp_is_list(doc)) {
            hemp_debug_msg("found document_path list\n");
        }
        else {
            hemp_debug_msg("found document_path, not a list: %s\n", hemp_type_name(doc));
        }
    }
    else {
        hemp_debug_msg("document_path not found\n");
    }
    
    HempText text = hemp_hash_dump(hemp->config);
    fprintf(
        stderr, "%sconfig: %s%s%s\n",
        HEMP_ANSI_YELLOW,
        HEMP_ANSI_CYAN,
        text->string,
        HEMP_ANSI_RESET
    );
    hemp_text_free(text);
}


/*--------------------------------------------------------------------------
 * input
 *--------------------------------------------------------------------------*/

HempString
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


HempString
hemp_input_read(
    HempString prompt
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
//    HempString *matches = NULL;
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

void hemp_error_report(
    Hemp hemp
) {
    HempText error = hemp_error_text(hemp->error);
    fprintf(stderr, "%s", error->string);
    hemp_text_free(error);
}


/*--------------------------------------------------------------------------
 * commands
 *--------------------------------------------------------------------------*/

HempBool 
hemp_cmd_todo(
    Hemp     hemp, 
    HempString input
) {
    hemp_warn("Sorry, that functionality is still TODO");
    return HEMP_FALSE;
}


HempBool 
hemp_cmd_exprs(
    Hemp   hemp, 
    HempString text
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


HempBool 
hemp_cmd_expr(
    Hemp   hemp, 
    HempString text
) {
    HempText     input, output;
    HempDocument hemplate;

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
        hemp_verbose_msg(hemp, "loaded text: %s", input->string);
        hemplate = hemp_document(
            hemp, HEMP_TT3, HEMP_TEXT, input->string
        );
        
        if (! hemplate)
            hemp_fatal("failed to create document... I think this should never happen, but need to check\n");

// TODO: this is a temporary hack... might want to use a temporary context
// managed by exprs command.
        output = hemp_document_render(hemplate, hemp->context);

        if (output) {
            if (output->string) {
                puts(output->string);
            }
        }
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


HempBool 
hemp_cmd_test(
    Hemp   hemp, 
    HempString test
) {
    HempDocument document;
    HempText     output;

    if (test)
        hemp_debug_msg("test: %s\n", test);

    if (! test)
        test = hemp_input_read(HEMP_TEST_PROMPT);
    
    if (! test)
        return HEMP_FALSE;
    
    hemp_string_trim(test);

    if (! *test)
        return HEMP_FALSE;

    hemp_language(hemp, "test");

    HEMP_TRY;
        document = hemp_document(
            hemp, "test", HEMP_FILE, test
        );
        // TODO: create child context
        output = hemp_document_render(document, hemp->context);

        fprintf(stderr, "OUTPUT:\n%s\n/OUTPUT\n", output->string);

        hemp_text_free(output);

    HEMP_CATCH_ALL;
        HempText error = hemp_error_text(hemp->error);
        hemp_warn(error->string);
        hemp_text_free(error);

    HEMP_END;

    // hack: set this value to indicate test passed/failed
    // exit_val = 1;
    
    return HEMP_FALSE;
}


HempBool 
hemp_cmd_help(
    Hemp     hemp, 
    HempString input
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


HempBool 
hemp_cmd_quit(
    Hemp     hemp, 
    HempString input
) {
    return HEMP_TRUE;
}


/*--------------------------------------------------------------------------
 * options and help
 *--------------------------------------------------------------------------*/

void
hemp_getopt(
    Hemp   hemp,
    int         argc,
    char        **argv
) {
    int opt;
    int ndx = 0;

    while (1) {
        opt = getopt_long(argc, argv, HEMP_OPTION_SIG, hemp_options, &ndx);

        if (opt == -1)
            break;

        switch (opt) {
            case 0:
                /* currently not used */
                printf ("option %s", hemp_options[ndx].name);
                if (optarg)
                    printf (" with arg %s", optarg);
                printf ("\n");
                break;

            case 'v':
                hemp_set_flag(hemp, HEMP_OPT_VERBOSE);
                break;

            case 'd':
                hemp_set_flag(hemp, HEMP_OPT_DEBUG);
                break;

            case 'e':
                execute = hemp_command_lookup_or_die(optarg);
                break;

            case 't':
                read_text = 1;
                break;

            case 'q':
                be_quiet = 1;
                break;

            case 'f':
                HEMP_TRY;
                    /* TODO: make config dialect configurable */
                    hemp_language(hemp, "json");
                    hemp_configure_from(hemp, "json", "file", optarg);
                HEMP_CATCH_ALL;
                    hemp_error_report(hemp);
                    exit(1);
                HEMP_END;
                break;

            case 'r':
                hemp_config_set(hemp, HEMP_CONFIG_DIR, hemp_str_val(optarg));
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
        "    -e cmd  --execute cmd       execute command\n"
        "    -t      --text              read text from comment line arguments\n"
        "    -q      --quiet             quiet mode - no messages\n"
        "    -v      --verbose           enable verbose messages\n"
        "    -r      --root              hemp root directory (default: %s)\n"
        "    -d      --debug             enable debugging messages\n"
        "    -h      --help              this help\n",
        HEMP_DIR
    );
}

