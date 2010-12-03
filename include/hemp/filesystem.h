#ifndef HEMP_FILESYSTEM_H
#define HEMP_FILESYSTEM_H

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <hemp/core.h>

#include "hemp/error.h"


#define HEMP_CWD_NAME     "current working directory"
#define HEMP_FS_PATH_NAME "filesystem path"



/*--------------------------------------------------------------------------
 * type definitions
 *--------------------------------------------------------------------------*/

struct hemp_filesystem {
    hemp_hemp       hemp;
    hemp_list       path;
}; 


/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

hemp_filesystem
hemp_filesystem_new(
    hemp_hemp       hemp,
    hemp_string     path
);

void
hemp_filesystem_free(
    hemp_filesystem filesystem
);

void
hemp_filesystem_set_path(
    hemp_filesystem filesystem, 
    hemp_string     path
);

hemp_string
hemp_filesystem_join_path(
    hemp_string     base,
    hemp_string     path
);

hemp_string
hemp_filesystem_readable_path(
    hemp_filesystem filesystem,
    hemp_string     path
);
    
hemp_string 
hemp_filesystem_read_file(
    hemp_string     path
);


/*--------------------------------------------------------------------------
 * macros
 *--------------------------------------------------------------------------*/

#define hemp_filesystem_clear_path(fs)          \
    if (fs->path) {                             \
        hemp_list_free(fs->path);               \
        fs->path = NULL;                        \
    }


#endif /* HEMP_FILESYSTEM_H */