#ifndef HEMP_MACROS_H
#define HEMP_MACROS_H


/*--------------------------------------------------------------------------
 * Languages
 *
 * In this context a language is a template language, or more precisely a 
 * language "pack". The builtin core language is, rather unsurprisingly,
 * called "hemp". It defines all the core elements required for basic 
 * template processing. The other builtin language is called "tt3" and is
 * (or will be) an implementation of v3 of the Perl Template Toolkit 
 * (see http://template-toolkit.org/). This builds on the basic language 
 * features provided by "hemp" and adds a number of high-level keywords and
 * commands.
 * 
 * The HEMP_LANGUAGE(...) macro can be used as a shortcut for registering
 * a language with a hemp instance.  See library/language/*.c for examples.
 *--------------------------------------------------------------------------*/

#define HEMP_LANGUAGE(name, constructor) \
    hemp_register_language(hemp, name, (hemp_actor_f) constructor);

#define HEMP_LANGUAGE_FUNC(f)               \
    hemp_language_p f(                      \
        hemp_p      hemp,                   \
        hemp_cstr_p name                    \
    )


/*--------------------------------------------------------------------------
 * Dialects
 *
 * A language can have one or more different dialects that are variations of 
 * the same basic language.  Each dialect specifies the language grammar(s) 
 * that can be embedded inside template documents and the tag tokens used to 
 * embed them (e.g. [% ... %]).  
 *
 * The HEMP_DIALECT(...) macro is a shortcut for registering a language 
 * dialect with a hemp instance.  HEMP_DIALECT_FUNC(...) is a shortcut for
 * defining the dialect callback function that initialises the dialect on 
 * demand.
 *--------------------------------------------------------------------------*/

#define HEMP_DIALECT(name, constructor)     \
    hemp_register_dialect(hemp, name, (hemp_actor_f) constructor);

#define HEMP_DIALECT_FUNC(f)                \
    hemp_dialect_p f(                       \
        hemp_p      hemp,                   \
        hemp_cstr_p name                    \
    )


/*--------------------------------------------------------------------------
 * Grammars
 *
 * Within an embedded tag a grammar defines the mapping of tokens to symbols
 * 
 * The HEMP_GRAMMAR(...) macro is a shortcut for registering a grammar with
 * a hemp instance.
 *--------------------------------------------------------------------------*/

#define HEMP_GRAMMAR(name, constructor)     \
    hemp_register_grammar(hemp, name, (hemp_actor_f) constructor);

#define HEMP_GRAMMAR_FUNC(f)                \
    hemp_grammar_p f(                       \
        hemp_p      hemp,                   \
        hemp_cstr_p name                    \
    )


/*--------------------------------------------------------------------------
 * Symbols
 *
 * A language grammar maps input tokens (words, numbers, punctuation 
 * characters, etc) to symbols.  A symbol entry is effectively a vtable
 * containing functions that implement the behaviours for a particular 
 * kind of template element (a chunk of text, number, variable, operator, 
 * keyword, etc) along with a few other flags and values for housekeeping
 * purpose.
 *
 * A grammar is constructed by asking hemp to create symbols to represent 
 * the different element types (e.g. via hemp_grammar_add_symbol()).  A 
 * unique name is used to identify the element type (e.g. hemp.numop.multiply 
 * to represent numerical multiplication).  The relevant token (e.g. '*' or 
 * perhaps 'x') and left/right precedence levels are also specified, where 
 * appropriate (used to implement operator precedence parsing).
 *
 * HEMP_SYMBOLS_FUNC(name) (note plural) can be used to declare/define a 
 * function which can provide hemp with a list of symbols, e.g. all the 
 * hemp.numop.* symbols.
 *
 * HEMP_SYMBOL_FUNC(name) (not singular) can be used to declare/define a 
 * function which initialises a single symbol type.
 *
 * HEMP_SYMBOL0(...), HEMP_SYMBOL1(...) and HEMP_SYMBOL2(...) can be used as 
 * shortcuts for registering a symbol with a grammar.  The first is for 
 * symbols that don't have any particular start or end token (e.g. number,
 * word, text), the second is for those that have a unique start token, (e.g.
 * '+', '#', 'if', etc) and the third is for those that have start and end 
 * tokens (e.g. quoted strings like "...", '...', q{...}, etc).
 *
 * HEMP_OPERATOR1(...) and HEMP_OPERATOR2(...) do similar things, but add 
 * left and rightward precedence levels as options.
 *
 * See library/language/*.c for examples of these in action.
 *--------------------------------------------------------------------------*/

#define HEMP_SYMBOLS_ARGS                   \
    hemp_p          hemp,                   \
    hemp_cstr_p     name

#define HEMP_SYMBOLS_ARG_NAMES              \
    hemp, name

#define HEMP_SYMBOLS_FUNC(f)                \
    hemp_action_p f(                        \
        hemp_p      hemp,                   \
        hemp_cstr_p name                    \
    )

#define HEMP_SYMBOL_ARGS                    \
    hemp_p          hemp,                   \
    hemp_symbol_p   symbol

#define HEMP_SYMBOL_ARG_NAMES               \
    hemp, symbol

#define HEMP_SYMBOL_FUNC(f)                 \
    hemp_symbol_p f(                        \
        HEMP_SYMBOL_ARGS                    \
    )

//#define HEMP_SYMBOL0(name)                                      \
//    hemp_grammar_add_symbol(grammar, name, NULL, NULL, 0, 0);

#define HEMP_SYMBOL1(name, start)                               \
    hemp_grammar_add_symbol(grammar, name, start, NULL, 0, 0);

#define HEMP_SYMBOL2(name, start, end)                          \
    hemp_grammar_add_symbol(grammar, name, start, end,  0, 0);

#define HEMP_OPERATOR1(name, start, lprec, rprec)               \
    hemp_grammar_add_symbol(grammar, name, start, NULL, lprec, rprec);

#define HEMP_OPERATOR2(name, start, end, lprec, rprec)          \
    hemp_grammar_add_symbol(grammar, name, start, end, lprec, rprec);


/*--------------------------------------------------------------------------
 * Elements
 *
 * Right down at the very bottom of it all we have elements.  Elements are 
 * data structures representing a parsed template.  Each element has a pointer
 * the grammar symbol that defines what kind of element it is.
 *
 * HEMP_ELEMENTS(...) is a macro for registering a set of element types
 * (e.g. hemp.numop.*).  Hemp will automatically call the registered callback
 * when any matching elements are requested for adding to a grammar as a 
 * symbol (e.g. a grammar using hemp.numop.multiply will trigger a call to 
 * the provider registered for hemp.numop.*).  HEMP_ELEMENT(...) is used to
 * register a callback to construct an individual element type (e.g. 
 * hemp.numop.multiply).  The HEMP_ELEMENT_FUNC() macro can be used to 
 * declare/define the element constructor callback function.
 *--------------------------------------------------------------------------*/

#define HEMP_ELEMENTS(symbols)              \
    hemp_register_elements(hemp, symbols);

#define HEMP_ELEMENT(name, constructor)     \
    hemp_register_element(hemp, name, (hemp_actor_f) constructor);

#define HEMP_ELEMENT_FUNC(f)                \
    HEMP_NO_INLINE hemp_symbol_p f(         \
        hemp_p          hemp,               \
        hemp_symbol_p   symbol              \
    )


/*--------------------------------------------------------------------------
 * Scanning
 *
 * The main template scanner (see library/scanner.c) scans the template text 
 * for embedded tags (as specified by the dialect defined for the current 
 * template) and then calls the function registered to scan the tag.  The
 * callback should create one or more elements and attach them to the 
 * template->elements stream by calling hemp_elements_append().
 * 
 * HEMP_SCAN_FUNC(name) can be used as a shortcut to declare and define such 
 * functions.  HEMP_SCAN_ERROR(...) can be used inside scanner functions to
 * report errors.  It relies on the local definition of the HEMP_SCAN_ARGS.
 *--------------------------------------------------------------------------*/

#define HEMP_SCAN_ARGS                      \
    hemp_template_p tmpl,                   \
    hemp_tag_p      tag,                    \
    hemp_cstr_p     start,                  \
    hemp_pos_t      pos,                    \
    hemp_cstr_p    *srcptr,                 \
    hemp_symbol_p   symbol

#define HEMP_SCAN_ARG_NAMES                 \
    tmpl, tag, start, pos, srcptr, symbol

#define HEMP_SCAN_FUNC(f)                   \
    hemp_element_p f(                       \
        HEMP_SCAN_ARGS                      \
    )

#define HEMP_SCAN_ERROR(type,...)           \
    hemp_scan_error(                        \
        tmpl->dialect->hemp,                \
        HEMP_SCAN_ARG_NAMES,                \
        HEMP_ERROR_##type,                  \
        __VA_ARGS__                         \
    )


/*--------------------------------------------------------------------------
 * Parsing
 *
 * After the scanner has done its thing we have a single linked list of 
 * elements representing the raw tokens in the template.  We use parse 
 * functions to build a tree representation of the template.  We skip over
 * any elements that don't generate output (whitespace, comments, tag markers,
 * etc) and organise the rest into the appropriate structure using top-down
 * operator precedence parsing.  See Ch.9 of "Beautiful Code" (O'Reilly, 
 * ISBN-13: 978-0-596-51004-6) for a good introduction to the subject.
 *
 * HEMP_PARSE_FUNC() and HEMP_INFIX_FUNC() can be used to declare and define
 * expression parsing functions for this purpose.
 *--------------------------------------------------------------------------*/

#define HEMP_PARSE_ARGS                     \
    hemp_element_p *elemptr,                \
    hemp_scope_p    scope,                  \
    hemp_prec_t     precedence,             \
    hemp_bool_t     force

#define HEMP_PARSE_ARG_NAMES                \
    elemptr, scope, precedence, force

#define HEMP_PARSE_FUNC(f)                  \
    HEMP_DO_INLINE hemp_element_p f(        \
        HEMP_PARSE_ARGS                     \
    )

#define HEMP_INFIX_ARGS                     \
    HEMP_PARSE_ARGS,                        \
    hemp_element_p  lhs 

#define HEMP_INFIX_ARG_NAMES                \
        elemptr, scope, precedence, force, lhs

#define HEMP_INFIX_FUNC(f)                  \
    HEMP_DO_INLINE hemp_element_p f(        \
        HEMP_INFIX_ARGS                     \
    )


/*--------------------------------------------------------------------------
 * Values
 *
 * Each element type defines a function (via the vtable in the grammar 
 * symbol entry for the relevant element type) which evaluates the element
 * and returns a generic value reference (see include/value.h) to represent
 * the result.  Static elements (like chunks of text, numbers, quoted strings
 * and so on) simply return their source text as a value.  Dynamic elements
 * like operators, expressions, keywords, etc., do other stuff and then return
 * some value.
 *
 * HEMP_VALUE_FUNC() can be used to declare and define value functions.
 *--------------------------------------------------------------------------*/

#define HEMP_VALUE_ARGS                     \
        hemp_element_p  element,            \
        hemp_context_p  context

#define HEMP_VALUE_ARG_NAMES                \
        element, context

#define HEMP_VALUE_FUNC(f)                  \
    HEMP_DO_INLINE hemp_value_t f(          \
        HEMP_VALUE_ARGS                     \
    )


/*--------------------------------------------------------------------------
 * Output
 *
 * To render a parsed template we simply render each expression in the 
 * template and concatenate the output text generated.  As an optimisation
 * we can pass a text value (see include/text.h and include/value.h) as the 
 * optional third argument.  The element will then append its output text to
 * this (automatically growing) buffer rather creating a new text value of
 * of its own that we then have to worry about freeing up afterwards.  If 
 * you don't pass an output buffer (e.g. by passing the special HempNothing
 * value instead) then it *will* return a text object which needs to be 
 * freed via hemp_text_free(HEMP_VAL_STR(output)).
 *
 * HEMP_OUTPUT_FUNC() can be used, yes, you guessed it, to declare and define
 * output functions that work this way.
 *--------------------------------------------------------------------------*/

#define HEMP_OUTPUT_ARGS                    \
    hemp_element_p  element,                \
    hemp_context_p  context,                \
    hemp_value_t    output

#define HEMP_OUTPUT_ARG_NAMES               \
    element, context, output

#define HEMP_OUTPUT_FUNC(f)                 \
    HEMP_DO_INLINE hemp_value_t f(          \
        HEMP_OUTPUT_ARGS                    \
    )


#endif /* HEMP_MACROS_H */