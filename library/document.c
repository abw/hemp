#include <hemp/document.h>


HempDocument
hemp_document_new(
    HempDialect dialect,
    HempSource  source
) {
    HempDocument document;
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
    HempDocument document
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


HempFragment
hemp_document_tokens(
    HempDocument document
) {
    hemp_debug_call("hemp_document_tokens(%p)\n", document);

    if (! document->fragments->head)
        hemp_document_scan(document);

    return document->fragments->head;
}


HempBool
hemp_document_scan(
    HempDocument document
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


HempBool
hemp_document_compile(
    HempDocument document
) {
    hemp_debug_call("hemp_document_compile(%p)\n", document);
    
    document->tree = hemp_fragment_parse(
        hemp_document_tokens(document),
        document->scope
    );

    // TODO: proper error handling
    return HEMP_TRUE;
}


HempFragment
hemp_document_tree(
    HempDocument document
) {
    hemp_debug_call("hemp_document_tree(%p)\n", document);

    if (! document->tree)
        hemp_document_compile(document);
        
    return document->tree;
}


HempText
hemp_document_render(
    HempDocument document,
    HempContext  context
) {
    hemp_debug_call("hemp_document_render(%p)\n", document);
    return hemp_document_process(document, context, NULL);
}


HempText
hemp_document_process(
    HempDocument document,
    HempContext  context,
    HempText     output
) {
    hemp_debug_call("hemp_document_process(%p)\n", document);
    HempBool       my_context  = HEMP_FALSE;
    HempFragment   root        = hemp_document_tree(document);

    if (! context) {
        my_context = HEMP_TRUE;
        context = hemp_context_new(document->hemp);
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



HempValue
hemp_document_data(
    HempDocument document,
    HempContext  context
) {
    hemp_debug_call("hemp_document_data(%p)\n", document);
    HempBool       my_context  = HEMP_FALSE;
    HempFragment   root        = hemp_document_tree(document);
    HempValue      values;
    HempList       results;

    if (! root)
        hemp_fatal("document does not have a root element");
    
    if (! context) {
        my_context  = HEMP_TRUE;
        context     = hemp_context_new(document->hemp);
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
    HempDocument   document = hemp_val_ptr(value);
    HempFragment   root     = hemp_document_tree(document);

    hemp_debug_call("hemp_document_pairs(%p)\n", document);

    if (! root)
        hemp_fatal("document does not have a root element");

    if (! root->type->pairs)
        hemp_fatal("root element (%s) does not yield pairs", root->type->name);

    root->type->pairs(hemp_frag_val(root), context, output);

    return output;
}
