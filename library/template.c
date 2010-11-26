#include <hemp/template.h>


hemp_template
hemp_template_init(
    hemp_dialect dialect,
    hemp_source  source
) {
    hemp_template tmpl = (hemp_template) hemp_mem_alloc(
        sizeof(struct hemp_template)
    );

    if (! tmpl)
        hemp_mem_fail("template");

    tmpl->dialect  = dialect;
    tmpl->source   = source;
    tmpl->tagset   = hemp_tagset_init();
    tmpl->scope    = hemp_scope_new(dialect->hemp);
    tmpl->elements = hemp_elements_new(dialect->hemp, 0);
    tmpl->tree     = NULL;
        
    return tmpl;
}


void
hemp_template_free(
    hemp_template tmpl
) {
    /* First call any custom cleanup code for the dialect */
    if (tmpl->dialect->cleanup)
        tmpl->dialect->cleanup(tmpl);

    /* Then call any cleanup handler defined for the expression tree */
    if (tmpl->tree && tmpl->tree->type->cleanup)
        tmpl->tree->type->cleanup(tmpl->tree);

    /* Free the source, the tagset and then the template object itself. */
    /* The elements cleaner will take care of cleaning any other tokens */
    hemp_elements_free(tmpl->elements);
    hemp_scope_free(tmpl->scope);
    hemp_source_free(tmpl->source);
    hemp_tagset_free(tmpl->tagset);

    hemp_mem_free(tmpl);
}


hemp_element
hemp_template_tokens(
    hemp_template tmpl
) {
    hemp_debug_call("hemp_template_tokens(%p)\n", tmpl);

    if (! tmpl->elements->head)
        hemp_template_scan(tmpl);

    return tmpl->elements->head;
}


hemp_bool
hemp_template_scan(
    hemp_template tmpl
) {
    hemp_debug_call("hemp_template_scan(%p)\n", tmpl);
    
    hemp_bool result = tmpl->dialect->scanner(tmpl);
    
    return result;
}


hemp_bool
hemp_template_compile(
    hemp_template tmpl
) {
    hemp_debug_call("hemp_template_compile(%p)\n", tmpl);
    
    tmpl->tree = hemp_element_parse(
        hemp_template_tokens(tmpl),
        tmpl->scope
    );

    // TODO: proper error handling
    return HEMP_TRUE;
}


hemp_element
hemp_template_tree(
    hemp_template tmpl
) {
    hemp_debug_call("hemp_template_tree(%p)\n", tmpl);

    if (! tmpl->tree)
        hemp_template_compile(tmpl);
        
    return tmpl->tree;
}


hemp_text
hemp_template_render(
    hemp_template tmpl,
    hemp_context  context
) {
    hemp_debug_call("hemp_template_render(%p)\n", tmpl);
    hemp_hemp hemp = tmpl->dialect->hemp;
    hemp_bool my_context = HEMP_FALSE;
    hemp_value v;
    hemp_text output;

    hemp_element root = hemp_template_tree(tmpl);

    if (! context) {
        my_context = HEMP_TRUE;
        context = hemp_context_init(tmpl->dialect->hemp);
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

