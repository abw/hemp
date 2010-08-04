#include <hemp/templates.h>

// NOTE: may not be required after all - putting functionality straight into 
// hemp

hemp_templates_p
hemp_templates_init(
    hemp_p hemp
) {
    hemp_templates_p templates = (hemp_templates_p) hemp_mem_alloc(
        sizeof(struct hemp_templates_s)
    );

    if (! templates)
        hemp_mem_fail("templates");

    return templates;
}


void
hemp_templates_free(
    hemp_templates_p templates
) {
    hemp_mem_free(templates);
}

