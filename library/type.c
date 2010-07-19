#include "hemp/type.h"
#include "hemp/error.h"
#include "hemp/debug.h"



/*--------------------------------------------------------------------------
 *  hemp_type_init(name, size)
 *
 *  Create and initialise a new hemp_type data structure.  The default
 *  acquire/release methods are bound to it.
 *--------------------------------------------------------------------------*/

hemp_type_t
hemp_type_init(
    hemp_name_t name,
    hemp_size_t size
) {
    // eventually we might want to make types object themselves, in which 
    // case we need to allocate the additional space for the type pointer
    // and have it point to the "type type", i.e. a vtable containing
    // methods for manipulating types, aka MOP
    hemp_type_t type = (hemp_type_t) 
        hemp_mem_init( 
            sizeof(struct hemp_type) 
        );

    if (! type)
        hemp_fatal(HEMP_ERRMSG_MALLOC, "type");

    debug_mem(
        "Allocated %d bytes at %p for new %s type\n", 
        size, type, name
    );

    // darn, we can't call hemp_type_prepare() because it's expecting a 
    // va_args list.  I can think of one really ugly hacky way to do it, 
    // by calling a delegation method delegate(&func_ptr, arg1, arg2, arg3)
    // hmm... there must be a better way...  perhaps we have to move the 
    // va_list code back into the prepare() method?

    type->name    = strdup(name);
    type->size    = size;
    type->acquire = &hemp_item_acquire;
    type->release = &hemp_item_release;
    type->parent  = NULL;
    type->prepare = NULL;
    type->cleanup = NULL;

    return type;
}

hemp_bool_t
hemp_type_prepare(
    hemp_type_t type,
    HEMP_ARGS
) {
    hemp_name_t name = HEMP_ARG(hemp_name_t);
    hemp_size_t size = HEMP_ARG(hemp_size_t);
    
    type->name    = strdup(name);
    type->size    = size;
    type->acquire = &hemp_item_acquire;
    type->release = &hemp_item_release;
    type->parent  = NULL;
    type->prepare = NULL;
    type->cleanup = NULL;

    return HEMP_TRUE;
}


/*--------------------------------------------------------------------------
 *  hemp_type_child(type, name, size)
 *
 *  Create a new type as a child (sub-type) of a parent type. 
 *--------------------------------------------------------------------------*/

hemp_type_t
hemp_type_child(
    hemp_type_t type,
    hemp_name_t name,
    hemp_size_t size
) {
    hemp_type_t child = hemp_type_init(name, size);
    child->parent = type;
    return child;
}


/*--------------------------------------------------------------------------
 *  hemp_type_free(type)
 *
 *  Cleanup and release the memory allocated to a type.
 *--------------------------------------------------------------------------*/

hemp_type_t
hemp_type_free(
    hemp_type_t type
) {
    hemp_mem_free((void *) type->name);
    hemp_mem_free(type);
}


/*--------------------------------------------------------------------------
 * hemp_new(type, ...)
 *
 * Generic function for creating a new instance of any type.  Calls the 
 * type's acquire() method to create an object and then the prepare() method
 * if one is defined.  This is a candidate for inlining via a macro.
 *--------------------------------------------------------------------------*/

hemp_item_t
hemp_new(
    hemp_type_t type,
    ...
) {
    hemp_item_t item = type->acquire(type);

    debug_type("hemp_new(%s) @ %p\n", type->name, item);

    /* TODO: walk up through parents to find any/all prepare methods */
    if (type->prepare) {
        va_list args;
        va_start(args, type);
        type->prepare(item, &args);
        va_end(args);
    }

    return item;
}


/*--------------------------------------------------------------------------
 * hemp_old(item)
 *
 * The antiverse of hemp_new(), this calls the cleanup() method for the 
 * object's type (if defined) and then release() to free up the memory.
 * Also a candidate for inlining via a macro.
 *--------------------------------------------------------------------------*/

void
hemp_old(
    hemp_item_t item
) {
    hemp_type_t type = hemp_type_of(item);

    debug_type("hemp_old(%s) @ %p\n", type->name, item);

    /* TODO: walk up through parents to find any/all cleanups */
    if (type->cleanup)
        type->cleanup(item);

    type->release(item);
}


/*--------------------------------------------------------------------------
 *  hemp_item_acquire(type)
 *
 *  The default memory allocator for creating new typed data items (objects)
 *--------------------------------------------------------------------------*/

hemp_item_t
hemp_item_acquire(
    hemp_type_t type
) {
    hemp_item_t item;

    if  (! type->size)
        hemp_fatal("You cannot create instances of %s", type->name);

    if  (! (item = (hemp_item_t) hemp_mem_init(type->size + HEMP_TYPE_SIZE)))
        hemp_fatal(HEMP_ERRMSG_MALLOC, type->name);

    debug_mem(
        "Allocated %d bytes (+ %d for type) at %p for new %s\n", 
        type->size, sizeof(hemp_type_t), item, type->name
    );

    /* The first HEMP_TYPE_SIZE bytes of the memory block (allocated in 
     * addition to the size of the object instance) are set to contain a 
     * pointer to the type record.  We then wind the pointer forward past it.
     */
    *((hemp_type_t *) item) = type;
    item += HEMP_TYPE_SIZE;

    debug_type(
        "hemp_item_acquire(%s) @ %p\n", 
        type->name, item
    );

    return item;
}


/*--------------------------------------------------------------------------
 *  hemp_item_release(item)
 *
 *  The default memory de-allocater for freeing typed data items
 *--------------------------------------------------------------------------*/

void
hemp_item_release(
    hemp_item_t item
) {
    debug_type(
        "hemp_item_release(%s) @ %p\n", 
        hemp_type_name(item), item
    );

    debug_mem(
        "freeing object at %p - %d = %p\n", 
        item, HEMP_TYPE_SIZE, 
        hemp_type_at(item)
    );

    hemp_mem_free( hemp_type_at(item) );
}
