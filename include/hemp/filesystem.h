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
 * data structures
 *--------------------------------------------------------------------------*/

struct hemp_filesystem_s {
    hemp_p      hemp;
    hemp_list_p path;
}; 


/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

hemp_filesystem_p
    hemp_filesystem_init(
        hemp_p      hemp,
        hemp_str_p  path
    );

void
    hemp_filesystem_free(
        hemp_filesystem_p filesystem
    );

void
    hemp_filesystem_set_path(
        hemp_filesystem_p filesystem, 
        hemp_str_p        path
    );

hemp_str_p
    hemp_filesystem_join_path(
        hemp_str_p base,
        hemp_str_p path
    );


hemp_str_p
    hemp_filesystem_readable_path(
        hemp_filesystem_p   filesystem,
        hemp_str_p          path
    );
    
//hemp_string_t hemp_filesystem_read_file(hemp_filesystem_t filesystem, hemp_string_t path);

hemp_str_p 
    hemp_filesystem_read_file(
        hemp_str_p path
    );


/*--------------------------------------------------------------------------
 * macros
 *--------------------------------------------------------------------------*/

#define hemp_filesystem_clear_path(fs)          \
    if (fs->path) {                             \
        hemp_list_free(fs->path);               \
        fs->path = NULL;                        \
    }

//#define hemp_filesystem_cwd(fs)                 \
//    getcwd(NULL, 0)


#endif /* HEMP_FILESYSTEM_H */