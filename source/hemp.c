#include <stdio.h>
#include "config.h"
#include "hemp.h"
#include "hemp/debug.h"


int
main(int argc, char **argv, char **env)
{
    hemp_t hemp = hemp_init();
    printf("hemp >\n");
    hemp_free(hemp);
    return 0;
}


