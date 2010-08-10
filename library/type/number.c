#include "hemp/type/number.h"


// NO LONGER USED


static struct hemp_type_number
    hemp_type_integer = {
        "hemp.type.integer",
        NULL,       // base
        1,          // size
        NULL,       // pool
        &hemp_integer_init,
        &hemp_integer_free,
        &hemp_integer_text,
        &hemp_integer_truth,
        &hemp_integer_integer,
        &hemp_integer_float;
        &hemp_integer_positive;
        &hemp_integer_negative;
        &hemp_integer_absolute;
        &hemp_integer_add;
        &hemp_integer_subtract;
        &hemp_integer_multiply;
        &hemp_integer_divide;
        &hemp_integer_divint;
        &hemp_integer_divmod;
        &hemp_integer_power;
    };


hemp_type_t HempTypeInteger = (hemp_type_t) &hemp_type_integer;


void 
hemp_scan_inline_tag(
