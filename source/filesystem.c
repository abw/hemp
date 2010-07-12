#include <stdio.h>
#include "hemp/filesystem.h"
#include "hemp/debug.h"


hemp_filesystem_t
hemp_filesystem_init(
    hemp_t      hemp,
    hemp_cstr_t path
) {
    hemp_filesystem_t filesystem;

    if (
        (filesystem = (hemp_filesystem_t) 
            hemp_mem_init( sizeof(struct hemp_filesystem) )
        )
    ) {
        filesystem->hemp = hemp;
        filesystem->path = NULL;
        if (path)
            hemp_filesystem_set_path(filesystem, path);
    }
    
    return filesystem;
}


void
hemp_filesystem_free(
    hemp_filesystem_t filesystem
) {
    hemp_filesystem_clear_path(filesystem);
    hemp_mem_free(filesystem);
}



/* NOTE: I find it confusing to have "path" refer to both the filesystem
 * search path, e.g. /home/abw/foo:/usr/local/share/foo:/tmp/foo and also
 * a single file/directory path: /home/abw/foo.  Gotta think up some less
 * confusing terminology...
 * Come to think of it, there's a third use: File::Spec differentiates 
 * between a directory (split_dir, join_dir) and a path which can have
 * a volume, directory and file, e.g. C:\BLAH\BLAH
 * I think we should refer to anything like file:/blah/blah or C:\BLAH\BLAH 
 * as a URI.... but that makes me think I should be writing the path splitting
 * and merging code in uri.c / hemp_uri
 *
 */

void
hemp_filesystem_set_path(
    hemp_filesystem_t filesystem,
    hemp_cstr_t path
) {
    hemp_filesystem_clear_path(filesystem);             // TODO: free strings
    debug_file("setting filesystem path to %s\n", path);
    filesystem->path = hemp_cstr_split(path, HEMP_PATH_SEPARATOR);      // TODO: handle errors?
    debug_file("filesystem path is now %s\n", hemp_list_dump(filesystem->path));
}


hemp_cstr_t
hemp_filesystem_cwd(
    hemp_filesystem_t filesystem
) {
    // HMM... not sure about this - see comments on cwd in 
    // Badger::Filesystem::Virtual... it doesn't really make sense in the 
    // context of a virtual filesystem
    hemp_cstr_t cwd = getcwd(NULL, 0);
    if (! cwd)
        hemp_fatal(HEMP_MEMORY_ERROR_MSG, HEMP_CWD_NAME);
    return cwd;
}


hemp_cstr_t
hemp_filesystem_join_path(
    hemp_cstr_t base,
    hemp_cstr_t path
) {
    // quick hack to get something working - this needs doing properly to
    // clean up the generated path to make it canonical (e.g. collapse 
    // multiple slashes, resolve /./ and /../ elements, and so on)
    hemp_size_t baselen = strlen(base);
    hemp_bool_t slashb  = baselen && base[baselen - 1] == *HEMP_DIR_SEPARATOR;
    hemp_bool_t slashp  = *path == *HEMP_DIR_SEPARATOR;
    hemp_cstr_t joined  = hemp_mem_init(
        baselen + strlen(path) + 3              /* terminating NUL and extra slashes */
    );
    if (! joined)
        hemp_fatal(HEMP_MEMORY_ERROR_MSG, HEMP_FS_PATH_NAME);
    
    /* always add a leading slash */
    if (*base == *HEMP_ROOT_DIR) {
        strcpy(joined, base);
    }
    else {
        strcpy(joined, HEMP_ROOT_DIR);
        strcat(joined, base);
    }
    
    /* If we've got a trailing slash on the base then we can skip over a 
     * leading slash on the path.  If we don't have either then we must 
     * add one
     */
    if (slashb && slashp)
        path++;
    else if (! slashb && ! slashp)
        strcat(joined, HEMP_DIR_SEPARATOR);

    strcat(joined, path);
    
//  debug("joined [%s] + [%s] => [%s]\n", base, path, joined);
    return joined;
}


hemp_cstr_t
hemp_filesystem_absolute_path(
    hemp_filesystem_t   filesystem,
    hemp_cstr_t         path
) {
    return hemp_filesystem_is_path_absolute(filesystem, path)
        ? path                              // bugger!  can't do this - don't know what we can free
        : hemp_filesystem_join_path(
              HEMP_ROOT_DIR,
              path
          );
}


hemp_cstr_t
hemp_filesystem_readable_path(
    hemp_filesystem_t   filesystem,
    hemp_cstr_t         path
) {
    int n;
    hemp_cstr_t root, full;
    
    for (n = 0; n < filesystem->path->length; n++) {
        root = hemp_list_item(filesystem->path, n);
        full = hemp_filesystem_join_path(root, path);
//      debug("full path: %s\n", full);
        hemp_mem_free(full);
    }
    
    return full;
}
        


hemp_cstr_t 
hemp_filesystem_read_file(
    hemp_cstr_t path
) {
    hemp_cstr_t text = NULL;
    hemp_size_t size = 0;
    FILE *fp = fopen(path,"r");
    struct stat stat_buf;

    if (fp) {
        fstat(fileno(fp), &stat_buf);
        text = hemp_mem_init(stat_buf.st_size + 1);

        if (fread(text, stat_buf.st_size, 1, fp)) {
            text[stat_buf.st_size] = '\0';
        }
        else {
            perror(path);               // fix me
        }
        fclose(fp);
    }
    else {
        perror(path);
    } 

    return text;
}

