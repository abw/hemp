#ifndef HEMP_MACROS_H
#define HEMP_MACROS_H


/*--------------------------------------------------------------------------
 * Memory allocation
 *--------------------------------------------------------------------------*/

#define HEMP_TYPE_ALLOCATE(type, name) ({                                   \
    name = (struct hemp_##type *) hemp_mem_alloc(                           \
        sizeof(struct hemp_##type)                                          \
    );                                                                      \
    if (! name)                                                             \
        hemp_mem_fail(#type);                                               \
})

#define HEMP_TYPE_INSTANCE(type, name) ({                                   \
    if (! name) {                                                           \
        HEMP_TYPE_ALLOCATE(type, name);                                     \
    }                                                                       \
})

#define HEMP_ALLOCATE(name)                                                 \
    HEMP_TYPE_ALLOCATE(name, name)

#define HEMP_INSTANCE(name)                                                 \
    HEMP_TYPE_INSTANCE(name, name)



/*--------------------------------------------------------------------------
 * Macros for declaring constructor functions for factory components.
 *--------------------------------------------------------------------------*/

#define HEMP_DECLARE(type, func)                                            \
    type func(                                                              \
        Hemp        hemp,                                                   \
        HempString  name                                                    \
    )

#define HEMP_CACHE(f)                                                       \
    HEMP_DECLARE(HempCache, f)

#define HEMP_CODEC(f)                                                       \
    HEMP_DECLARE(HempCodec, f)

#define HEMP_DIALECT(f)                                                     \
    HEMP_DECLARE(HempDialect, f)

#define HEMP_FACTORY(f)                                                     \
    HEMP_DECLARE(HempFactory, f)

#define HEMP_GRAMMAR(f)                                                     \
    HEMP_DECLARE(HempGrammar, f)

#define HEMP_LANGUAGE(f)                                                    \
    HEMP_DECLARE(HempLanguage, f)

#define HEMP_VIEWER(f)                                                      \
    HEMP_DECLARE(HempViewer, f)



/*--------------------------------------------------------------------------
 * Macros for other components.
 *--------------------------------------------------------------------------*/

#define HEMP_AUTOLOAD(f)                                                    \
    HempBool f(                                                             \
        HempFactory     factory,                                            \
        HempString      name                                                \
    )

#define HEMP_ELEMENT(f)                                                     \
    HempElement f(                                                          \
        Hemp            hemp,                                               \
        HempElement     element                                             \
    )

#define HEMP_ELEMENTS(f)                                                    \
    HempAction f(                                                           \
        Hemp            hemp,                                               \
        HempString      name                                                \
    )

#define HEMP_GLOBAL_ELEMENT(f)                                              \
    HempElement f(                                                          \
        HempGlobal      global                                              \
    )

#define HEMP_FEATURE(f)                                                     \
    HempGrammar f(                                                          \
        Hemp            hemp,                                               \
        HempGrammar     grammar                                             \
    )

#define HEMP_SCANNER(f)                                                     \
    HempBool f(                                                             \
        HempMemory      self,                                               \
        HempDocument    document                                            \
    )

#define HEMP_SKIPPER(f)                                                     \
    HempString f(                                                           \
        HempTag         tag,                                                \
        HempString      src                                                 \
    )

#define HEMP_TAG(f)                                                         \
    HempTag f(                                                              \
        Hemp            hemp,                                               \
        HempString      type,                                               \
        HempString      name,                                               \
        HempString      start,                                              \
        HempString      end,                                                \
        HempGrammar     grammar                                             \
    )

#define HEMP_TYPE(f)                                                        \
    HempType f(                                                             \
        HempInt    id,                                                      \
        HempString name                                                     \
    )

#define HEMP_VIEW(f)                                                        \
    HempValue f(                                                            \
        HempViewer      viewer,                                             \
        HempFragment    fragment,                                           \
        HempContext     context,                                            \
        HempValue       output                                              \
    )

#define HEMP_DOC_PREP(f)                                                    \
    HempDocument f(                                                         \
        HempDocument    document                                            \
    )

#define HEMP_DOC_SCAN(f)                                                    \
    HempBool f(                                                             \
        HempDocument    document                                            \
    )

#define HEMP_DOC_CLEAN(f)                                                   \
    void f(                                                                 \
        HempDocument    document                                            \
    )

#define HEMP_HASH_ITERATOR(f)                                               \
    HempBool f(                                                             \
        HempHash        hash,                                               \
        HempPos         position,                                           \
        HempSlot        item                                                \
    )



/*--------------------------------------------------------------------------
 * Factories
 *
 * A factory module is responsible for managing a particular kind of resource.
 * Hemp includes factories for languages, dialects, grammars, tags, elements,
 * and so on.  A factory will attempt to dynamically load a module to
 * satisfy a request for a resource.  In many (but not all) cases, we use
 * factories to maintain singleton instances of various resources.
 *--------------------------------------------------------------------------*/

#define hemp_register_factory(hemp, name, constructor)                      \
    hemp_factory_register(                                                  \
        hemp->factory,                                                      \
        name,                                                               \
        (HempActor) constructor,                                            \
        hemp                                                                \
    )

#define hemp_factory(hemp, name) ({                                         \
    HempFactory _hemp_factory = (HempFactory) hemp_factory_instance(        \
        hemp->factory,                                                      \
        name                                                                \
    );                                                                      \
    if (! _hemp_factory)                                                    \
        hemp_throw(hemp, HEMP_ERROR_INVALID, HEMP_FACTORY_FACTORY, name);   \
    _hemp_factory;                                                          \
})

#define hemp_register(hemp, type, name, ctor)                               \
    hemp_factory_register(                                                  \
        hemp_factory(hemp, type),                                           \
        name,                                                               \
        (HempActor) ctor,                                                   \
        hemp                                                                \
    )

#define hemp_register_all(hemp, type, item) ({                              \
    HempFactory _hemp_fact = hemp_factory(hemp, type);                      \
    while (item && item->name) {                                            \
        hemp_factory_register(                                              \
            _hemp_fact,                                                     \
            item->name,                                                     \
            (HempActor) item->ctor,                                         \
            hemp                                                            \
        );                                                                  \
        item++;                                                             \
    }                                                                       \
})

#define hemp_constructor(hemp, type, name) ({                               \
    HempAction _hemp_cons = hemp_factory_constructor(                       \
        hemp_factory(hemp, type),                                           \
        name                                                                \
    );                                                                      \
    if (! _hemp_cons)                                                       \
        hemp_throw(hemp, HEMP_ERROR_INVALID, type, name);                   \
    _hemp_cons;                                                             \
})

#define hemp_construct(hemp, type, name, ...) ({                            \
    HempAction _hemp_cons = hemp_constructor(hemp, type, name);             \
    hemp_action_run(_hemp_cons, name, __VA_ARGS__);                         \
})

#define hemp_instance(hemp, type, name) ({                                  \
    HempMemory _hemp_item = hemp_factory_instance(                          \
        hemp_factory(hemp, type),                                           \
        name                                                                \
    );                                                                      \
    if (! _hemp_item)                                                       \
        hemp_throw(hemp, HEMP_ERROR_INVALID, type, name);                   \
    _hemp_item;                                                             \
})


/*--------------------------------------------------------------------------
 * Additional macros for registering resources with specific factories
 *--------------------------------------------------------------------------*/

#define hemp_register_codec(hemp, name, constructor)                \
    hemp_register(hemp, HEMP_FACTORY_CODEC, name, constructor)

#define hemp_register_dialect(hemp, name, constructor)              \
    hemp_register(hemp, HEMP_FACTORY_DIALECT, name, constructor)

#define hemp_register_dialects(hemp, dialects)                      \
    hemp_register_all(hemp, HEMP_FACTORY_DIALECT, dialects)

#define hemp_register_element(hemp, name, constructor)              \
    hemp_register(hemp, HEMP_FACTORY_ELEMENT, name, constructor)

#define hemp_register_feature(hemp, name, constructor)              \
    hemp_register(hemp, HEMP_FACTORY_FEATURE, name, constructor)

#define hemp_register_grammar(hemp, name, constructor)              \
    hemp_register(hemp, HEMP_FACTORY_GRAMMAR, name, constructor)

#define hemp_register_language(hemp, name, constructor)             \
    hemp_register(hemp, HEMP_FACTORY_LANGUAGE, name, constructor)

#define hemp_register_scheme(hemp, name, constructor)               \
    hemp_register(hemp, HEMP_FACTORY_SCHEME, name, constructor)

#define hemp_register_tag(hemp, name, constructor)                  \
    hemp_register(hemp, HEMP_FACTORY_TAG, name, constructor)

#define hemp_register_viewer(hemp, name, constructor)               \
    hemp_register(hemp, HEMP_FACTORY_VIEWER, name, constructor)



/*--------------------------------------------------------------------------
 * Macros for fetching and instantiating resources via specific factories
 *--------------------------------------------------------------------------*/

#define hemp_codec(hemp, name)                                      \
    hemp_instance(hemp, HEMP_FACTORY_CODEC, name)

#define hemp_dialect(hemp, name)                                    \
    hemp_instance(hemp, HEMP_FACTORY_DIALECT, name)

#define hemp_grammar(hemp, name)                                    \
    hemp_instance(hemp, HEMP_FACTORY_GRAMMAR, name)

#define hemp_language(hemp, name)                                   \
    hemp_instance(hemp, HEMP_FACTORY_LANGUAGE, name)

#define hemp_scheme(hemp, name)                                     \
    hemp_instance(hemp, HEMP_FACTORY_SCHEME, name)

#define hemp_tag(hemp, type, name, start, end, grammar) (           \
    (HempTag) hemp_construct(hemp, HEMP_FACTORY_TAG, type, name, start, end, grammar)    \
)

#define hemp_viewer(hemp, name)                                     \
    hemp_instance(hemp, HEMP_FACTORY_VIEWER, name)

#define hemp_element(hemp,type,start,end) ({                        \
    HempAction _hemp_cons = hemp_constructor(hemp, HEMP_FACTORY_ELEMENT, type); \
    (HempElement) hemp_action_run(                                  \
        _hemp_cons, hemp_element_new(type, start, end)              \
    );                                                              \
})

#define hemp_grammar_feature(hemp, grammar, name) ({                \
    HempAction _hemp_cons = hemp_constructor(hemp, HEMP_FACTORY_FEATURE, name);  \
    hemp_action_run(_hemp_cons, grammar);                           \
})


#define hemp_source(hemp, scheme, source)                           \
    hemp_source_new(                                                \
        hemp_scheme(hemp, scheme),                                  \
        source                                                      \
    )


/*--------------------------------------------------------------------------
 * Elements
 *
 * A language grammar maps input tokens (words, numbers, punctuation
 * characters, etc) to elements.  An element entry is effectively a vtable
 * containing functions that implement the behaviours for a particular
 * kind of document element (a chunk of text, number, variable, operator,
 * keyword, etc) along with a few other flags and values for housekeeping
 * purpose.
 *
 * A grammar is constructed by asking hemp to create elements to represent
 * the different element types (e.g. via hemp_grammar_add_symbol()).  A
 * unique name is used to identify the element type (e.g. hemp.numop.multiply
 * to represent numerical multiplication).  The relevant token (e.g. '*' or
 * perhaps 'x') and left/right precedence levels are also specified, where
 * appropriate (used to implement operator precedence parsing).
 *
 * HEMP_USE_ELEMENT0(...), HEMP_USE_ELEMENT1(...) and HEMP_USE_ELEMENT2(...) can be used as
 * shortcuts for registering a symbol with a grammar.  The first is for
 * symbols that don't have any particular start or end token (e.g. number,
 * word, text), the second is for those that have a unique start token, (e.g.
 * '+', '#', 'if', etc) and the third is for those that have start and end
 * tokens (e.g. quoted strings like "...", '...', q{...}, etc).
 *
 * HEMP_USE_OPERATOR1(...) and HEMP_USE_OPERATOR2(...) do similar things, but add
 * left and rightward precedence levels as options.
 *
 * See library/language/XXX.c for examples of these in action.
 *--------------------------------------------------------------------------*/


#define HEMP_USE_ELEMENT0(name)                                     \
    hemp_grammar_add_element(grammar, name, NULL, NULL, 0, 0);

#define HEMP_USE_ELEMENT1(name, start)                              \
    hemp_grammar_add_element(grammar, name, start, NULL, 0, 0);

#define HEMP_USE_ELEMENT2(name, start, end)                         \
    hemp_grammar_add_element(grammar, name, start, end,  0, 0);

#define HEMP_USE_OPERATOR1(name, start, lprec, rprec)               \
    hemp_grammar_add_element(grammar, name, start, NULL, lprec, rprec);

#define HEMP_USE_OPERATOR2(name, start, end, lprec, rprec)          \
    hemp_grammar_add_element(grammar, name, start, end, lprec, rprec);

#define HEMP_USE_COMMAND1(name, start)                              \
    hemp_grammar_add_element(grammar, name, start, NULL, 100, 100);

#define HEMP_USE_COMMAND2(name, start, end)                         \
    hemp_grammar_add_element(grammar, name, start, end,  100, 100);

#define HEMP_USE_BLOCK(name, start, prec)                           \
    hemp_grammar_add_element(grammar, name, start, NULL, prec, prec);



/*--------------------------------------------------------------------------
 * Scanning
 *--------------------------------------------------------------------------*/

#define hemp_document_errmsg(doc, error_no, ...) ({                 \
    HempError _hemp_err = hemp_error_message(                       \
        doc->dialect->hemp,                                         \
        error_no,                                                   \
        __VA_ARGS__                                                 \
    );                                                              \
    _hemp_err->document = doc;                                      \
    hemp_error_throw(doc->dialect->hemp, _hemp_err);                \
})



/*--------------------------------------------------------------------------
 * Parsing
 *
 * After the scanner has done its thing we have a single linked list of
 * elements representing the raw tokens in the document.  We use parse
 * functions to build a tree representation of the document.  We skip over
 * any elements that don't generate output (whitespace, comments, tag markers,
 * etc) and organise the rest into the appropriate structure using top-down
 * operator precedence parsing.  See Ch.9 of "Beautiful Code" (O'Reilly,
 * ISBN-13: 978-0-596-51004-6) for a good introduction to the subject.
 *
 * HEMP_PREFIX() and HEMP_POSTFIX() can be used to declare and
 * define expression parsing functions for this purpose.  A prefix function
 * is called when an element appears at the start of an expression.  A
 * postfix function is called when it appears following a preceding
 * expression element.
 *--------------------------------------------------------------------------*/

#define HEMP_PREFIX_ARGS                    \
    HempFragment  * fragptr,                \
    HempScope       scope,                  \
    HempPrec        precedence,             \
    HempBool        force

#define HEMP_PREFIX_ARG_NAMES               \
    fragptr, scope, precedence, force

#define HEMP_PREFIX(f)                      \
    HempFragment f(                         \
        HEMP_PREFIX_ARGS                    \
    )

#define HEMP_POSTFIX_ARGS                   \
    HEMP_PREFIX_ARGS,                       \
    HempFragment    lhs

#define HEMP_POSTFIX_ARG_NAMES              \
    fragptr, scope, precedence, force, lhs

#define HEMP_POSTFIX(f)                     \
    HempFragment f(                         \
        HEMP_POSTFIX_ARGS                   \
    )

#define HEMP_INFIX_ARGS                     \
    HEMP_POSTFIX_ARGS

#define HEMP_INFIX_ARG_NAMES                \
    HEMP_POSTFIX_ARG_NAMES

#define HEMP_INFIX(f)                       \
    HEMP_POSTFIX(f)

#define HEMP_FIXUP(f)                       \
    HempFragment f(                         \
        HempFragment    fragment,           \
        HempScope       scope,              \
        HempValue       fixative            \
    )


/* operator precedence */

#define HEMP_FPTYPE ((*fragptr)->type)

#define HEMP_PREFIX_DEBUG                                           \
    hemp_debug_parse(                                               \
        "prefix precedence reqd is %d  OP:%s is %d\n",              \
        precedence,                                                 \
        (*fragptr)->type->name, (*fragptr)->type->rprec             \
    )

#define HEMP_INFIX_DEBUG                                            \
    hemp_debug_parse(                                               \
        "infix precedence LHS:%s is %d  OP:%s is %d\n",             \
        lhs->type->name, precedence,                                \
        (*fragptr)->type->name, (*fragptr)->type->lprec             \
    )


#define HEMP_PREFIX_PRECEDENCE ({                                   \
    HEMP_PREFIX_DEBUG;                                              \
    if (precedence && HEMP_FPTYPE->lprec <= precedence && ! force) {\
        return NULL;                                                \
    }                                                               \
})

#define HEMP_INFIX_LEFT_PRECEDENCE ({                               \
    HEMP_INFIX_DEBUG;                                               \
    if (precedence && HEMP_FPTYPE->lprec <= precedence) {           \
        return lhs;                                                 \
    }                                                               \
})

#define HEMP_INFIX_RIGHT_PRECEDENCE ({                              \
    HEMP_INFIX_DEBUG;                                               \
    if (precedence && HEMP_FPTYPE->lprec < precedence) {            \
        return lhs;                                                 \
    }                                                               \
})



/*--------------------------------------------------------------------------
 * Values
 *
 * Data values define vtables for coercing one type to another (e.g. number
 * to text) and other methods that can be called on values, e.g. text.length
 *
 * HEMP_VALUE() can be used to declare and define value functions.
 * HEMP_OUTPUT() is a special case for text yielding functions where we
 * allow an existing text/list object to be passed as an extra argument for
 * the function to append the value onto.
 *--------------------------------------------------------------------------*/

#define HEMP_VALUE(f)                       \
    HempValue f(                            \
        HempValue       value,              \
        HempContext     context             \
    )

#define HEMP_INPUT(f)                       \
    HempValue f(                            \
        HempValue       value,              \
        HempContext     context,            \
        HempValue       input               \
    )

#define HEMP_OUTPUT(f)                      \
    HempValue f(                            \
        HempValue       value,              \
        HempContext     context,            \
        HempValue       output              \
    )

#define HEMP_FETCH(f)                       \
    HempValue f(                            \
        HempValue       container,          \
        HempContext     context,            \
        HempValue       key                 \
    )

#define HEMP_STORE(f)                       \
    HempValue f(                            \
        HempValue       container,          \
        HempContext     context,            \
        HempValue       key,                \
        HempValue       value               \
    )

#define HEMP_CLEAN(f)                       \
    void f(                                 \
        HempValue       value               \
    )

#define HEMP_CLEANUP(f)                     \
    void f(                                 \
        HempFragment    fragment            \
    )



/*--------------------------------------------------------------------------
 * error handling
 *--------------------------------------------------------------------------*/

#define HEMP_UNDEF_ERROR(context,type)                              \
    hemp_error_throw(                                               \
        context->hemp,                                              \
        hemp_error_message(                                         \
            context->hemp,                                          \
            HEMP_ERROR_UNDEF,                                       \
            type                                                    \
        )                                                           \
    )

#define HEMP_CONVERT_ERROR(context,from,to,val)                     \
    hemp_error_throw(                                               \
        context->hemp,                                              \
        hemp_error_message(                                         \
            context->hemp,                                          \
            HEMP_ERROR_CONVERT,                                     \
            from, to, val                                           \
        )                                                           \
    )

#define HEMP_OVERFLOW_ERROR(context,text)                           \
    hemp_error_throw(                                               \
        context->hemp,                                              \
        hemp_error_message(                                         \
            context->hemp,                                          \
            HEMP_ERROR_OVERFLOW,                                    \
            text                                                    \
        )                                                           \
    )

#define HEMP_FETCH_ERROR(context,container,key)                     \
    hemp_error_throw(                                               \
        context->hemp,                                              \
        hemp_error_message(                                         \
            context->hemp,                                          \
            HEMP_ERROR_FETCH,                                       \
            key, container                                          \
        )                                                           \
    )

#define HEMP_STORE_ERROR(context,container,key)                     \
    hemp_error_throw(                                               \
        context->hemp,                                              \
        hemp_error_message(                                         \
            context->hemp,                                          \
            HEMP_ERROR_STORE,                                       \
            key, container                                          \
        )                                                           \
    )


/*--------------------------------------------------------------------------
 * flag manipulation for elements, symbols or anything else with flags
 *--------------------------------------------------------------------------*/

#define hemp_set_flag(item, flag)           \
    ((item)->flags |= (flag))

#define hemp_clear_flag(item, flag)         \
    ((item)->flags &= ~(flag))

#define hemp_has_flag(item, flag)           \
    ((item)->flags & (flag))

#define hemp_not_flag(item, flag)           \
    ! (hemp_has_flag(item, flag))


/*--------------------------------------------------------------------------
 * Macros for preparing output values
 *--------------------------------------------------------------------------*/

#define hemp_prepare_text(context, output, text)                    \
    if (hemp_is_undef(output)) {                                    \
        text   = hemp_context_tmp_text(context);                    \
        output = hemp_text_val(text);                               \
    }                                                               \
    else {                                                          \
        text   = hemp_val_text(output);                             \
    }

#define hemp_prepare_text_size(context, output, text, size)         \
    if (hemp_is_undef(output)) {                                    \
        text   = hemp_context_tmp_text_size(context, size);         \
        output = hemp_text_val(text);                               \
    }                                                               \
    else {                                                          \
        text   = hemp_val_text(output);                             \
    }

#define hemp_prepare_values(context, output, list)                  \
    if (hemp_is_undef(output)) {                                    \
        list   = hemp_context_tmp_list(context);                    \
        output = hemp_list_val(list);                               \
    }                                                               \
    else {                                                          \
        list   = hemp_val_list(output);                             \
    }

#define hemp_prepare_params(context, output, params)                \
    if (hemp_is_undef(output)) {                                    \
        params = hemp_context_tmp_params(context);                  \
        output = hemp_params_val(params);                           \
    }                                                               \
    else {                                                          \
        params = hemp_val_params(output);                           \
    }

#define hemp_prepare_pairs(context, output, hash)                   \
    if (hemp_is_undef(output)) {                                    \
        hash   = hemp_context_tmp_hash(context);                    \
        output = hemp_hash_val(hash);                               \
    }                                                               \
    else {                                                          \
        hash   = hemp_val_hash(output);                             \
    }



/*--------------------------------------------------------------------------
 * Macros for getting and setting configuration values
 *--------------------------------------------------------------------------*/

#define hemp_config_get(hemp, name)         \
    hemp_hash_fetch_dotted(hemp->config, name, hemp->context)

#define hemp_config_set(hemp, name, value)  \
    hemp_hash_store_dotted(hemp->config, name, value, hemp->context)



/*--------------------------------------------------------------------------
 * Other stuff
 *--------------------------------------------------------------------------*/

/* shut up "unused variable" compiler warnings */
#define HEMP_UNUSED(x)  ((void) x)


#endif /* HEMP_MACROS_H */
