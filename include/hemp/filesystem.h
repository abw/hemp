#ifndef HEMP_FILESYSTEM_H
#define HEMP_FILESYSTEM_H

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "hemp/types.h"
#include "hemp/list.h"
#include "hemp/error.h"
#include "hemp/utils.h"
#include "hemp/os.h"


#define HEMP_CWD_NAME     "current working directory"
#define HEMP_FS_PATH_NAME "filesystem path"



/*--------------------------------------------------------------------------
 * data structures
 *--------------------------------------------------------------------------*/

struct hemp_filesystem {
    hemp_t      hemp;
    hemp_list_t path;
}; 


/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

hemp_filesystem_t
    hemp_filesystem_init(
        hemp_t      hemp,
        hemp_cstr_t path
    );

void
    hemp_filesystem_free(
        hemp_filesystem_t filesystem
    );

void
    hemp_filesystem_set_path(
        hemp_filesystem_t filesystem, 
        hemp_cstr_t       path
    );

hemp_cstr_t
    hemp_filesystem_join_path(
        hemp_cstr_t base,
        hemp_cstr_t path
    );


hemp_cstr_t
hemp_filesystem_readable_path(
    hemp_filesystem_t   filesystem,
    hemp_cstr_t         path
);
    
    

//hemp_cstr_t hemp_filesystem_read_file(hemp_filesystem_t filesystem, hemp_cstr_t path);
hemp_cstr_t hemp_filesystem_read_file(hemp_cstr_t path);


/*--------------------------------------------------------------------------
 * macros
 *--------------------------------------------------------------------------*/

#define hemp_filesystem_clear_path(fs)          \
    if (fs->path)                               \
        hemp_list_null(fs->path);

//#define hemp_filesystem_cwd(fs)                 \
//    getcwd(NULL, 0)


#endif /* HEMP_FILESYSTEM_H */