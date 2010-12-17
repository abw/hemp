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
    hemp_debug_msg("hemp_grammar_scanner()\n");

    hemp_grammar    grammar  = (hemp_grammar) self;
    hemp_string     text     = document->source->text,
                    src      = text,
                    from     = text;
    hemp_pos        pos      = 0;
    hemp_num        num_val  = 0;
    hemp_int        int_val  = 0;
    hemp_fragment   fragment;
    hemp_element    element;
    hemp_bool       is_int, is_word;
    hemp_pnode      pnode;

    is_word = HEMP_FALSE;

    while (*src) {
        if (isspace(*src)) {
            /* whitespace */
            hemp_scan_while(src, isspace);
            hemp_debug_token("SPACE", from, src-from);
            hemp_fragments_add_fragment(
                document->fragments, HempElementSpace,
                from, pos, src - from
            );
        }
        else if (isdigit(*src)) {
            /* number - try integer first */
            errno   = 0;
            int_val = strtol(src, &src, 0);
            is_int  = HEMP_TRUE;

            /* If there's a decimal point and a digit following then it's a 
             * floating point number.  We also look out for e/E which also
             * indicate fp numbers in scientific notation, e.g. 1.23e6.
             * Note that we don't accept the decimal point if the next 
             * character is not a digit.  This is required to support methods
             * called against numeric constants, e.g. 12345.hex 
             */
            if ( ( *src == '.' && isdigit(*(src + 1)) )
              || ( *src == 'e' || *src == 'E' )
            )  {
                is_int  = HEMP_FALSE;
                num_val = strtod(from, &src);
            }

            if (errno == ERANGE) {
                /* TODO: trim next token out of text */
                hemp_throw(document->dialect->hemp, HEMP_ERROR_OVERFLOW, from);
            }
            else if (errno) {
                /* should never happen (famous last words) as we pre-check 
                 * that there is at least one valid digit available to be 
                 * parsed, but we check anyway
                 */
                hemp_fatal("Unknown number parsing error: %d", errno);
            }
            else if (is_int) {
                hemp_debug_token("INTEGER", from, src-from);
                fragment = hemp_fragments_add_fragment(
                    document->fragments, HempElementInteger,
                    from, pos, src - from
                );
                fragment->args.value = hemp_int_val(int_val);
            }
            else {
                hemp_debug_token("NUMBER", from, src-from);
                fragment = hemp_fragments_add_fragment(
                    document->fragments, HempElementNumber,
                    from, pos, src - from
                );
                fragment->args.value = hemp_num_val(num_val);
            }
        }
        else if (
            (pnode   = hemp_ptree_root(grammar->operators, src))
        &&  (element = (hemp_element) hemp_pnode_match_more(pnode, &src))
        ) {
            hemp_debug_token("OPERATOR", from, src-from);

            /* We have to be check that we haven't matched the first part of
             * a longer word, e.g. matching 'le' at the start of 'length'.
             * However, we also have to account for the fact that elements
             * may contain a mixture of word and non-word characters, e.g.
             * 'C<', 'q/' and so on.  It's OK to have a non-word followed by
             * a word, e.g. 'q/a' ('q/' is the operator, 'a' the next word),
             * or a word followed by a non-word, e.g. '+foo'.
             *
             * If we find that we are at a word/word boundary then we pretend
             * this bit never happened by jumping down to the next check...
             * don't look if you're squeamish about the use of "goto"
             */
            if (isalpha(*src) && isalpha(*(src - 1)))
                goto bareword;

            if (element->scanner) {
                hemp_debug_msg("TODO: can't call element scanner for %s without tag\n", element->name);
                // symbol->scanner(document, tag, from, pos, &src, symbol);
            }
            else {
                hemp_fragments_add_fragment(
                    document->fragments, element,
                    from, pos, src - from
                );
            }
        }
        else if (isalpha(*src)) {
bareword:
            /* word */
            hemp_scan_while(src, isalnum);
            // TODO: check for ':' following after, e.g. file:/blah/blah
            hemp_debug_token("WORD", from, src-from);
            hemp_fragments_add_fragment(
                document->fragments, HempElementWord,
                from, pos, src - from
            );
        }
        else {
            hemp_throw(document->dialect->hemp, HEMP_ERROR_TOKEN, src);
            break;
        }

        pos += src - from;
        from = src;
    }
    
    hemp_fragments_add_eof(document->fragments, pos);

    return (hemp_memory) document;
}
