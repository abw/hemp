#include <hemp/value.h>
#include <hemp/context.h>


HEMP_TYPE(hemp_type_object) {
    HempType type = hemp_type_subtype(HempTypeValue, id, name);
    return type;
};

