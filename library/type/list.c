#include <hemp/list.h>
#include <hemp/value.h>
#include <hemp/context.h>


HEMP_TYPE_FUNC(hemp_type_list) {
    hemp_type_p type = hemp_type_subtype(HempValue, id, name);
    type->text       = &hemp_value_list_text;       /* return/append text   */
    type->number     = &hemp_value_not_number;      /* list is not a number */
    type->integer    = &hemp_value_not_integer;     /* or an integer        */
    type->compare    = &hemp_value_not_compare;     /* it can't compare     */
    type->boolean    = &hemp_value_true;            /* but is always true   */  /* or list size? */
    type->defined    = &hemp_value_true;            /* and always defined   */

    hemp_type_extend(type, "length", &hemp_method_list_length);

    return type;
};


/*--------------------------------------------------------------------------
 * List pseudo-object methods
 *--------------------------------------------------------------------------*/

HEMP_VALUE_FUNC(hemp_method_list_length) {
    return hemp_int_val( hemp_val_list(value)->length );
}


