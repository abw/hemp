#include <hemp/type.h>


hemp_type_p 
hemp_type_init(
    hemp_cstr_p name
) {
    hemp_type_p type = (hemp_type_p) hemp_mem_alloc(
        sizeof(struct hemp_type_s)
    );

    if (! type)
        hemp_mem_fail("type");

    type->name = hemp_cstr_copy(name);

    return type;
}


void 
hemp_type_free(
    hemp_type_p type
) {
    hemp_mem_free(type->name);
    hemp_mem_free(type);
}

