#include "EXTERN.h"
#include "perl.h"
#include "XSUB.h"

#include "hemp.h"

MODULE = Hemp     PACKAGE = Hemp
PROTOTYPES: DISABLE

char *
hemp_version()
    CODE:
        RETVAL = (char *) hemp_version();
    OUTPUT:
        RETVAL

