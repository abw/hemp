#include "EXTERN.h"
#include "perl.h"
#include "XSUB.h"
#include "ppport.h"
#include <hemp.h>
#include "types.h"

hemp_type_p HempPerlScalar  = NULL;
hemp_type_p HempPerlArray   = NULL;
hemp_type_p HempPerlHash    = NULL;
hemp_type_p HempPerlObject  = NULL;


/*--------------------------------------------------------------------------
 * Initialisation and type registration
 *--------------------------------------------------------------------------*/

void 
hemp_perl_init() {
    hemp_global_types_init();
    HempPerlScalar = hemp_use_type("PerlScalar", &hemp_type_perl_scalar);
    HempPerlArray  = hemp_use_type("PerlArray",  &hemp_type_perl_array);
    HempPerlHash   = hemp_use_type("PerlHash",   &hemp_type_perl_hash);
    HempPerlObject = hemp_use_type("PerlObject", &hemp_type_perl_object);
}


hemp_value_t
hemp_perl_value(
    SV * sv
) {
    if (! SvOK(sv)) {
//      hemp_debug("SV is undefined\n");
        return HempNothing;
    }
        
    if (SvROK(sv)) {
//      hemp_debug("SV is a reference\n");

        if (sv_isobject(sv)) {
//          hemp_debug("SV is an object\n");                /* OBJECT */
            return hemp_perl_obj_val(sv);
        }
        else {
            switch ( SvTYPE( SvRV(sv) ) ) {
                case SVt_PVHV:                              /* HASH */
//                  hemp_debug("SV is a hash reference\n");
                    return hemp_perl_hv_val( SvRV(sv) );

                case SVt_PVAV:                              /* ARRAY */
//                  hemp_debug("SV is an array reference\n");
                    return hemp_perl_av_val( SvRV(sv) );

                default:
                    hemp_fatal("Can't determine type of Perl reference");
            }
        }
    }
    else {
//      hemp_debug("SV is not a reference\n");
        return hemp_perl_sv_val(sv);
    }
}


/*--------------------------------------------------------------------------
 * Type constructor and methods for Perl scalar value
 *--------------------------------------------------------------------------*/

HEMP_TYPE_FUNC(hemp_type_perl_scalar) {
    hemp_type_p type = hemp_type_subtype(HempValue, id, name);
    type->text    = &hemp_perl_scalar_text;      /* perl -> text         */
    type->number  = &hemp_perl_scalar_number;    /* perl -> number       */
    type->integer = &hemp_perl_scalar_integer;   /* perl -> integer      */
    type->boolean = &hemp_perl_scalar_boolean;   /* perl -> boolean      */
    type->defined = &hemp_perl_scalar_defined;   /* perl -> defined      */

    hemp_type_extend(type, "length", &hemp_perl_scalar_length);

    return type;
};


HEMP_VTEXT_FUNC(hemp_perl_scalar_text) {
    hemp_debug_call("hemp_perl_scalar_text()\n");
    hemp_text_p text;
    STRLEN length;
    char *string = SvPV( hemp_perl_val_sv(value), length );

    hemp_prepare_output(output, text, length);
    hemp_text_append_stringn(text, string, length);
    return output;
}


HEMP_VALUE_FUNC(hemp_perl_scalar_number) {
    hemp_debug_call("hemp_perl_scalar_number()\n");
    // TODO: should use looks_like_number first and throw error if not
    return hemp_num_val( 
        SvNV( hemp_perl_val_sv(value) )
    );
}


HEMP_VALUE_FUNC(hemp_perl_scalar_integer) {
    hemp_debug_call("hemp_perl_scalar_integer()\n");
    // TODO: should use looks_like_number first and throw error if not
    return hemp_int_val( 
        SvIV( hemp_perl_val_sv(value) )
    );
}


HEMP_VALUE_FUNC(hemp_perl_scalar_boolean) {
    hemp_debug_call("hemp_perl_scalar_boolean()\n");
    return SvTRUE( hemp_perl_val_sv(value) )
        ? HempTrue
        : HempFalse;
}


HEMP_VALUE_FUNC(hemp_perl_scalar_defined) {
    hemp_debug_call("hemp_perl_scalar_defined()");
    return SvOK( hemp_perl_val_sv(value) )
        ? HempTrue
        : HempFalse;
}


HEMP_VALUE_FUNC(hemp_perl_scalar_length) {
    hemp_debug_call("hemp_perl_scalar_length()\n");
    return hemp_int_val(
        SvCUR( hemp_perl_val_sv(value) )
    );
}


/*--------------------------------------------------------------------------
 * Type constructor and methods for Perl array value
 *--------------------------------------------------------------------------*/

HEMP_TYPE_FUNC(hemp_type_perl_array) {
    hemp_type_p type = hemp_type_subtype(HempValue, id, name);
    type->text    = &hemp_perl_scalar_text;      /* perl -> text         */
    type->boolean = &hemp_perl_array_boolean;    /* false if empty       */
    type->defined = &hemp_perl_true;             /* implicitly defined   */

    hemp_type_extend(type, "length", &hemp_perl_array_length);

    return type;
};


HEMP_VALUE_FUNC(hemp_perl_array_boolean) {
    hemp_debug_call("hemp_perl_array_boolean()\n");
    return (av_len( hemp_perl_val_av(value) ) >= 0)
        ? HempTrue
        : HempFalse;
}


HEMP_VALUE_FUNC(hemp_perl_array_length) {
    hemp_debug_call("hemp_perl_array_length()\n");
    return hemp_int_val(
        av_len( hemp_perl_val_av(value) ) + 1
    );
}


/*--------------------------------------------------------------------------
 * Type constructor and methods for Perl hash value
 *--------------------------------------------------------------------------*/

HEMP_TYPE_FUNC(hemp_type_perl_hash) {
    hemp_type_p type = hemp_type_subtype(HempValue, id, name);
    type->text    = &hemp_perl_scalar_text;      /* perl -> text         */
    type->boolean = &hemp_perl_hash_boolean;     /* false if empty       */
    type->defined = &hemp_perl_true;             /* implicitly defined   */

    hemp_type_extend(type, "length", &hemp_perl_hash_length);

    return type;
};


HEMP_VALUE_FUNC(hemp_perl_hash_boolean) {
    hemp_debug_call("hemp_perl_hash_boolean()\n");
    return (HvKEYS( hemp_perl_val_av(value) ) > 0)
        ? HempTrue
        : HempFalse;
}


HEMP_VALUE_FUNC(hemp_perl_hash_length) {
    hemp_debug_call("hemp_perl_hash_length()\n");
    return hemp_int_val(
        HvKEYS( hemp_perl_val_av(value) )
    );
}


/*--------------------------------------------------------------------------
 * Type constructor and methods for Perl objects
 *--------------------------------------------------------------------------*/

HEMP_TYPE_FUNC(hemp_type_perl_object) {
    hemp_type_p type = hemp_type_subtype(HempValue, id, name);
    type->text    = &hemp_perl_scalar_text;      /* perl -> text         */
    type->boolean = &hemp_perl_true;             /* implicitly true      */
    type->defined = &hemp_perl_true;             /* implicitly defined   */
    return type;
};


/*--------------------------------------------------------------------------
 * Miscellaneous Perl value function
 *--------------------------------------------------------------------------*/

HEMP_VALUE_FUNC(hemp_perl_true) {
    return HempTrue;
}



