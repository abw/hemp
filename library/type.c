#include "hemp/type.h"
#include "hemp/debug.h"

static struct hemp_type
    _hemp_type_ = { 
        "HempType",
        NULL,                       /* base */
        0,                          /* size */
        NULL,                       /* init */
        NULL                        /* wipe */
    };
    
hemp_type_t HempType = &_hemp_type_;


hemp_ptr_t
hemp_new(
    hemp_type_t type,
    ...
) {
    hemp_ptr_t item;

    if  (! type->size)
        hemp_fatal("You cannot create instances of %s", type->name);

    if  (! (item = (hemp_ptr_t) hemp_mem_init(type->size + HEMP_TYPE_HEAD)))
        hemp_fatal("Failed to allocate %d bytes for new %s instance", type->name);

    debug_mem(
        "Allocated %d bytes (+ %d for type) at %p for new %s\n", 
        type->size, sizeof(hemp_type_t), item, type->name
    );

    /* The first HEMP_TYPE_HEAD bytes of the memory block (allocated in 
     * addition to the size of the object instance) are set to contain a 
     * pointer to the type record.  We then wind the pointer forward past it.
     */
    *((hemp_type_t *) item) = type;
    item += HEMP_TYPE_HEAD;

    /* Call the init method if the type defines one */
    if (type->init) {
        va_list args;
        va_start(args, type);
        type->init(item, &args);
        va_end(args);
    }

    debug_type("hemp_new(%s) @ %p\n", type->name, item);

    return item;
}



void
hemp_old(
    hemp_ptr_t item
) {
    hemp_type_t type = hemp_type_of(item);
    debug_type("hemp_old(%s) @ %p\n", type->name, item);
    
    /* call the wipe method if there is one defined for this type */
    if (type->wipe)
        type->wipe(item);

    debug_mem(
        "freeing object at %p - %d = %p\n", 
        item, HEMP_TYPE_HEAD, 
        hemp_type_at(item)
    );

    hemp_mem_free( hemp_type_at(item) );
}

