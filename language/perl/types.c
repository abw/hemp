#include "EXTERN.h"
#include "perl.h"
#include "XSUB.h"
#include "ppport.h"
#include <hemp.h>
#include "types.h"

hemp_type_p HempPerlScalar  = NULL;
hemp_type_p HempPerlArray   = NULL;
hemp_type_p HempPerlHash    = NULL;
hemp_type_p HempPerlCode    = NULL;
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
    HempPerlCode   = hemp_use_type("PerlCode",   &hemp_type_perl_code);
    HempPerlObject = hemp_use_type("PerlObject", &hemp_type_perl_object);
}


HEMP_INLINE hemp_value_t
hemp_perl_value(
    SV * sv
) {
    if (! SvOK(sv)) {
        hemp_debug_perl("SV is undefined\n");               /* undef        */
        return HempNothing;
    }
        
    if (SvROK(sv)) {
        hemp_debug_perl("SV is a reference\n");             /* reference    */

        if (sv_isobject(sv)) {
            hemp_debug_perl("SV is an object\n");           /* object       */
            return hemp_perl_obj_val(sv);
        }
        else {
            switch ( SvTYPE( SvRV(sv) ) ) {
                case SVt_PVHV:                              /* hash ref     */
                    hemp_debug_perl("SV is a hash reference\n");
                    return hemp_perl_hv_val( SvRV(sv) );

                case SVt_PVAV:                              /* array ref    */
                    hemp_debug_perl("SV is an array reference\n");
                    return hemp_perl_av_val( SvRV(sv) );

                case SVt_PVCV:
                    hemp_debug_perl("SV is a CODE reference\n");
                    return hemp_perl_cv_val( SvRV(sv) );

                default:
                    hemp_fatal("Can't determine type of Perl reference");
            }
        }
    }
    else {
        hemp_debug_perl("SV is not a reference\n");         /* scalar value */
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


HEMP_OUTPUT_FUNC(hemp_perl_scalar_text) {
    hemp_debug_call("hemp_perl_scalar_text()\n");

    hemp_str_p  string;
    hemp_size_t length;
    hemp_text_p text;

    string = SvPV( hemp_perl_val_sv(value), length );
    hemp_prepare_text_size(context, output, text, length);
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
    type->fetch   = &hemp_perl_array_fetch;

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


HEMP_FETCH_FUNC(hemp_perl_array_fetch) {
    hemp_debug_perl("hemp_perl_array_fetch()\n");

    hemp_uint_t index;
    hemp_bool_t found = HEMP_FALSE;
    SV **       result;

    if (hemp_is_integer(key)) {
        hemp_debug_perl("got integer key\n");
        index = hemp_val_int(key);
        found = HEMP_TRUE;
    }
    else {
        hemp_text_p ktext;
        hemp_bool_t kmine  = HEMP_FALSE;

        /* TODO: this has been cut-n-pasted... needs moving out */
        if (hemp_is_text(key)) {
            hemp_debug_perl("got text key\n");
            ktext = hemp_val_text(key);
        }
        else {
            /* otherwise we have to convert the key to text */
            /* TODO: must be a better way to check for numeric conversion without throwing an error? */
            hemp_debug_perl("creating text key\n");
            ktext = hemp_text_new_size(16);
            kmine = HEMP_TRUE;
            hemp_onto_text(key, context, hemp_text_val(ktext));
        }
        hemp_debug_perl("list text key: %s\n", ktext->string);

        if (hemp_string_numlike(ktext->string)) {
            hemp_debug_perl("got numlike string\n");
            index = hemp_val_num( 
                hemp_type_string_number( hemp_str_val(ktext->string), context) 
            );
            found = HEMP_TRUE;
        }
        
        if (kmine)
            hemp_text_free(ktext);
    }
    
    if (! found) {
        HEMP_FETCH_ERROR(
            context, 
            hemp_type_name(container),
            HEMP_STR_NOTHING
        );
    }

    hemp_debug_perl("got index key: %d\n", index);

    result = av_fetch( hemp_perl_val_av(container), index, 0 );

    hemp_debug_perl(
        "%s.%u => %s\n", hemp_type_name(container), index, 
        result ? SvPV_nolen(*result) : HEMP_STR_MISSING
    );
    
    return result
        ? hemp_perl_value(*result)
        : HempMissing;
}



/*--------------------------------------------------------------------------
 * Type constructor and methods for Perl hash value
 *--------------------------------------------------------------------------*/

HEMP_TYPE_FUNC(hemp_type_perl_hash) {
    hemp_type_p type = hemp_type_subtype(HempValue, id, name);
    type->text    = &hemp_perl_scalar_text;      /* perl -> text         */
    type->boolean = &hemp_perl_hash_boolean;     /* false if empty       */
    type->defined = &hemp_perl_true;             /* implicitly defined   */
    type->fetch   = &hemp_perl_hash_fetch;

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


HEMP_FETCH_FUNC(hemp_perl_hash_fetch) {
    hemp_bool_t kmine  = HEMP_FALSE;
    hemp_text_p ktext;
    SV **       result;

    if (hemp_is_text(key)) {
        ktext = hemp_val_text(key);
    }
    else {
        /* otherwise we have to convert the key to text */
        ktext = hemp_text_new_size(16);
        kmine = HEMP_TRUE;
        hemp_onto_text(key, context, hemp_text_val(ktext));
    }

    hemp_debug_perl(
        "hemp_perl_hash_fetch(%s, %s)\n", 
        hemp_type_name(container), 
        ktext->string
    );

    result = hv_fetch( 
        hemp_perl_val_hv(container), 
        ktext->string, 
        ktext->length,
        0
    );

    hemp_debug_perl(
        "%s.%s => %s\n", hemp_type_name(container), ktext->string, 
        result ? SvPV_nolen(*result) : HEMP_STR_MISSING
    );
    
    if (kmine)
        hemp_text_free(ktext);

    return result
        ? hemp_perl_value(*result)
        : HempMissing;
}

/*--------------------------------------------------------------------------
 * Type constructor and methods for Perl code (subroutine references)
 *--------------------------------------------------------------------------*/

HEMP_TYPE_FUNC(hemp_type_perl_code) {
    hemp_type_p type = hemp_type_subtype(HempValue, id, name);
    type->apply   = &hemp_perl_code_apply;          /* call a function      */
    type->text    = &hemp_perl_code_text;           /* perl -> text         */
    type->boolean = &hemp_perl_true;                /* implicitly true      */
    type->defined = &hemp_perl_true;                /* implicitly defined   */
    return type;
};


HEMP_OUTPUT_FUNC(hemp_perl_code_text) {
    hemp_debug_call("hemp_perl_code_text()\n");
    hemp_debug("hemp_perl_code_text() calling hemp_perl_code_apply()\n");
    hemp_value_t result = hemp_perl_code_apply(value, context);
    hemp_debug("got %s result: \n", hemp_type_name(result)); hemp_dump_value(result);
    return hemp_text(result, context, output);
}


HEMP_VALUE_FUNC(hemp_perl_code_apply) {
    hemp_debug("TODO: hemp_perl_code_apply()");
    SV *sv = hemp_perl_val_sv(value);
    I32 nret;

    hemp_debug("calling code ref: %p\n", sv);

    ENTER;
    SAVETMPS;
    nret = call_sv(sv, G_SCALAR | G_DISCARD | G_NOARGS);
    FREETMPS;
    LEAVE;

    return HempTrue;
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



