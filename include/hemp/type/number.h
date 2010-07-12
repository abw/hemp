#ifndef HEMP_TYPE_NUMBER_H
#define HEMP_TYPE_NUMBER_H

#include <string.h>
#include <stdlib.h>
#include "hemp/type.h"

struct hemp_type_number {
    HEMP_TYPE_BASE
    hemp_truth_fn   truth;
    hemp_unary_fn   integer;
    hemp_unary_fn   floater;
    hemp_unary_fn   positive;
    hemp_unary_fn   negative;
    hemp_unary_fn   absolute;
    hemp_binary_fn  add;
    hemp_binary_fn  subtract;
    hemp_binary_fn  multiply;
    hemp_binary_fn  divide;
    hemp_binary_fn  divint;
    hemp_binary_fn  divmod;
    hemp_ternary_fn power;
};

#endif /* HEMP_TYPE_NUMBER_H */

