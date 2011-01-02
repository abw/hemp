#include <hemp/filesystem.h>


HempFilesystem
hemp_filesystem_new(
    Hemp   hemp
) {
    HempFilesystem filesystem; 
    HEMP_ALLOCATE(filesystem);

    filesystem->hemp  = hemp;
    filesystem->roots = hemp_list_new();

//    if (path)
//        hemp_filesystem_set_path(filesystem, path);
    
    return filesystem;
}


void
hemp_filesystem_free(
    HempFilesystem filesystem
) {
    hemp_list_free(filesystem->roots);
    hemp_mem_free(filesystem);
}


void 
hemp_filesystem_roots(
    HempFilesystem filesystem,
    HempList       roots
) {
    hemp_list_push_list(filesystem->roots, roots);
}


void 
hemp_filesystem_add_root(
    HempFilesystem filesystem,
    HempString     root
) {
    hemp_list_push(filesystem->roots, hemp_str_val(root));
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
 * and merging code in uri.c / HempUri
 *
 */

void
OLD_hemp_filesystem_set_path(
    HempFilesystem filesystem,
    HempString     path
) {
//    hemp_filesystem_clear_path(filesystem);             // TODO: free strings
//    hemp_debug_file("setting filesystem path to %s\n", path);
//    filesystem->path = hemp_string_split(path, HEMP_PATH_SEPARATOR);      // TODO: handle errors?
//    // TODO: the following now leaks memory as the text object returned by 
//    // hemp_list_dump() is never freed.
//    hemp_debug_file("filesystem path is now %s\n", hemp_list_dump(filesystem->path));
}


HempString
hemp_filesystem_cwd(
    HempFilesystem filesystem
) {
    // HMM... not sure about this - see comments on cwd in 
    // Badger::Filesystem::Virtual... it doesn't really make sense in the 
    // context of a virtual filesystem
    HempString cwd = getcwd(NULL, 0);
    if (! cwd)
        hemp_fatal("Can't read current working directory");     // TODO: proper error
    return cwd;
}


HempString
hemp_filesystem_join_path(
    HempString base,
    HempString path
) {
    // quick hack to get something working - this needs doing properly to
    // clean up the generated path to make it canonical (e.g. collapse 
    // multiple slashes, resolve /./ and /../ elements, and so on)
    HempSize baselen = strlen(base);
    HempBool slashb  = baselen && base[baselen - 1] == *HEMP_DIR_SEPARATOR;
    HempBool slashp  = *path == *HEMP_DIR_SEPARATOR;
    HempString  joined  = hemp_mem_alloc(
        baselen + strlen(path) + 3              /* terminating NUL and extra slashes */
    );
    if (! joined)
        hemp_mem_fail(HEMP_FS_PATH_NAME);
    
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
    
//  hemp_debug("joined [%s] + [%s] => [%s]\n", base, path, joined);
    return joined;
}


HempString
hemp_filesystem_absolute_path(
    HempFilesystem filesystem,
    HempString     path
) {
    return hemp_filesystem_is_path_absolute(filesystem, path)
        ? path                              // bugger!  can't do this - don't know what we can free
        : hemp_filesystem_join_path(
              HEMP_ROOT_DIR,
              path
          );
}


HempString
hemp_filesystem_readable_path(
    HempFilesystem filesystem,
    HempString     path
) {
    int n;
    HempString root;
    HempString full = NULL;
    
    for (n = 0; n < filesystem->roots->length; n++) {
        root = hemp_val_str( hemp_list_item(filesystem->roots, n) );
        full = hemp_filesystem_join_path(root, path);
//      hemp_debug("full path: %s\n", full);
        hemp_mem_free(full);
    }
    
    return full;
}
        


HempString
hemp_filesystem_read_file(
    HempString path
) {
    HempString text = NULL;
    HempSize   size = 0;
    FILE *fp = fopen(path,"r");

    if (fp) {
        fseek(fp, 0, SEEK_END);
        size = ftell(fp);
        fseek(fp, 0, SEEK_SET);

        text = hemp_mem_alloc(size + 1);

        if (fread(text, size, 1, fp)) {
            text[size] = HEMP_NUL;
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


