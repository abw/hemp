#include <hemp/value.h>
#include <hemp/context.h>


HEMP_TYPE_FUNC(hemp_type_object) {
    HempType type = hemp_type_subtype(HempTypeValue, id, name);
    return type;
};

