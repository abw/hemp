#ifndef HEMP_MACROS_H
#define HEMP_MACROS_H


/*--------------------------------------------------------------------------
 * Macros for registering component constructors and instantiating components
 *--------------------------------------------------------------------------*/

#define hemp_register(hemp, type, name, ctor)                               \
    hemp_factory_register(                                                  \
        hemp->type,                                                         \
        name,                                                               \
        (hemp_actor) ctor,                                                  \
        hemp                                                                \
    )

/* 
 * Calls to this macro should be wrapped in a dedicated function to provide 
 * static type checking of the resource list and to ensure that a local item
 * variable is provided that the macro can increment without affecting the 
 * code in the calling scope - see hemp_register_dialects(), etc., in hemp.c
 */

#define hemp_register_all(hemp, type, item) ({                              \
    while (item && item->name) {                                            \
        hemp_factory_register(                                              \
            hemp->type,                                                     \
            item->name,                                                     \
            (hemp_actor) item->ctor,                                        \
            hemp                                                            \
        );                                                                  \
        item++;                                                             \
    }                                                                       \
})

#define hemp_constructor(hemp, type, name) ({                               \
    hemp_action _cons = hemp_factory_constructor(                           \
        hemp->type,                                                         \
        name                                                                \
    );                                                                      \
    if (! _cons)                                                            \
        hemp_throw(hemp, HEMP_ERROR_INVALID, #type, name);                  \
    _cons;                                                                  \
})

#define hemp_construct(hemp, type, name, ...) ({                            \
    hemp_action _cons = hemp_constructor(hemp, type, name);                 \
    hemp_action_run(_cons, name, __VA_ARGS__);                              \
})

#define hemp_instance(hemp, type, name) ({                                  \
    hemp_memory _item = hemp_factory_instance(                              \
        hemp->type,                                                         \
        name                                                                \
    );                                                                      \
    if (! _item)                                                            \
        hemp_throw(hemp, HEMP_ERROR_INVALID, #type, name);                  \
    _item;                                                                  \
})


#define hemp_register_codec(hemp, name, constructor)                        \
    hemp_register(hemp, codec, name, constructor)

#define hemp_register_dialect(hemp, name, constructor)                      \
    hemp_register(hemp, dialect, name, constructor)

#define hemp_register_element(hemp, name, constructor)                      \
    hemp_register(hemp, element, name, constructor)

#define hemp_register_feature(hemp, name, constructor)                      \
    hemp_register(hemp, feature, name, constructor)

#define hemp_register_grammar(hemp, name, constructor)                      \
    hemp_register(hemp, grammar, name, constructor)

#define hemp_register_language(hemp, name, constructor)                     \
    hemp_register(hemp, language, name, constructor)

#define hemp_register_scheme(hemp, name, constructor)                       \
    hemp_register(hemp, scheme, name, constructor)

#define hemp_register_tag(hemp, name, constructor)                          \
    hemp_register(hemp, tag, name, constructor)

#define hemp_register_viewer(hemp, name, constructor)                       \
    hemp_register(hemp, viewer, name, constructor)


#define hemp_codec_instance(hemp, name)                                     \
    hemp_instance(hemp, codec, name)

#define hemp_dialect_instance(hemp, name)                                   \
    hemp_instance(hemp, dialect, name)

#define hemp_grammar_instance(hemp, name)                                   \
    hemp_instance(hemp, grammar, name)

#define hemp_language_instance(hemp, name)                                  \
    hemp_instance(hemp, language, name)

#define hemp_scheme_instance(hemp, name)                                    \
    hemp_instance(hemp, scheme, name)

#define hemp_viewer_instance(hemp, name)                                    \
    hemp_instance(hemp, viewer, name)

#define hemp_grammar_feature(hemp, grammar, name) ({                        \
    hemp_action _cons = hemp_constructor(hemp, feature, name);              \
    hemp_action_run(_cons, grammar);                                        \
})

#define hemp_element_instance(hemp,type,start,end) ({                       \
        hemp_action _cons = (hemp_action) hemp_factory_constructor(         \
            hemp->element, type                                             \
        );                                                                  \
        if (! _cons)                                                        \
            hemp_throw(hemp, HEMP_ERROR_INVALID, "element", type);          \
        (hemp_element) hemp_action_run(                                     \
            _cons, hemp_element_new(type, start, end)                       \
        );                                                                  \
    })

#define hemp_source_instance(hemp, scheme, source)                          \
    hemp_source_new(                                                        \
        hemp_scheme_instance(hemp, scheme),                                 \
        source                                                              \
    )

#define hemp_tag_construct(hemp, type, name, start, end, grammar) (         \
    (hemp_tag) hemp_construct(hemp, tag, type, name, start, end, grammar)   \
)






/*--------------------------------------------------------------------------
 * Macros for getting and setting configuration values
 *--------------------------------------------------------------------------*/

#define hemp_config_get(hemp, name)         \
    hemp_hash_fetch_dotted(hemp->config, name, hemp->context)

#define hemp_config_set(hemp, name, value)  \
    hemp_hash_store_dotted(hemp->config, name, value, hemp->context)


/*--------------------------------------------------------------------------
 * Thread locking.  Encapsulates a block of code with a locked mutex.
 *--------------------------------------------------------------------------*/

#ifdef HEMP_HAVE_LIBPTHREAD
    #include <pthread.h>
    static  pthread_mutex_t M = PTHREAD_MUTEX_INITIALIZER;
    #define HEMP_MUTEX_LOCK     pthread_mutex_lock(&M);
    #define HEMP_MUTEX_UNLOCK   pthread_mutex_unlock(&M);
#else
    #define HEMP_MUTEX_LOCK
    #define HEMP_MUTEX_UNLOCK
#endif


/*--------------------------------------------------------------------------
 * Memory allocation
 *--------------------------------------------------------------------------*/

#define HEMP_TYPE_ALLOCATE(type, name) ({       \
    name = (hemp_##type) hemp_mem_alloc(        \
        sizeof(struct hemp_##type)              \
    );                                          \
    if (! name)                                 \
        hemp_mem_fail(#type);                   \
})

#define HEMP_TYPE_INSTANCE(type, name) ({       \
    if (! name) {                               \
        HEMP_TYPE_ALLOCATE(type, name);         \
    }                                           \
})

#define HEMP_ALLOCATE(name)                     \
    HEMP_TYPE_ALLOCATE(name, name)

#define HEMP_INSTANCE(name)                     \
    HEMP_TYPE_INSTANCE(name, name)


/*--------------------------------------------------------------------------
 * Macros for declaring language, dialect, tag, grammar, etc. constructors.
 *--------------------------------------------------------------------------*/

#define HEMP_AUTOLOAD(f)                        \
    hemp_bool f(                                \
        hemp_factory    factory,                \
        hemp_string     name                    \
    )

#define HEMP_DIALECT(f)                         \
    hemp_dialect f(                             \
        hemp_hemp   hemp,                       \
        hemp_string name                        \
    )

#define HEMP_ELEMENT(f)                         \
    hemp_element f(                             \
        hemp_hemp       hemp,                   \
        hemp_element    element                 \
    )

#define HEMP_ELEMENTS(f)                        \
    hemp_action f(                              \
        hemp_hemp       hemp,                   \
        hemp_string     name                    \
    )

#define HEMP_GLOBAL_ELEMENT(f)                  \
    hemp_element f()

#define HEMP_GRAMMAR(f)                         \
    hemp_grammar f(                             \
        hemp_hemp       hemp,                   \
        hemp_string     name                    \
    )

#define HEMP_FEATURE(f)                         \
    hemp_grammar f(                             \
        hemp_hemp       hemp,                   \
        hemp_grammar    grammar                 \
    )

#define HEMP_LANGUAGE(f)                        \
    hemp_language f(                            \
        hemp_hemp   hemp,                       \
        hemp_string name                        \
    )

#define HEMP_TAG(f)                             \
    hemp_tag f(                                 \
        hemp_hemp       hemp,                   \
        hemp_string     type,                   \
        hemp_string     name,                   \
        hemp_string     start,                  \
        hemp_string     end,                    \
        hemp_grammar    grammar                 \
    )

#define HEMP_SCANNER(f)                         \
    hemp_bool f(                                \
        hemp_memory     self,                   \
        hemp_document   document                \
    )

#define HEMP_SKIPPER(f)                         \
    hemp_string f(                              \
        hemp_tag        tag,                    \
        hemp_string     src                     \
    )

#define HEMP_VIEWER(f)                          \
    hemp_viewer f(                              \
        hemp_hemp       hemp,                   \
        hemp_string     name                    \
    )

#define HEMP_VIEW(f)                            \
    HEMP_INLINE hemp_value f(                   \
        hemp_viewer     viewer,                 \
        hemp_fragment   fragment,               \
        hemp_context    context,                \
        hemp_value      output                  \
    )

#define HEMP_PREPARE(f)                         \
    hemp_document f(                            \
        hemp_document   document                \
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

#define hemp_document_errmsg(doc, error_no, ...) ({         \
    hemp_error _hemp_err = hemp_error_message(              \
        doc->dialect->hemp,                                 \
        error_no,                                           \
        __VA_ARGS__                                         \
    );                                                      \
    _hemp_err->document = doc;                              \
    hemp_error_throw(doc->dialect->hemp, _hemp_err);        \
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
    hemp_fragment  *fragptr,                \
    hemp_scope      scope,                  \
    hemp_oprec      precedence,             \
    hemp_bool       force

#define HEMP_PREFIX_ARG_NAMES               \
    fragptr, scope, precedence, force

#define HEMP_PREFIX(f)                      \
    HEMP_INLINE hemp_fragment f(            \
        HEMP_PREFIX_ARGS                    \
    )

#define HEMP_POSTFIX_ARGS                   \
    HEMP_PREFIX_ARGS,                       \
    hemp_fragment   lhs 

#define HEMP_POSTFIX_ARG_NAMES              \
    fragptr, scope, precedence, force, lhs

#define HEMP_POSTFIX(f)                     \
    HEMP_INLINE hemp_fragment f(            \
        HEMP_POSTFIX_ARGS                   \
    )

#define HEMP_INFIX_ARGS                     \
    HEMP_POSTFIX_ARGS

#define HEMP_INFIX_ARG_NAMES                \
    HEMP_POSTFIX_ARG_NAMES

#define HEMP_INFIX(f)                       \
    HEMP_POSTFIX(f)

#define HEMP_FIXUP(f)                       \
    HEMP_INLINE hemp_fragment f(            \
        hemp_fragment   fragment,           \
        hemp_scope      scope,              \
        hemp_value      fixative            \
    )


/* operator precedence */

#define HEMP_FPTYPE ((*fragptr)->type)

#define HEMP_PREFIX_DEBUG                                                   \
    hemp_debug_parse(                                                       \
        "prefix precedence reqd is %d  OP:%s is %d\n",                      \
        precedence,                                                         \
        (*fragptr)->type->name, (*fragptr)->type->rprec                     \
    )

#define HEMP_INFIX_DEBUG                                                    \
    hemp_debug_parse(                                                       \
        "infix precedence LHS:%s is %d  OP:%s is %d\n",                     \
        lhs->type->name, precedence,                                        \
        (*fragptr)->type->name, (*fragptr)->type->lprec                     \
    )


// NOTE: this macro produces stupid debugging messages... can't be arsed to fix right now
#define HEMP_PREFIX_PRECEDENCE ({                                           \
    HEMP_PREFIX_DEBUG;                                                      \
    if (precedence && HEMP_FPTYPE->rprec <= precedence) {                   \
        return NULL;                                                        \
    }                                                                       \
})

#define HEMP_INFIX_LEFT_PRECEDENCE ({                                       \
    HEMP_INFIX_DEBUG;                                                       \
    if (precedence && HEMP_FPTYPE->lprec <= precedence) {                   \
        return lhs;                                                         \
    }                                                                       \
})

#define HEMP_INFIX_RIGHT_PRECEDENCE ({                                      \
    HEMP_INFIX_DEBUG;                                                       \
    if (precedence && HEMP_FPTYPE->lprec < precedence) {                    \
        return lhs;                                                         \
    }                                                                       \
})


/*--------------------------------------------------------------------------
 * Element evaluation
 *
 * Each element type defines a function (via the vtable in the grammar 
 * symbol entry for the relevant element type) which evaluates the element
 * and returns a generic value reference (see include/value.h) to represent
 * the result.  Static elements (like chunks of text, numbers, quoted strings
 * and so on) simply return their source text as a value.  Dynamic elements
 * like operators, expressions, keywords, etc., do other stuff and then return
 * some value.
 *
 *--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------
 * Views: comment TODO
 *--------------------------------------------------------------------------*/

#define HEMP_USE_VIEW(name, view)           \
    hemp_viewer_add_view(viewer, name, view);


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
    HEMP_INLINE hemp_value f(               \
        hemp_value      value,              \
        hemp_context    context             \
    )

#define HEMP_INPUT(f)                       \
    HEMP_INLINE hemp_value f(               \
        hemp_value      value,              \
        hemp_context    context,            \
        hemp_value      input               \
    )

#define HEMP_OUTPUT(f)                      \
    HEMP_INLINE hemp_value f(               \
        hemp_value      value,              \
        hemp_context    context,            \
        hemp_value      output              \
    )

#define HEMP_FETCH_FUNC(f)                  \
    HEMP_INLINE hemp_value f(               \
        hemp_value      container,          \
        hemp_context    context,            \
        hemp_value      key                 \
    )

#define HEMP_STORE_FUNC(f)                  \
    HEMP_INLINE hemp_value f(               \
        hemp_value      container,          \
        hemp_context    context,            \
        hemp_value      key,                \
        hemp_value      value               \
    )

#define HEMP_CLEANUP(f)                     \
    HEMP_INLINE void f(                     \
        hemp_fragment   fragment            \
    )


/*--------------------------------------------------------------------------
 * modules and plugins
 *--------------------------------------------------------------------------*/

#define HEMP_PLUGIN(f)                          \
    hemp_bool f(                                \
        hemp_hemp   hemp                        \
    )



/*--------------------------------------------------------------------------
 * error handling
 *--------------------------------------------------------------------------*/

#define HEMP_UNDEF_ERROR(context,type)      \
    hemp_error_throw(                       \
        context->hemp,                      \
        hemp_error_message(                 \
            context->hemp,                  \
            HEMP_ERROR_UNDEF,               \
            type                            \
        )                                   \
    )

#define HEMP_CONVERT_ERROR(context,from,to,val) \
    hemp_error_throw(                       \
        context->hemp,                      \
        hemp_error_message(                 \
            context->hemp,                  \
            HEMP_ERROR_CONVERT,             \
            from, to, val                   \
        )                                   \
    )

#define HEMP_OVERFLOW_ERROR(context,text)   \
    hemp_error_throw(                       \
        context->hemp,                      \
        hemp_error_message(                 \
            context->hemp,                  \
            HEMP_ERROR_OVERFLOW,            \
            text                            \
        )                                   \
    )

#define HEMP_FETCH_ERROR(context,container,key) \
    hemp_error_throw(                       \
        context->hemp,                      \
        hemp_error_message(                 \
            context->hemp,                  \
            HEMP_ERROR_FETCH,               \
            key, container                  \
        )                                   \
    )

#define HEMP_STORE_ERROR(context,container,key) \
    hemp_error_throw(                       \
        context->hemp,                      \
        hemp_error_message(                 \
            context->hemp,                  \
            HEMP_ERROR_STORE,               \
            key, container                  \
        )                                   \
    )


/*--------------------------------------------------------------------------
 * Data types
 *--------------------------------------------------------------------------*/

#define HEMP_TYPE_FUNC(f)                   \
    hemp_type f(                          \
        hemp_int    id,                     \
        hemp_string name                    \
    )


/*--------------------------------------------------------------------------
 * flag manipulation for elements, symbols or anything else with flags
 *--------------------------------------------------------------------------*/

#define hemp_set_flag(item, flag) \
    ((item)->flags |= (flag))

#define hemp_clear_flag(item, flag) \
    ((item)->flags &= ~(flag))

#define hemp_has_flag(item, flag) \
    ((item)->flags & (flag))

#define hemp_not_flag(item, flag) \
    ! (hemp_has_flag(item, flag))


/*--------------------------------------------------------------------------
 * other stuff
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


/* shut up "unused variable" compiler warnings */
#define HEMP_UNUSED(x)  ((void) x)



#endif /* HEMP_MACROS_H */
