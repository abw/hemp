#include <hemp/value.h>
#include <hemp/context.h>


HEMP_TYPE_FUNC(hemp_type_object) {
    hemp_type type = hemp_type_subtype(HempValue, id, name);
    return type;
};

