#include <hemp/value.h>
#include <hemp/context.h>
//#include <hemp/type/object.h>


HEMP_TYPE_FUNC(hemp_type_object) {
    hemp_type_p type = hemp_type_subtype(HempValue, id, name);
    return type;
};

