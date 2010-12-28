#include <hemp/document.h>


hemp_document
hemp_document_new(
    hemp_dialect dialect,
    hemp_source  source
) {
    hemp_document document;
    HEMP_ALLOCATE(document);

    document->hemp      = dialect->hemp;
    document->dialect   = dialect;
    document->source    = source;
    document->scope     = hemp_scope_new(dialect->hemp);
    document->fragments = hemp_fragments_new(document, 0);
    document->tree      = NULL;
//  document->scanner   = NULL;
    document->scantags  = NULL;

//  document->tagset   = hemp_tagset_new(document);
        
    return document;
}


void
hemp_document_free(
    hemp_document document
) {
    /* First call any custom cleanup code for the dialect */
    if (document->dialect->cleanup)
        document->dialect->cleanup(document);

    /* Zap the scanner stack if we have one */ 
    if (document->scantags)
        hemp_stack_free(document->scantags);

    /* Free the source, the tagset and then the document object itself. */
    /* The fragments cleaner will take care of cleaning any other tokens */
    hemp_fragments_free(document->fragments);
    hemp_scope_free(document->scope);
    hemp_source_free(document->source);
//    hemp_tagset_free(document->tagset);

    hemp_mem_free(document);
}


hemp_fragment
hemp_document_tokens(
    hemp_document document
) {
    hemp_debug_call("hemp_document_tokens(%p)\n", document);

    if (! document->fragments->head)
        hemp_document_scan(document);

    return document->fragments->head;
}


hemp_bool
hemp_document_scan(
    hemp_document document
) {
    hemp_debug_call("hemp_document_scan(%p)\n", document);

    if (! document->dialect->scanner)
        hemp_fatal("No scanner defined for %s document\n", document->dialect->name);

    if (! document->source->text)
        hemp_source_read(document->source);

    document->scanptr = 
    document->scantok = document->source->text;
    document->scanpos = 0;

    if (! document->scantags)
        document->scantags = hemp_stack_new(HEMP_SCANTAGS_SIZE);

    return document->dialect->scanner(document)
        ? HEMP_TRUE
        : HEMP_FALSE;

//    return hemp_action_run(document->scanner, document)
//        ? HEMP_TRUE
//        : HEMP_FALSE;
}


hemp_bool
hemp_document_compile(
    hemp_document document
) {
    hemp_debug_call("hemp_document_compile(%p)\n", document);
    
    document->tree = hemp_fragment_parse(
        hemp_document_tokens(document),
        document->scope
    );

    // TODO: proper error handling
    return HEMP_TRUE;
}


hemp_fragment
hemp_document_tree(
    hemp_document document
) {
    hemp_debug_call("hemp_document_tree(%p)\n", document);

    if (! document->tree)
        hemp_document_compile(document);
        
    return document->tree;
}


hemp_text
hemp_document_render(
    hemp_document document,
    hemp_context  context
) {
    hemp_debug_call("hemp_document_render(%p)\n", document);
    return hemp_document_process(document, context, NULL);
}


hemp_text
hemp_document_process(
    hemp_document document,
    hemp_context  context,
    hemp_text     output
) {
    hemp_debug_call("hemp_document_process(%p)\n", document);
    hemp_bool       my_context  = HEMP_FALSE;
    hemp_fragment   root        = hemp_document_tree(document);

    if (! context) {
        my_context = HEMP_TRUE;
        context = hemp_context_new(document->dialect->hemp);
    }

    if (! root)
        hemp_fatal("document does not have a root element");

    if (! output)
        output = hemp_text_new();

    root->type->text(hemp_frag_val(root), context, hemp_text_val(output));

//    HEMP_CATCH_ALL;
//       hemp_fatal("Error processing document: %s", hemp->error->message);
//    HEMP_END;

    if (my_context)
        hemp_context_free(context);

    return output;
}



hemp_value
hemp_document_data(
    hemp_document document,
    hemp_context  context
) {
    hemp_debug_call("hemp_document_data(%p)\n", document);
    hemp_bool       my_context  = HEMP_FALSE;
    hemp_fragment   root        = hemp_document_tree(document);
    hemp_value      values;
    hemp_list       results;

    if (! root)
        hemp_fatal("document does not have a root element");
    
    if (! context) {
        my_context  = HEMP_TRUE;
        context     = hemp_context_new(document->dialect->hemp);
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



/*--------------------------------------------------------------------------
 * Hmmm... here's a thought... why don't we just make document a data type
 * have have it forward text, values, pairs, etc, to its children.  Then 
 * we can treat other documents as data types directly instead of having to
 * pre-load their data.
 *--------------------------------------------------------------------------*/

HEMP_OUTPUT(hemp_document_pairs) {
    hemp_document   document = hemp_val_ptr(value);
    hemp_fragment   root     = hemp_document_tree(document);

    hemp_debug_call("hemp_document_pairs(%p)\n", document);

    if (! root)
        hemp_fatal("document does not have a root element");

    if (! root->type->pairs)
        hemp_fatal("root element (%s) does not yield pairs", root->type->name);

    root->type->pairs(hemp_frag_val(root), context, output);

    return output;
}
