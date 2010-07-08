#include <stdio.h>
#include "hemp.h"

void test_hemp();

int
main(int argc, char **argv, char **env)
{
    test_hemp();
    return 0;
}

void test_hemp() {
    hemp_t hemp = hemp_init();
    hemp
        ? pass("Created hemp object: %p", hemp)
        : fail("Failed to create hemp object");
    hemp_free(hemp);
}
