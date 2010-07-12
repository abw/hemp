#ifndef HEMP_VALUE_H
#define HEMP_VALUE_H

#include <string.h>
#include <stdlib.h>
#include "hemp/types.h"

#define HEMP_VALUE_BASE     \
    hemp_type_t  type;      \
    hemp_flags_t  flags;    \
    hemp_value_t next;
    
struct hemp_value {
    HEMP_VALUE_BASE
};


#endif /* HEMP_VALUE_H */
