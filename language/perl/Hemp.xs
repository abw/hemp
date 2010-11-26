#include "EXTERN.h"
#include "perl.h"
#include "XSUB.h"
#include "ppport.h"
#include <hemp.h>
#include "types.h"

typedef hemp_hemp          Hemp;
typedef hemp_template Hemp__Template;
typedef hemp_context  Hemp__Context;
typedef hemp_text     Hemp__Text;

/*
#--------------------------------------------------------------------------
# Hemp
#--------------------------------------------------------------------------
*/

MODULE = Hemp           PACKAGE = Hemp

PROTOTYPES: enable


char *
hemp_version()
    CODE:
        RETVAL = hemp_version();
    OUTPUT:
        RETVAL

Hemp
new(package)
    char *package
    INIT:
        fprintf(stderr, "Hemp->new()\n");
    CODE:
        RETVAL = hemp_init();
        hemp_perl_init();
    OUTPUT:
        RETVAL


void 
language(hemp, language)
    Hemp   hemp
    char * language
    INIT:
        fprintf(stderr, "Hemp->language('%s')\n", language); 
    PPCODE:
        hemp_language(hemp, language);


Hemp::Template
template(hemp, dialect, scheme, source)
    Hemp   hemp
    char * dialect
    char * scheme
    char * source
    INIT:
        fprintf(stderr, "Hemp->template('%s', '%s', '%s')\n", dialect, scheme, source);
    CODE:
        RETVAL = hemp_template_instance(hemp, dialect, scheme, source);
    OUTPUT:
        RETVAL


Hemp::Context
context(hemp)
    Hemp   hemp
    INIT:
        fprintf(stderr, "Hemp->context()\n");
    CODE:
        RETVAL = hemp_context_instance(hemp);
    OUTPUT:
        RETVAL


void 
DESTROY(hemp)
    Hemp  hemp
    INIT:
        fprintf(stderr, "Hemp->DESTROY()\n"); 
    PPCODE:
        hemp_free(hemp);


#--------------------------------------------------------------------------
# Hemp::Template
#--------------------------------------------------------------------------

MODULE = Hemp           PACKAGE = Hemp::Template

PROTOTYPES: enable


Hemp::Text
render(template, context)
    Hemp::Template template
    Hemp::Context  context
    INIT:
        fprintf(stderr, "Hemp::Template->render()\n");
    CODE:
        hemp_hemp hemp = context->hemp;
        HEMP_TRY;
            RETVAL = hemp_template_render(template, context);
        HEMP_CATCH_ALL;
            croak("Hemp error: %s", hemp->error->message);
        HEMP_END;
    OUTPUT:
        RETVAL


#--------------------------------------------------------------------------
# Hemp::Context
#--------------------------------------------------------------------------

MODULE = Hemp           PACKAGE = Hemp::Context

PROTOTYPES: enable


void 
set(context, name, value)
    Hemp::Context   context
    char *  name
    SV   *  value
    INIT:
        fprintf(stderr, "Hemp::Context->set(%s, %s)\n", name, SvPV_nolen(value));
    PPCODE:
        hemp_context_set(context, name, hemp_perl_value(value));


void
DESTROY(context)
    Hemp::Context context
    INIT:
        fprintf(stderr, "Hemp::Context->DESTROY()\n"); 
    PPCODE:
        hemp_context_free(context);


#--------------------------------------------------------------------------
# Hemp::Text
#--------------------------------------------------------------------------

MODULE = Hemp           PACKAGE = Hemp::Text

PROTOTYPES: enable


char *
text(text)
    Hemp::Text text
    INIT:
        fprintf(stderr, "Hemp::Text->text()\n");
    CODE:
        RETVAL = text->string;
    OUTPUT:
        RETVAL


void
DESTROY(text)
    Hemp::Text text
    INIT:
        fprintf(stderr, "Hemp::Text->DESTROY()\n"); 
    PPCODE:
        hemp_text_free(text);

