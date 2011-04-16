#include <hemp/documents.h>


HempDocuments
hemp_documents_new(
    Hemp hemp
) {
    HempDocuments documents;
    HEMP_ALLOCATE(documents);

    documents->hemp     = hemp;
    documents->dialect  = NULL;

    return documents;
}


HempDocuments
hemp_documents_init(
    HempDocuments   documents,
    HempValue       config
) {
    HEMP_INSTANCE(documents);
    return documents;
}


void
hemp_documents_free(
    HempDocuments documents
) {
    hemp_mem_free(documents);
}


