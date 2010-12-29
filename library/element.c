#include <hemp/element.h>


/*--------------------------------------------------------------------------
 * initialisation and cleanup functions
 *--------------------------------------------------------------------------*/

hemp_element
hemp_element_new(
    hemp_string name,
    hemp_string start,
    hemp_string end
) {
    hemp_element element;
    HEMP_ALLOCATE(element);

    /* initialise the basic element details */
    element->name            = name;    // should be const?
    element->namespace       = hemp_namespace_instance(name);
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
    hemp_element element
) {
    /* only free the end token if it's not the same as the start token */
    if ( element->end 
    && ( (! element->start) || (element->start != element->end) ) ) 
        hemp_mem_free(element->end);

    if (element->start)
        hemp_mem_free(element->start);

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
    hemp_fragment fragment = *fragptr;
    hemp_string   string   = hemp_string_extract(
        fragment->token,
        fragment->token + fragment->length
    );

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
    hemp_fragment fragment = hemp_parse_prefix(
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




/*--------------------------------------------------------------------------
 * expression parsing methods
 *--------------------------------------------------------------------------*/

HEMP_PREFIX(hemp_element_parse_prefix) {
    hemp_fragment self = *fragptr;
    hemp_element  type = self->type;
    hemp_fragment expr;

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
    hemp_fragment self = *fragptr;

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
    hemp_fragment fragment = *fragptr;
    hemp_fragment rhs;

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
    hemp_fragment fragment = *fragptr;

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
    hemp_value result = hemp_obcall(value, value, context);
    return hemp_onto_text(result, context, output);
}


HEMP_VALUE(hemp_element_value_number) {
    hemp_debug_call("hemp_element_value_number()\n");
    hemp_value result = hemp_obcall(value, value, context);
    return hemp_is_numeric(result)
        ? result
        : hemp_call(result, number, context);

// forced coersion is not the way forward
//    return hemp_to_num(v);
}


HEMP_VALUE(hemp_element_value_integer) {
    hemp_debug_call("hemp_element_value_integer()\n");
    hemp_value result = hemp_obcall(value, value, context);
    return hemp_to_integer(result, context);
}


HEMP_VALUE(hemp_element_value_boolean) {
    hemp_debug_call("hemp_element_value_boolean()\n");
    hemp_value result = hemp_obcall(value, value, context);
    return hemp_to_boolean(result, context);
}


HEMP_VALUE(hemp_element_value_compare) {
    hemp_debug_call("hemp_element_value_compare()\n");
    hemp_value result = hemp_obcall(value, value, context);
    return hemp_to_compare(result, context);
}


HEMP_OUTPUT(hemp_element_value_values) {
    hemp_debug_call("hemp_element_value_values()\n");
    hemp_value result = hemp_obcall(value, value, context);
    return hemp_values(result, context, output);
}



/*--------------------------------------------------------------------------
 * debugging
 *--------------------------------------------------------------------------*/

void 
hemp_element_dump(
    hemp_element element
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
