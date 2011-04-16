#include <hemp/element.h>

/*--------------------------------------------------------------------------
 * Type definition function.  In OO terminology, elements are essentially 
 * object classes which are subclasses of the base class HempTypeElement 
 * class, which the following function instantiates.  Except we call them
 * "types" not "classes".  The object instances are HempFragment pointers.
 * So "element" == "class" and "fragment" == "object", roughly speaking.
 *--------------------------------------------------------------------------*/

HEMP_TYPE(hemp_type_element) {
    HempType type = hemp_type_subtype(HempTypeValue, id, name);

    /* Add method(s) to element base class */
    hemp_type_extend(type, "each", &hemp_method_element_each);

    return type;
};


/*--------------------------------------------------------------------------
 * initialisation and cleanup functions
 *--------------------------------------------------------------------------*/

HempElement
hemp_element_new(
    HempString name,
    HempString start,
    HempString end
) {
    HempNamespace   namespace = hemp_namespace(name);
    HempElement     element;
    HEMP_ALLOCATE(element);

    /* The lower part of the element structure contains the same members as
     * the type structure.  This can be thought of as the "base class" part
     * of the extended element class.  We call hemp_type_xxx() functions using
     * a HempType typecast so the compiler knows that we know what we're doing.
     */

    HempType type = (HempType) element;
    hemp_type_init(type, HEMP_OBJECT_ID, name);
    hemp_type_isa(type, HempTypeElement);

    /* initialise the basic element details */
    element->namespace       = namespace;
    element->flags           = 0;
    element->lprec           = 0;
    element->rprec           = 0;
    element->grammar         = NULL;
    element->scanner         = NULL;
    element->cleanup         = NULL;

    /* different element types override these evaluation functions */
    element->text            = &hemp_element_not_text;
    element->value           = &hemp_element_not_value;
    element->number          = &hemp_element_not_number;
    element->integer         = &hemp_element_not_integer;
    element->boolean         = &hemp_element_not_boolean;
    element->compare         = &hemp_element_not_compare;
    element->assign          = &hemp_element_not_assign;

    /* some additional evaluation method specific to element fragments */
    element->token           = &hemp_element_not_token;
    element->source          = &hemp_element_not_source;

    /* it's safe to use these defaults as they all call element->value */
    element->values          = &hemp_value_values;
    element->pairs           = &hemp_value_pairs;
    element->params          = &hemp_value_params;
    element->apply           = &hemp_value_apply;

    /* different element types override these parsing function */
    element->parse_prefix    = NULL;
    element->parse_postfix   = NULL;
    element->parse_branch    = NULL;
    element->parse_fixed     = NULL;
    element->parse_params    = NULL;
    element->parse_pair      = NULL;
    element->parse_lvalue    = NULL; //&hemp_element_not_lvalue;
    element->parse_proto     = &hemp_element_not_proto;
    element->parse_body      = &hemp_element_parse_body;     // default to NULL?

    /* clone the start token if there is one, and the end token if there 
     * is one and it's not the same as the start token
     */
    if (start) 
        start = hemp_string_clone(start, "element start token");

    if (end)
        end = (start && (start == end || hemp_string_eq(start, end)))
            ? start
            : hemp_string_clone(end, "element end token");
        
    element->start   = start;
    element->end     = end;

    return element;
}


void
hemp_element_free(
    HempElement element
) {
    /* only free the end token if it's not the same as the start token */
    if ( element->end 
    && ( (! element->start) || (element->start != element->end) ) ) 
        hemp_mem_free(element->end);

    /* start token is optional, so this may be NULL */
    if (element->start)
        hemp_mem_free(element->start);

    /* cleanup any memory allocated for the type "base class" */
    hemp_type_wipe((HempType) element);

    /* free the element data structure and we're done! */
    hemp_mem_free(element);
}



/*--------------------------------------------------------------------------
 * Default functions which raise an error when a fragment is used in a 
 * parsing context not supported by the element type.
 *--------------------------------------------------------------------------*/

HEMP_FIXUP(hemp_element_not_proto) {
    hemp_fatal(
        "%s element cannot be a function prototype\n", 
        fragment->type->name
    );
    return NULL;
}



/*--------------------------------------------------------------------------
 * Default functions which raise an error when a fragment is used in a 
 * value context which is not supported by the element type.
 *--------------------------------------------------------------------------*/

HEMP_OUTPUT(hemp_element_not_text) {
    hemp_fatal(
        "%s element does not yield text\n", 
        hemp_val_frag(value)->type->name);
    return output;
}


HEMP_VALUE(hemp_element_not_value) {
    hemp_fatal(
        "%s element does not yield value\n", 
        hemp_val_frag(value)->type->name
    );
    return HempNothing;
}


HEMP_VALUE(hemp_element_not_number) {
    hemp_fatal(
        "%s element does not yield number\n", 
        hemp_val_frag(value)->type->name
    );
    return HempNothing;
}


HEMP_VALUE(hemp_element_not_integer) {
    hemp_fatal(
        "%s element does not yield integer\n", 
        hemp_val_frag(value)->type->name
    );
    return HempNothing;
}


HEMP_VALUE(hemp_element_not_boolean) {
    hemp_fatal(
        "%s element does not yield boolean\n", 
        hemp_val_frag(value)->type->name
    );
    return HempNothing;
}


HEMP_VALUE(hemp_element_not_compare) {
    hemp_fatal(
        "%s element does not yield comparison\n", 
        hemp_val_frag(value)->type->name
    );
    return HempNothing;
}


HEMP_INPUT(hemp_element_not_assign) {
    hemp_debug_msg("Throwing not_assign error....\n");
    hemp_fatal(
        "%s element cannot be assigned to\n", 
        hemp_val_frag(value)->type->name
    );
    return HempNothing;
}


HEMP_OUTPUT(hemp_element_not_token) {
    hemp_fatal(
        "%s element does not yield token\n", 
        hemp_val_frag(value)->type->name
    );
    return output;
}


HEMP_OUTPUT(hemp_element_not_source) {
    hemp_fatal(
        "%s element does not yield source\n", 
        hemp_val_frag(value)->type->name);
    return output;
}



/*--------------------------------------------------------------------------
 * generic element parsing functions
 *--------------------------------------------------------------------------*/


HEMP_PREFIX(hemp_element_parse_fixed) {
    hemp_debug_call("hemp_element_parse_fixed()\n");
    HempFragment fragment = *fragptr;

    /* temporary hack to catch cases where parser backtracks and this function
     * gets called multiple times for an element.
     */
    if (hemp_has_flag(fragment, HEMP_BE_ALLOCATED)) {
        hemp_advance(fragptr);
        return fragment;
    }

    HempString   string   = hemp_string_extract(
        fragment->token,
        fragment->token + fragment->length
    );
//  hemp_debug_msg("hemp_element_parse_fixed(%p:%s) string extract: %p\n", fragment, fragment->type->name, string);

//  hemp_debug_msg("extracted word token string: %s\n", string);
    hemp_set_flag(fragment, HEMP_BE_FIXED | HEMP_BE_ALLOCATED);
    hemp_set_expr(fragment, hemp_str_val(string));
    hemp_advance(fragptr);

    return fragment;
}


HEMP_PREFIX(hemp_element_parse_body) {
    hemp_debug_call("hemp_element_parse_body(%s)\n", (*fragptr)->type->name);

    /* we don't need to skip whitespace here as any skippable whitespace 
     * elements will forward the parse_prefix call onto the next token, but
     * it's slightly more efficient this way.
     */
    hemp_skip_whitespace(fragptr);

    /* See comments for parse_body() in Template::TT3::Element.pm
     * It forces precedence to CMD_PRECEDENCE and FORCE to 1.  I don't 
     * think we need to do this if we just use the values passed as args.
     */
    HempFragment fragment = hemp_parse_prefix(
        fragptr, scope, precedence, force
    );

    /* A single expression body is fully terminated, so the caller should
     * not look for a corresponding terminator token, e.g. sub inc(a) a + 1
     * has a single expression body, a + 1, so it doesn't require an 'end',
     * unlike: sub inc(a); a + 1; end, which invokes parse_body on the ';'
     * terminator, returning an unterminated block
     */
    if (fragment)
        hemp_set_flag(fragment, HEMP_BE_TERMINATED);

    return fragment;
}


HEMP_PREFIX(hemp_element_parse_body_block) {
    hemp_debug_call("hemp_element_parse_body_block(%s)\n", (*fragptr)->type->name);
    return hemp_parse_block(fragptr, scope, precedence, force);
}


HEMP_PREFIX(hemp_element_parse_prefix_pair) {
    hemp_debug_call("hemp_element_parse_expr_pair(%s)\n", (*fragptr)->type->name);

    /* Default function for element that may start an expression that yields
     * one or more pairs.  For example, a word may be the LHS of an assignment
     * expression, e.g. [% foo = 10 %].  The word element maps parse_pair() 
     * to this function which calls the parse_prefix() function to parse a 
     * complete expression.  It then checks that the expression has the 
     * HEMP_BE_PAIRS flag set, returning it if it does, or rewinding to the 
     * start of the expression and returning NULL if not.
     */

    HempFragment fragment = *fragptr;
    HempFragment expr     = hemp_parse_prefix(fragptr, scope, precedence, force);

    if (expr && hemp_not_flag(expr, HEMP_BE_PAIRS)) {
        *fragptr = fragment;
        hemp_debug_parse("expression does not yield pairs: %s\n", expr->type->name);
        hemp_debug_parse("rewound to: %s\n", (*fragptr)->type->name);
        expr = NULL;
    }

    return expr;
}



/*--------------------------------------------------------------------------
 * expression parsing methods
 *--------------------------------------------------------------------------*/

HEMP_PREFIX(hemp_element_parse_prefix) {
    HempFragment self = *fragptr;
    HempElement  type = self->type;
    HempFragment expr;

    hemp_debug_call("hemp_element_parse_prefix()\n");

    hemp_set_flag(self, HEMP_BE_PREFIX);
    hemp_advance(fragptr);

    expr = hemp_parse_prefix(fragptr, scope, type->rprec, 1);

    if (! expr)
        hemp_fatal("missing expression on rhs of %s\n", type->start);
    
    hemp_set_expr_fragment(self, expr);

    return hemp_parse_postfix(
        fragptr, scope, precedence, 0,
        self
    );
}


HEMP_POSTFIX(hemp_element_parse_postfix) {
    hemp_debug_call("hemp_element_parse_postfix()\n");
    HempFragment self = *fragptr;

    HEMP_INFIX_LEFT_PRECEDENCE;

    hemp_set_flag(self, HEMP_BE_POSTFIX);
    hemp_set_expr_fragment(self, lhs);
    hemp_advance(fragptr);

    return hemp_parse_postfix(
        fragptr, scope, precedence, 0,
        self
    );
}


HEMP_POSTFIX(hemp_element_parse_infix_left) {
    hemp_debug_call("hemp_element_parse_infix_left()\n");
    HempFragment fragment = *fragptr;
    HempFragment rhs;

    HEMP_INFIX_LEFT_PRECEDENCE;

    hemp_set_flag(fragment, HEMP_BE_INFIX);
    hemp_set_lhs_fragment(fragment, lhs);
    hemp_advance(fragptr);

    rhs = hemp_parse_rhs_expr_lprec(fragment);

    hemp_debug_parse(
        "parsed infix [%s] [%s] [%s]\n", 
        lhs->type->name, fragment->type->start, rhs->type->name
    );

    hemp_debug_parse("next fragment is %s:\n", (*fragptr)->type->name);

    // TODO: convince myself that we shouldn't be passing self->rprec
    // instead of precedence
    return hemp_parse_postfix(
        fragptr, scope, precedence, 0,
        fragment
    );
}


HEMP_POSTFIX(hemp_element_parse_infix_right) {
    HempFragment fragment = *fragptr;

    hemp_debug_call("hemp_element_parse_infix_right()\n");

    HEMP_INFIX_RIGHT_PRECEDENCE;

    hemp_set_flag(fragment, HEMP_BE_INFIX);
    hemp_set_lhs_fragment(fragment, lhs);
    hemp_advance(fragptr);
    hemp_parse_rhs_expr_lprec(fragment);

    return hemp_parse_postfix(
        fragptr, scope, precedence, 0,
        fragment
    );
}


/*--------------------------------------------------------------------------
 * evaluation and output functions
 *
 * Elements can provide custom functions for evaluation as a text fragment,
 * number, integer, boolean truth, or tri-state comparison (before, equal,
 * after) value.  In the usual case they "inherit" the default "methods" 
 * below that call the element's generic value() method and then coerce the 
 * returned value to the appropriate type.  That way an element is only 
 * required to implement a single value() method and everything else Just
 * Works[tm].
 *
 * NOTE: don't be tempted to write something like this:
 *   return hemp_to_XXX( element->type->value(element, context) );
 *
 * hemp_to_XXX are macros and the above will cause the element to be 
 * evaluated multiple times.
 *--------------------------------------------------------------------------*/

HEMP_VALUE(hemp_element_value) {
    hemp_debug_call("hemp_element_value()\n");
    return hemp_obcall(value, value, context);
}


HEMP_OUTPUT(hemp_element_value_text) {
    hemp_debug_call("hemp_element_value_text()\n");
    HempValue result = hemp_obcall(value, value, context);
    return hemp_onto_text(result, context, output);
}


HEMP_VALUE(hemp_element_value_number) {
    hemp_debug_call("hemp_element_value_number()\n");
    HempValue result = hemp_obcall(value, value, context);
    return hemp_is_numeric(result)
        ? result
        : hemp_call(result, number, context);

// forced coersion is not the way forward
//    return hemp_to_num(v);
}


HEMP_VALUE(hemp_element_value_integer) {
    hemp_debug_call("hemp_element_value_integer()\n");
    HempValue result = hemp_obcall(value, value, context);
    return hemp_to_integer(result, context);
}


HEMP_VALUE(hemp_element_value_boolean) {
    hemp_debug_call("hemp_element_value_boolean()\n");
    HempValue result = hemp_obcall(value, value, context);
    return hemp_to_boolean(result, context);
}


HEMP_VALUE(hemp_element_value_compare) {
    hemp_debug_call("hemp_element_value_compare()\n");
    HempValue result = hemp_obcall(value, value, context);
    return hemp_to_compare(result, context);
}


HEMP_OUTPUT(hemp_element_value_values) {
    hemp_debug_call("hemp_element_value_values()\n");
    HempValue result = hemp_obcall(value, value, context);
    return hemp_values(result, context, output);
}



/*--------------------------------------------------------------------------
 * Methods
 *--------------------------------------------------------------------------*/

HEMP_VALUE(hemp_method_element_each) {
    hemp_debug_call("hemp_method_element_each()\n");
    HempValue result = hemp_obcall(value, value, context);
//  hemp_debug_msg("element value: %s->each\n", hemp_type_name(result));
    return hemp_send(result, "each", context);
}


/*--------------------------------------------------------------------------
 * debugging
 *--------------------------------------------------------------------------*/

void 
hemp_element_dump(
    HempElement element
) {
    hemp_debug("element at %p\n", element->name, element);
    hemp_debug("       name: %s\n", element->name);
    hemp_debug("      start: %s\n", element->start ? element->start : "<none>");
    hemp_debug("        end: %s\n", element->end ? element->end : "<none>");
    hemp_debug("      flags: %04x\n", element->flags);
    hemp_debug("      lprec: %d\n", element->lprec);
    hemp_debug("      rprec: %d\n", element->rprec);
    hemp_debug("    scanner: %p\n", element->scanner);
    hemp_debug("    cleanup: %p\n", element->cleanup);
    hemp_debug("     prefix: %p\n", element->parse_prefix);
    hemp_debug("    postfix: %p\n", element->parse_postfix);
    hemp_debug("      token: %p\n", element->token);
    hemp_debug("     source: %p\n", element->source);
    hemp_debug("       text: %p\n", element->text);
    hemp_debug("      value: %p\n", element->value);
    hemp_debug("     number: %p\n", element->number);
    hemp_debug("    integer: %p\n", element->integer);
    hemp_debug("    boolean: %p\n", element->boolean);
}
