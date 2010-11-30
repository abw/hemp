#ifndef HEMP_MACROS_H
#define HEMP_MACROS_H


/*--------------------------------------------------------------------------
 * Macros for registering component constructors and instantiating components
 *--------------------------------------------------------------------------*/

#define hemp_register(hemp, type, name, constructor)                        \
    hemp_factory_register(                                                  \
        hemp->type,                                                         \
        name,                                                               \
        (hemp_actor) constructor,                                           \
        hemp                                                                \
    )

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


#define hemp_register_dialect(hemp, name, constructor)                      \
    hemp_register(hemp, dialect, name, constructor)

#define hemp_register_element(hemp, name, constructor)                      \
    hemp_register(hemp, element, name, constructor)

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


#define hemp_symbol_instance(hemp,type,start,end) ({                        \
        hemp_action _cons = (hemp_action) hemp_factory_constructor(         \
            hemp->element, type                                             \
        );                                                                  \
        if (! _cons)                                                        \
            hemp_throw(hemp, HEMP_ERROR_INVALID, "element", type);          \
        (hemp_symbol) hemp_action_run(                                      \
            _cons, hemp_symbol_new(type, start, end)                        \
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
 * Thread locking.  Encapsulates a block of code with a locked mutex.
 *--------------------------------------------------------------------------*/

#ifdef HAVE_LIBPTHREAD
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
 * Macros for declaring language, dialect, tag and grammar constructors.
 *
 * A language (or "language pack", more accurately) defines one or more 
 * dialects.  Each dialect is a configuration of tags that can appear in a 
 * document.  Each tag may define a grammar that denotes which tokens may
 * appear inside a tag and how they map to elements.
 *--------------------------------------------------------------------------*/

#define HEMP_LANGUAGE(f)                        \
    hemp_language f(                            \
        hemp_hemp   hemp,                       \
        hemp_string name                        \
    )

#define HEMP_DIALECT(f)                         \
    hemp_dialect f(                             \
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

#define HEMP_GRAMMAR(f)                         \
    hemp_grammar f(                             \
        hemp_hemp   hemp,                       \
        hemp_string name                        \
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
 * HEMP_SYMBOLS(name) (note plural) can be used to declare/define a 
 * function which can provide hemp with a list of symbols, e.g. all the 
 * hemp.numop.* symbols.
 *
 * HEMP_SYMBOL(name) (not singular) can be used to declare/define a 
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

#define HEMP_GLOBAL_SYMBOL(f)               \
    hemp_symbol f()

#define HEMP_SYMBOLS_ARGS                   \
    hemp_hemp        hemp,                  \
    hemp_string      name

#define HEMP_SYMBOLS_ARG_NAMES              \
    hemp, name

#define HEMP_SYMBOLS(f)                     \
    hemp_action f(                          \
        hemp_hemp    hemp,                  \
        hemp_string  name                   \
    )

#define HEMP_SYMBOL_ARGS                    \
    hemp_hemp       hemp,                   \
    hemp_symbol     symbol

#define HEMP_SYMBOL_ARG_NAMES               \
    hemp, symbol

#define HEMP_SYMBOL(f)                      \
    hemp_symbol f(                          \
        HEMP_SYMBOL_ARGS                    \
    )

//#define HEMP_SYMBOL0(name)                                      \
//    hemp_grammar_add_symbol(grammar, name, NULL, NULL, 0, 0);

#define HEMP_SYMBOL0(name)                                       \
    hemp_grammar_add_symbol(grammar, name, NULL, NULL, 0, 0);

#define HEMP_SYMBOL1(name, start)                               \
    hemp_grammar_add_symbol(grammar, name, start, NULL, 0, 0);

#define HEMP_SYMBOL2(name, start, end)                          \
    hemp_grammar_add_symbol(grammar, name, start, end,  0, 0);

#define HEMP_OPERATOR1(name, start, lprec, rprec)               \
    hemp_grammar_add_symbol(grammar, name, start, NULL, lprec, rprec);

#define HEMP_OPERATOR2(name, start, end, lprec, rprec)          \
    hemp_grammar_add_symbol(grammar, name, start, end, lprec, rprec);

#define HEMP_BLOCKOP(name, start, prec)                         \
    hemp_grammar_add_symbol(grammar, name, start, NULL, prec, prec);


/*--------------------------------------------------------------------------
 * Elements
 *
 * Right down at the very bottom of it all we have elements.  Elements are 
 * type definitions representing a parsed template.  Each element has a pointer
 * the grammar symbol that defines what kind of element it is.
 *
 * hemp_register_elements(hemp, ...) is a macro for registering a set of element types
 * (e.g. hemp.numop.*).  Hemp will automatically call the registered callback
 * when any matching elements are requested for adding to a grammar as a 
 * symbol (e.g. a grammar using hemp.numop.multiply will trigger a call to 
 * the provider registered for hemp.numop.*).  HEMP_ELEMENT(...) is used to
 * register a callback to construct an individual element type (e.g. 
 * hemp.numop.multiply).
 *--------------------------------------------------------------------------*/

#define HEMP_ELEMENT(name, constructor)     \
    hemp_register_element(                  \
        hemp, name,                         \
        (hemp_actor) constructor            \
    );


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
    hemp_template   tmpl,                   \
    hemp_tag        tag,                    \
    hemp_string     start,                  \
    hemp_pos        pos,                    \
    hemp_string    *srcptr,                 \
    hemp_symbol     symbol

#define HEMP_SCAN_ARG_NAMES                 \
    tmpl, tag, start, pos, srcptr, symbol

#define HEMP_SCAN_FUNC(f)                   \
    hemp_element f(                         \
        HEMP_SCAN_ARGS                      \
    )

/* TODO: move this into scanner.h */
hemp_scan_pos hemp_scan_pos_init(HEMP_SCAN_ARGS);
hemp_error    hemp_error_scan_pos(hemp_error, hemp_scan_pos);

#define HEMP_SCAN_ERROR(type,...)           \
    hemp_error_throw(                       \
        tmpl->dialect->hemp,                \
        hemp_error_scan_pos(                \
            hemp_error_message(             \
                tmpl->dialect->hemp,        \
                HEMP_ERROR_##type,          \
                __VA_ARGS__                 \
            ),                              \
            hemp_scan_pos_init(             \
                HEMP_SCAN_ARG_NAMES         \
            )                               \
        )                                   \
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
 * HEMP_PREFIX_FUNC() and HEMP_POSTFIX_FUNC() can be used to declare and 
 * define expression parsing functions for this purpose.  A prefix function
 * is called when an element appears at the start of an expression.  A 
 * postfix function is called when it appears following a preceding 
 * expression element.
 *--------------------------------------------------------------------------*/

#define HEMP_PREFIX_ARGS                    \
    hemp_element   *elemptr,                \
    hemp_scope      scope,                  \
    hemp_oprec      precedence,             \
    hemp_bool       force

#define HEMP_PREFIX_ARG_NAMES               \
    elemptr, scope, precedence, force

#define HEMP_PREFIX_FUNC(f)                 \
    HEMP_INLINE hemp_element f(             \
        HEMP_PREFIX_ARGS                    \
    )

#define HEMP_POSTFIX_ARGS                   \
    HEMP_PREFIX_ARGS,                       \
    hemp_element  lhs 

#define HEMP_POSTFIX_ARG_NAMES              \
    elemptr, scope, precedence, force, lhs

#define HEMP_POSTFIX_FUNC(f)                \
    HEMP_INLINE hemp_element f(             \
        HEMP_POSTFIX_ARGS                   \
    )

/* Add these for completeness, in case we ever need to make them different */

#define HEMP_INFIX_ARGS                     \
    HEMP_POSTFIX_ARGS

#define HEMP_INFIX_ARG_NAMES                \
    HEMP_POSTFIX_ARG_NAMES

#define HEMP_INFIX_FUNC(f)                  \
    HEMP_POSTFIX_FUNC(f)

#define HEMP_COMPILE_FUNC(f)                \
    HEMP_INLINE void f(                     \
        hemp_element    element,            \
        hemp_scope      scope,              \
        hemp_value    compiler            \
    )

/* operator precedence */

#define HEMP_PREC_DBG(type, tprec, lhs, prec, compare, action)              \
    hemp_debug_parse(                                                       \
        "precedence of %s (%d) is %s than %s (%d), %s\n",                   \
        type->name, tprec, compare,                                         \
        lhs->type->name, prec, action                                       \
    )

#define HEMP_PREFIX_DBG(type, prec, compare, action)                        \
    hemp_debug_parse(                                                       \
        "precedence of %s (%d) is %s than %d, %s\n",                        \
        type->name, type->rprec, compare,                                   \
        prec, action                                                        \
    )

#define HEMP_LPREC_DBG(type, lhs, prec, compare, action)                    \
    HEMP_PREC_DBG(type, type->lprec, lhs, prec, compare, action)

#define HEMP_RPREC_DBG(type, lhs, prec, compare, action)                    \
    HEMP_PREC_DBG(type, type->rprec, lhs, prec, compare, action)


// NOTE: this macro produces stupid debugging messages... can't be arsed to fix right now
#define HEMP_PREFIX_PRECEDENCE                                              \
    if (precedence && type->rprec <= precedence) {                          \
        HEMP_PREFIX_DBG(type, precedence, "not more", "returning NULL");    \
        return NULL;                                                        \
    }                                                                       \
    else {                                                                  \
        HEMP_PREFIX_DBG(type, precedence, "less", "continuing");            \
    }

#define HEMP_INFIX_LEFT_PRECEDENCE                                          \
    if (precedence && type->lprec <= precedence) {                          \
        HEMP_LPREC_DBG(type, lhs, precedence, "not more", "returning lhs"); \
        return lhs;                                                         \
    }                                                                       \
    else {                                                                  \
        HEMP_LPREC_DBG(type, lhs, precedence, "less", "continuing");        \
    }

#define HEMP_INFIX_RIGHT_PRECEDENCE                                         \
    if (precedence && type->lprec < precedence) {                           \
        HEMP_LPREC_DBG(type, lhs, precedence, "less", "returning lhs");     \
        return lhs;                                                         \
    }                                                                       \
    else {                                                                  \
        HEMP_LPREC_DBG(type, lhs, precedence, "not less", "continuing");    \
    }


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

#define HEMP_VIEW_ARGS                      \
    hemp_viewer   viewer,                 \
    hemp_element  element,                  \
    hemp_context  context,                  \
    hemp_value    output

#define HEMP_VIEW_ARG_NAMES                 \
    viewer, element, context, output

#define HEMP_VIEW_FUNC(f)                   \
    HEMP_INLINE hemp_value f(             \
        HEMP_VIEW_ARGS                      \
    )

#define HEMP_VIEWER_FUNC(f)                 \
    hemp_viewer f(                        \
        hemp_hemp   hemp,                   \
        hemp_string name                    \
    )

#define HEMP_VIEWER(name, constructor)      \
    hemp_register_viewer(                   \
        hemp, name,                         \
        (hemp_actor) constructor            \
    );

#define HEMP_VIEW(name, view)               \
    hemp_viewer_add_view(viewer, name, view);


/*--------------------------------------------------------------------------
 * Values
 *
 * Data values define vtables for coercing one type to another (e.g. number
 * to text) and other methods that can be called on values, e.g. text.length
 *
 * HEMP_VALUE_FUNC() can be used to declare and define value functions.
 * HEMP_OUTPUT_FUNC() is a special case for text yielding functions where we 
 * allow an existing text/list object to be passed as an extra argument for 
 * the function to append the value onto.
 *--------------------------------------------------------------------------*/

#define HEMP_VALUE_FUNC(f)                  \
    HEMP_INLINE hemp_value f(             \
        hemp_value    value,              \
        hemp_context    context             \
    )

#define HEMP_OUTPUT_FUNC(f)                 \
    HEMP_INLINE hemp_value f(             \
        hemp_value    value,              \
        hemp_context    context,            \
        hemp_value    output              \
    )

#define HEMP_FETCH_FUNC(f)                  \
    HEMP_INLINE hemp_value f(             \
        hemp_value    container,          \
        hemp_context    context,            \
        hemp_value    key                 \
    )

#define HEMP_STORE_FUNC(f)                  \
    HEMP_INLINE hemp_value f(             \
        hemp_value    container,          \
        hemp_context    context,            \
        hemp_value    key,                \
        hemp_value    value               \
    )

#define HEMP_OPERATE_FUNC(f)                \
    HEMP_INLINE hemp_value f(             \
        hemp_value    value,              \
        hemp_context    context,            \
        hemp_value    operand             \
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
    item->flags |= (flag)

#define hemp_clear_flag(item, flag) \
    item->flags &= ~(flag)

#define hemp_has_flag(item, flag) \
    item->flags & (flag)

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




#endif /* HEMP_MACROS_H */
