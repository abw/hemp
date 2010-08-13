#include <hemp/template.h>


hemp_template_p
hemp_template_init(
    hemp_dialect_p dialect,
    hemp_source_p  source
) {
    hemp_template_p tmpl = (hemp_template_p) hemp_mem_alloc(
        sizeof(struct hemp_template_s)
    );

    if (! tmpl)
        hemp_mem_fail("template");

    tmpl->dialect  = dialect;
    tmpl->source   = source;
    tmpl->tagset   = hemp_tagset_init();
    tmpl->elements = hemp_elements_init(dialect->hemp, 0);
    tmpl->tree     = NULL;
        
    return tmpl;
}


void
hemp_template_free(
    hemp_template_p tmpl
) {
    /* first call any custom cleanup code for the dialect */
    if (tmpl->dialect->cleanup)
        tmpl->dialect->cleanup(tmpl);

    /* then call any cleanup handler defined for the expression tree */
    if (tmpl->tree && tmpl->tree->type->cleanup)
        tmpl->tree->type->cleanup(tmpl->tree);

    /* the elements cleaner will take care of cleaning any other tokens */
    hemp_elements_free(tmpl->elements);

    /* free the source, the tagset and then the template object itself */
    hemp_source_free(tmpl->source);
    hemp_tagset_free(tmpl->tagset);
    hemp_mem_free(tmpl);
}


hemp_element_p
hemp_template_tokens(
    hemp_template_p tmpl
) {
    hemp_debug_call("hemp_template_tokens(%p)\n", tmpl);

    if (! tmpl->elements->head)
        hemp_template_scan(tmpl);

    return tmpl->elements->head;
}


hemp_bool_t
hemp_template_scan(
    hemp_template_p tmpl
) {
    hemp_debug_call("hemp_template_scan(%p)\n", tmpl);
    
    hemp_bool_t result = tmpl->dialect->scanner(tmpl);
    
    return result;
}


hemp_bool_t
hemp_template_compile(
    hemp_template_p tmpl
) {
    hemp_debug_call("hemp_template_compile(%p)\n", tmpl);
    
    hemp_element_p element = hemp_template_tokens(tmpl);
    tmpl->tree = hemp_element_parse(element);

    // TODO: proper error handling
    return HEMP_TRUE;
}


hemp_element_p
hemp_template_tree(
    hemp_template_p tmpl
) {
    hemp_debug_call("hemp_template_tree(%p)\n", tmpl);

    if (! tmpl->tree)
        hemp_template_compile(tmpl);
        
    return tmpl->tree;
}


hemp_text_p
hemp_template_render(
    hemp_template_p tmpl
) {
    hemp_debug_call("hemp_template_render(%p)\n", tmpl);

    hemp_element_p root = hemp_template_tree(tmpl);

    if (! root)
        hemp_fatal("template does not have a root element");
    
    hemp_value_t v = root->type->text(root, NULL, HempNothing);
    return hemp_val_text(v);
}

