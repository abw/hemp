#include <hemp/value.h>
#include <hemp/context.h>
#include <hemp/type/code.h>


HEMP_TYPE_FUNC(hemp_type_code) {
    hemp_type_p type = hemp_type_subtype(HempValue, id, name);
//    type->text       = &hemp_type_code_text;       /* return/append text   */
//    type->number     = &hemp_type_code_number;      /* text -> number       */
//    type->integer    = &hemp_type_code_integer;     /* text -> integer      */
//    type->boolean    = &hemp_type_code_boolean;     /* text -> boolean      */
//    type->compare    = &hemp_value_not_compare;     /* can't compare        */
//    type->defined    = &hemp_valuerue;            /* always defined       */

//    hemp_type_extend(type, "length", &hemp_method_text_length);

    return type;
};



/*--------------------------------------------------------------------------
 * constructor/destructor functions.
 *--------------------------------------------------------------------------*/

HEMP_INLINE hemp_code
hemp_code_init(
    hemp_code code
) {
    hemp_debug_msg("hemp_code_init()\n");
    HEMP_INSTANCE(code);
    return code;
}


HEMP_INLINE void
hemp_code_release(
    hemp_code code
) {
    hemp_debug_msg("hemp_code_release()\n");
}


HEMP_INLINE void
hemp_code_free(
    hemp_code code
) {
    hemp_debug_msg("hemp_code_free(%p)\n", code);
    hemp_code_release(code);
    hemp_mem_free(code);
}

