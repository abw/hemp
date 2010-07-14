#include "hemp/template.h"
#include "hemp/dialect.h"
#include "hemp/debug.h"


hemp_template_t 
hemp_template_init(
    hemp_cstr_t    scheme, 
    hemp_cstr_t    source,
    hemp_dialect_t dialect
) {
    hemp_template_t tmpl;
    
    if (! dialect)
        dialect = hemp_dialect_tt3();

    if ((tmpl = (hemp_template_t) hemp_mem_init(sizeof(struct hemp_template)))) {
        tmpl->source   = hemp_source(scheme, source);
        tmpl->elements = hemp_elements_init(0);
        tmpl->dialect  = dialect;
        
        if (tmpl->source && tmpl->elements) {
            debug_mem(
                "Allocated %s %s template at %p\n", 
                dialect->name, scheme, tmpl
            );
        }
        else {
            hemp_template_null(tmpl);
        }
    }

    // TODO handle tmpl == NULL
    return tmpl;
}


void
hemp_template_free(
    hemp_template_t tmpl
) {
    if (tmpl->source) {
        hemp_source_free(tmpl->source);
    }
    if (tmpl->elements) {
        hemp_elements_free(tmpl->elements);
    }
    debug_cyan("Releasing %s template at %p\n", tmpl->source->scheme->name, tmpl);
    hemp_mem_free(tmpl);
}


hemp_element_t
hemp_template_tokens(
    hemp_template_t tmpl
) {
    if (! tmpl->elements->head) {
        if (tmpl->dialect->scanner(tmpl)) {
            debug_green("scanned OK\n");
        }
        else {
            debug_red("scan failed\n");
        }
    }
    return tmpl->elements->head;
}


hemp_bool_t
hemp_template_scan(
    hemp_template_t tmpl
) {
    hemp_bool_t result = tmpl->dialect->scanner(tmpl);
    
    if (result) {
        // TODO: proper exception handling
        debug_green("scanned OK\n");
    }
    else {
        debug_red("scan failed\n");
    }

    return result;
}


hemp_bool_t
hemp_template_compile(
    hemp_template_t tmpl
) {
    hemp_element_t element = hemp_template_tokens(tmpl);
    tmpl->tree = hemp_element_parse(element);
    debug("got tree: %p\n", tmpl->tree);
    // TODO: proper error handling
    return HEMP_TRUE;
}


hemp_element_t
hemp_template_tree(
    hemp_template_t tmpl
) {
    if (! tmpl->tree)
        hemp_template_compile(tmpl);
        
    return tmpl->tree;
}


hemp_text_t
hemp_template_render(
    hemp_template_t tmpl
) {
    hemp_element_t  root = hemp_template_tree(tmpl);

    if (! root)
        hemp_fatal("template does not have a root element");
        
    hemp_text_fn tfn = root->type->text;
    hemp_text_t text = root->type->text(root, NULL);
    return text;
}

