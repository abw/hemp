#include "EXTERN.h"
#include "perl.h"
#include "XSUB.h"
#include "ppport.h"
#include <hemp.h>

typedef hemp_p          Hemp;
typedef hemp_template_p Hemp__Template;
typedef hemp_context_p  Hemp__Context;

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
        RETVAL = hemp_template(hemp, dialect, scheme, source);
    OUTPUT:
        RETVAL


Hemp::Context
context(hemp)
    Hemp   hemp
    INIT:
        fprintf(stderr, "Hemp->context()\n");
    CODE:
        RETVAL = hemp_context(hemp);
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


char *
render(template, context)
    Hemp::Template template
    Hemp::Context  context
    INIT:
        fprintf(stderr, "Hemp::Template->render()\n");
    CODE:
        RETVAL = hemp_template_render(template, context)->string;
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
    char *          name
    char *          value
    INIT:
        fprintf(stderr, "Hemp::Context->set(%s, %s)\n", name, value);
    PPCODE:
        hemp_context_set_string(context, name, value);


void
DESTROY(context)
    Hemp::Context context
    INIT:
        fprintf(stderr, "Hemp::Context->DESTROY()\n"); 
    PPCODE:
        hemp_context_free(context);
