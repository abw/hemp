#include "hemp/element.h"
#include "hemp/debug.h"


hemp_element_t
hemp_element_init() {
    hemp_element_t element;

    if ((element = (hemp_element_t) hemp_mem_init(sizeof(struct hemp_element)))) {
        debug_mem("Allocated new element at %p\n", element);
    }
    
    return element;
}


void
hemp_element_free(hemp_element_t element) {
    if (element) {
        debug_mem("Releasing element at %p\n", element);
        hemp_mem_free(element);
    }
}
