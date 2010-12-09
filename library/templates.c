#include <hemp/documents.h>

// NOTE: may not be required after all - putting functionality straight into 
// hemp

hemp_documents_p
hemp_documents_init(
    hemp_hemp hemp
) {
    hemp_documents_p documents = (hemp_documents_p) hemp_mem_alloc(
        sizeof(struct hemp_documents_s)
    );

    if (! documents)
        hemp_mem_fail("documents");

    return documents;
}


void
hemp_documents_free(
    hemp_documents_p documents
) {
    hemp_mem_free(documents);
}

