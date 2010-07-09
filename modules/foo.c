#include <stdio.h>
#include <stdlib.h>
#include "hemp.h"

hemp_bool_t 
hemp_onload(
    hemp_t hemp
) {
    printf("hemp.foo loaded: %p\n", hemp);
    void (*hi)(void) = dlsym(RTLD_NEXT, "hemp_hello");
    if (hi) {
        printf("calling hemp_hello()\n");
        hi();
        printf("called hemp_hello()\n");
    }
    else {
        printf("cannot find hemp_hello()\n");
    }
    return HEMP_TRUE;
}

