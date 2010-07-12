#ifndef HEMP_OS_UNIX_H
#define HEMP_OS_UNIX_H


#define HEMP_PATH_SEPARATOR     ":"
#define HEMP_DIR_SEPARATOR      "/"
#define HEMP_ROOT_DIR           "/"
#define HEMP_CURRENT_DIR        "."
#define HEMP_PARENT_DIR         ".."

#define hemp_filesystem_is_path_absolute(fs,path)    \
    (*path == *HEMP_ROOT_DIR)


#endif /* HEMP_OS_UNIX_H */