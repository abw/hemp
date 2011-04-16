#include <hemp/test.h>


void test_documents();


int
main(
    int  argc,
    char **argv,
    char **env
) {
    plan(5);
    test_documents();
    return done();
}


void test_documents() {
    Hemp hemp = hemp_new();
    ok( hemp, "created hemp object" );

    HempDocuments documents = hemp_documents_new(hemp);
    ok( documents, "created documents" );
    hemp_documents_free(documents);

    HempDocuments hdocs1 = hemp_documents(hemp);
    ok( hdocs1, "fetched hemp documents" );

    HempDocuments hdocs2 = hemp_documents(hemp);
    ok( hdocs2, "fetched hemp documents again" );

    ok( hdocs1 == hdocs2, "hemp documents are the same instance" );

    hemp_free(hemp);
}


