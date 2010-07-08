#include <stdio.h>
#include "config.h"
#include "hemp.h"
#include "hemp/debug.h"


int
main(int argc, char **argv, char **env)
{
    char *a = hemp_mem_init(100);
    sprintf(a, "Hello %s!\n", "World");
    debug_green(a);
    printf("hemp >\n");
    return 0;
}
