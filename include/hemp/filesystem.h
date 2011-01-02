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
    HempType       type;
    Hemp       hemp;
    HempList       roots;
};

struct hemp_path {
    HempType       type;
    HempUri        uri;
    HempFilesystem hemp;
};




/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

HempFilesystem
hemp_filesystem_new(
    Hemp       hemp
);

void
hemp_filesystem_free(
    HempFilesystem filesystem
);

void 
hemp_filesystem_roots(
    HempFilesystem filesystem,
    HempList       roots
);

void 
hemp_filesystem_add_root(
    HempFilesystem filesystem,
    HempString     root
);

void
hemp_filesystem_set_path(
    HempFilesystem filesystem, 
    HempString     path
);

HempString
hemp_filesystem_join_path(
    HempString     base,
    HempString     path
);

HempString
hemp_filesystem_readable_path(
    HempFilesystem filesystem,
    HempString     path
);
    
HempString 
hemp_filesystem_read_file(
    HempString     path
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