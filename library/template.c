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
//    template->tagset   = hemp_tagset_new(template);
    template->scope    = hemp_scope_new(dialect->hemp);
    template->fragments = hemp_fragments_new(dialect->hemp, 0);
    template->tree     = NULL;
    template->scanner  = NULL;
        
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
    // no longer required as block element is managed by fragments
//    if (template->tree && template->tree->type->cleanup) {
//        hemp_debug_msg("cleaning up template tree\n");
//        template->tree->type->cleanup(template->tree);
//        hemp_debug_msg("done\n");
//    }

    /* Free the source, the tagset and then the template object itself. */
    /* The fragments cleaner will take care of cleaning any other tokens */
    hemp_fragments_free(template->fragments);
    hemp_scope_free(template->scope);
    hemp_source_free(template->source);
//    hemp_tagset_free(template->tagset);

    hemp_mem_free(template);
}


hemp_fragment
hemp_template_tokens(
    hemp_template template
) {
    hemp_debug_call("hemp_template_tokens(%p)\n", template);

    if (! template->fragments->head)
        hemp_template_scan(template);

    return template->fragments->head;
}


hemp_bool
hemp_template_scan(
    hemp_template template
) {
    hemp_debug_msg("hemp_template_scan(%p)\n", template);

    if (! template->scanner)
        hemp_fatal("No scanner defined for %s template\n", template->dialect->name);

    if (! template->source->text)
        hemp_source_read(template->source);

    return hemp_action_run(template->scanner, template)
        ? HEMP_TRUE
        : HEMP_FALSE;
}


hemp_bool
hemp_template_compile(
    hemp_template template
) {
    hemp_debug_call("hemp_template_compile(%p)\n", template);
    
    template->tree = hemp_fragment_parse(
        hemp_template_tokens(template),
        template->scope
    );

    // TODO: proper error handling
    return HEMP_TRUE;
}


hemp_fragment
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
//    hemp_hemp hemp = template->dialect->hemp;
    hemp_bool my_context = HEMP_FALSE;
    hemp_value v;
    hemp_text output;

    hemp_fragment root = hemp_template_tree(template);

    if (! context) {
        my_context = HEMP_TRUE;
        context = hemp_context_new(template->dialect->hemp);
    }

    if (! root)
        hemp_fatal("template does not have a root element");
    
//    hemp_debug("root type: %s\n", root->type->name);

//    HEMP_TRY;
        v = root->type->text(hemp_frag_val(root), context, HempNothing);
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



hemp_value
hemp_template_data(
    hemp_template template,
    hemp_context  context
) {
    hemp_debug_call("hemp_template_data(%p)\n", template);
//    hemp_hemp       hemp        = template->dialect->hemp;
    hemp_bool       my_context  = HEMP_FALSE;
    hemp_fragment   root        = hemp_template_tree(template);
    hemp_value      values;
    hemp_list       results;

    if (! root)
        hemp_fatal("template does not have a root element");
    
    if (! context) {
        my_context  = HEMP_TRUE;
        context     = hemp_context_new(template->dialect->hemp);
    }

    values  = root->type->values(hemp_frag_val(root), context, HempNothing);
    results = hemp_val_list(values);
    
    if (my_context)
        hemp_context_free(context);

    if (results->length > 1) {
        return values;
    }
    else if (results->length == 1) {
        return hemp_list_item(results, 0);
    }
    else {
        return HempEmpty;
    }
}
