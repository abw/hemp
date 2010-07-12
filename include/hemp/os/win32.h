#ifndef HEMP_OS_WIN32_H
#define HEMP_OS_WIN32_H

/* Little of this has been implemented and/or tested */

#define HEMP_PATH_SEPARATOR     ";"
#define HEMP_DIR_SEPARATOR      "\\"
#define HEMP_CURRENT_DIR        "."
#define HEMP_PARENT_DIR         ".."

#define hemp_filesystem_is_path_absolute(fs,path)    \
    (hemp_todo("hemp_filesystem_is_absolute() on Win32"))


#endif /* HEMP_OS_WIN32_H */