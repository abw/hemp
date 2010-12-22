#include <hemp/grammar.h>


hemp_grammar
hemp_grammar_new(
    hemp_hemp   hemp,
    hemp_string name
) {
    hemp_grammar grammar;
    HEMP_ALLOCATE(grammar);

    grammar->hemp      = hemp;
    grammar->elements  = hemp_hash_new();
//  grammar->keywords  = hemp_hash_new();
    grammar->operators = hemp_ptree_new(HEMP_OPERATORS_SIZE);
    grammar->name      = hemp_string_clone(name, "grammar name");

    return grammar;
}


hemp_element
hemp_grammar_new_element(
    hemp_grammar    grammar,
    hemp_string     etype,
    hemp_string     start,
    hemp_string     end
) {
    hemp_debug_call(
        "new [%s => %s] symbol\n", 
        start ? start : "NULL", etype
    );

    hemp_action constructor = hemp_factory_constructor(
        grammar->hemp->element, etype
    );
    if (! constructor)
        hemp_throw(grammar->hemp, HEMP_ERROR_INVALID, "element", etype);

    hemp_element element = hemp_element_new(
        etype, start, end
    );

    element = (hemp_element) hemp_action_run(
        constructor, element 
    );

    if (! element)
        hemp_throw(grammar->hemp, HEMP_ERROR_INVALID, "element", etype);

    return element;
}


hemp_element
hemp_grammar_add_element(
    hemp_grammar    grammar,
    hemp_string     etype,
    hemp_string     start,
    hemp_string     end,
    hemp_oprec      lprec,
    hemp_oprec      rprec
) {
    hemp_debug_call(
        "adding [%s => %s] symbol to %s grammar [%d|%d]\n", 
        start, etype, grammar->name, lprec, rprec
    );

    /* Any element with a start token goes into the operator prefix tree 
     * (a modified ternary search tree) which allows the scanner to easily 
     * and efficiently match longest tokens, e.g. so that '++' is 
     * recognised as one single token, not two instances of '+'
     *
     * Elements without start tokens can't be matched directly by a parser,
     * but may be synthesised into an element tree by other fragments as 
     * part of the parsing process (e.g. fragments that create a hemp.block
     * to store their body content).  We use the fully-qualified element type
     * name (e.g. hemp.block) as a hash index for retrieving the symbol table
     * entry.
     *
     * Either way, the element type name must be unique for a grammar.
     */
    hemp_string name = start ? start : etype;

    if (hemp_hash_fetch_pointer(grammar->elements, name))
        hemp_throw(grammar->hemp, HEMP_ERROR_DUPLICATE, "element", name);

    hemp_element element = hemp_grammar_new_element(
        grammar, etype, start, end
    );

//  hemp_debug_msg("created new element: %p (%s)\n", element, element->name);

    element->lprec   = lprec;
    element->rprec   = rprec;
    element->grammar = grammar;

    if (start) {
        hemp_ptree_store(
            grammar->operators, start, (hemp_memory) element
        );
    }

    hemp_hash_store_pointer(grammar->elements, name, element);

    return element;
}


HEMP_INLINE hemp_element
hemp_grammar_element(
    hemp_grammar  grammar,
    hemp_string   name
) {
    hemp_element element = (hemp_element) hemp_hash_fetch_pointer(
        grammar->elements, name
    );

    if (! element) {
        hemp_fatal(
            "Invalid element %s not found in %s grammar",
            name, grammar->name
        );
    }

    return element;
}


void
hemp_grammar_free(
    hemp_grammar grammar
) {
    hemp_hash_each(grammar->elements, &hemp_grammar_free_element);
    hemp_hash_free(grammar->elements);
    hemp_ptree_free(grammar->operators);
    hemp_mem_free(grammar->name);
    hemp_mem_free(grammar);
}


hemp_bool
hemp_grammar_free_element(
    hemp_hash     grammars,
    hemp_pos      position,
    hemp_slot     item
) {
    hemp_element_free( (hemp_element) hemp_val_ptr(item->value) );
    return HEMP_TRUE;
}


// unplugged scanner
hemp_memory
hemp_grammar_scanner(
    hemp_actor      self,
    hemp_document   document
) {
    hemp_debug_call("hemp_grammar_scanner()\n");

    hemp_grammar    grammar  = (hemp_grammar) self;
    hemp_string     src      = document->scanptr;
    hemp_pnode      pnode;
    hemp_element    element;

    while (*src) {
        if (isspace(*src)) {
            /* whitespace */
            hemp_scan_while(src, isspace);
            hemp_document_scanned_to(document, HempElementSpace, src);
        }
        else if (isdigit(*src)) {
            /* number */
            hemp_scan_number(document);
            src = document->scanptr;
        }
        else if (
            (pnode   = hemp_ptree_root(grammar->operators, src))
        &&  (element = (hemp_element) hemp_pnode_match_more(pnode, &src))
        ) {
            /* keyword/operator */
            if (isalpha(*src) && isalpha(*(src - 1)))
                goto bareword;

            if (element->scanner) {
                document->scanptr = src;
                element->scanner(element, document);
                src = document->scanptr;
            }
            else {
                hemp_document_scanned_to(
                    document, element, src
                );
            }
        }
        else if (isalpha(*src)) {
bareword:
            /* word */
            hemp_scan_while(src, isalnum);
            // TODO: check for ':' following after, e.g. file:/blah/blah
            hemp_document_scanned_to(
                document, HempElementWord, src
            );
        }
        else {
            hemp_throw(document->dialect->hemp, HEMP_ERROR_TOKEN, src);
            break;
        }
    }

    document->scanptr = src;
    
    hemp_fragments_add_eof(document->fragments, document->scanpos);

    return (hemp_memory) document;
}
