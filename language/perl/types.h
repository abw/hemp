#include <hemp.h>

extern hemp_type_p HempPerlScalar;
extern hemp_type_p HempPerlArray;
extern hemp_type_p HempPerlHash;
extern hemp_type_p HempPerlObject;

void            hemp_perl_init();
hemp_value_t    hemp_perl_value(SV *);


HEMP_TYPE_FUNC(hemp_type_perl_scalar);
HEMP_VTEXT_FUNC(hemp_perl_scalar_text);
HEMP_VALUE_FUNC(hemp_perl_scalar_number);
HEMP_VALUE_FUNC(hemp_perl_scalar_integer);
HEMP_VALUE_FUNC(hemp_perl_scalar_boolean);
HEMP_VALUE_FUNC(hemp_perl_scalar_defined);
HEMP_VALUE_FUNC(hemp_perl_scalar_length);

HEMP_TYPE_FUNC(hemp_type_perl_array);
HEMP_VALUE_FUNC(hemp_perl_array_boolean);
HEMP_VALUE_FUNC(hemp_perl_array_length);

HEMP_TYPE_FUNC(hemp_type_perl_hash);
HEMP_VALUE_FUNC(hemp_perl_hash_boolean);
HEMP_VALUE_FUNC(hemp_perl_hash_length);

HEMP_TYPE_FUNC(hemp_type_perl_object);

HEMP_VALUE_FUNC(hemp_perl_true);


#define hemp_perl_val_sv(value) \
    ( (SV *) HEMP_POINTER(value) )

#define hemp_perl_val_av(value) \
    ( (AV *) HEMP_POINTER(value) )

#define hemp_perl_val_hv(value) \
    ( (HV *) HEMP_POINTER(value) )

#define hemp_perl_sv_val(sv) \
    ( hemp_type_val(HempPerlScalar, sv) )

#define hemp_perl_av_val(av) \
    ( hemp_type_val(HempPerlArray, av) )

#define hemp_perl_hv_val(hv) \
    ( hemp_type_val(HempPerlHash, hv) )

#define hemp_perl_obj_val(sv) \
    ( hemp_type_val(HempPerlObject, sv) )

