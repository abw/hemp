#ifndef HEMP_TEMPLATE_H
#define HEMP_TEMPLATE_H

#include "hemp/sources.h"
#include "hemp/elements.h"

typedef struct hemp_template * hemp_template_t;

struct hemp_template {
    hemp_source_t   source;
    hemp_elements_t elements;
};

hemp_template_t hemp_template_init(hemp_text_t, hemp_text_t);
void            hemp_template_free(hemp_template_t);

#endif /* HEMP_TEMPLATE_H */