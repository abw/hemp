#include <hemp/template.h>


hemp_template
hemp_template_new(
    hemp_dialect dialect,
    hemp_source  source
) {
    hemp_template template;
    HEMP_ALLOCATE(template);

    template->dialect  = dialect;
    template->source   = source;
    template->tagset   = hemp_tagset_new(template);
    template->scope    = hemp_scope_new(dialect->hemp);
    template->elements = hemp_elements_new(dialect->hemp, 0);
    template->tree     = NULL;
        
    return template;
}


void
hemp_template_free(
    hemp_template template
) {
    /* First call any custom cleanup code for the dialect */
    if (template->dialect->cleanup)
        template->dialect->cleanup(template);

    /* Then call any cleanup handler defined for the expression tree */
    if (template->tree && template->tree->type->cleanup)
        template->tree->type->cleanup(template->tree);

    /* Free the source, the tagset and then the template object itself. */
    /* The elements cleaner will take care of cleaning any other tokens */
    hemp_elements_free(template->elements);
    hemp_scope_free(template->scope);
    hemp_source_free(template->source);
    hemp_tagset_free(template->tagset);

    hemp_mem_free(template);
}


hemp_element
hemp_template_tokens(
    hemp_template template
) {
    hemp_debug_call("hemp_template_tokens(%p)\n", template);

    if (! template->elements->head)
        hemp_template_scan(template);

    return template->elements->head;
}


hemp_bool
hemp_template_scan(
    hemp_template template
) {
    hemp_debug_call("hemp_template_scan(%p)\n", template);
    
    hemp_bool result = template->dialect->scanner(template);
    
    return result;
}


hemp_bool
hemp_template_compile(
    hemp_template template
) {
    hemp_debug_call("hemp_template_compile(%p)\n", template);
    
    template->tree = hemp_element_parse(
        hemp_template_tokens(template),
        template->scope
    );

    // TODO: proper error handling
    return HEMP_TRUE;
}


hemp_element
hemp_template_tree(
    hemp_template template
) {
    hemp_debug_call("hemp_template_tree(%p)\n", template);

    if (! template->tree)
        hemp_template_compile(template);
        
    return template->tree;
}


hemp_text
hemp_template_render(
    hemp_template template,
    hemp_context  context
) {
    hemp_debug_call("hemp_template_render(%p)\n", template);
    hemp_hemp hemp = template->dialect->hemp;
    hemp_bool my_context = HEMP_FALSE;
    hemp_value v;
    hemp_text output;

    hemp_element root = hemp_template_tree(template);

    if (! context) {
        my_context = HEMP_TRUE;
        context = hemp_context_init(template->dialect->hemp);
    }

    if (! root)
        hemp_fatal("template does not have a root element");
    
//    hemp_debug("root type: %s\n", root->type->name);

//    HEMP_TRY;
        v = root->type->text(hemp_elem_val(root), context, HempNothing);
//    HEMP_CATCH_ALL;
//       hemp_fatal("Error processing template: %s", hemp->error->message);
//    HEMP_END;

    /* v is a temporary value which will be freed by the context */
//  hemp_debug_mem("making return value mortal: %p\n", v);
    
    output = hemp_text_from_text( hemp_val_text(v) );

    if (my_context)
        hemp_context_free(context);

    return output;
}

