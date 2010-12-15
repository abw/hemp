cmake_minimum_required (VERSION 2.8)
enable_testing()


project (Hemp)

set(HEMP_VERSION_MAJOR     0)
set(HEMP_VERSION_MINOR     2)
set(HEMP_NAME              "hemp")
set(HEMP_AUTHOR            "Andy Wardley")
set(HEMP_EMAIL             "hemp@wardley.org")
set(HEMP_URL               "")

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

include(CheckIncludeFile)
check_include_file(dirent.h     HEMP_HAVE_DIRENT_H)
check_include_file(memory.h     HEMP_HAVE_MEMORY_H)
check_include_file(stdint.h     HEMP_HAVE_STDINT_H)
check_include_file(stdlib.h     HEMP_HAVE_STDLIB_H)
check_include_file(string.h     HEMP_HAVE_STRING_H)
check_include_file(strings.h    HEMP_HAVE_STRINGS_H)
check_include_file(inttypes.h   HEMP_HAVE_INTTYPES_H)
check_include_file(sys/stat.h   HEMP_HAVE_SYS_STAT_H)
check_include_file(sys/types.h  HEMP_HAVE_SYS_TYPES_H)
check_include_file(unistd.h     HEMP_HAVE_UNISTD_H)
check_include_file(windows.h    HEMP_HAVE_WINDOWS_H) 

include(CheckFunctionExists)
check_function_exists(bcopy     HEMP_HAVE_BCOPY)
check_function_exists(memmove   HEMP_HAVE_MEMMOVE)
check_function_exists(memset    HEMP_HAVE_MEMSET)
check_function_exists(strdup    HEMP_HAVE_STRDUP)
check_function_exists(strerror  HEMP_HAVE_STRERROR)
check_function_exists(strtol    HEMP_HAVE_STRTOL)
check_function_exists(strtoll   HEMP_HAVE_STRTOLL)
check_function_exists(strtoq    HEMP_HAVE_STRTOQ)

find_package( Readline REQUIRED )

set(
    PROGRAM_LIBS 
    ${PROGRAM_LIBS} 
    readline
)

option (
    HEMP_DEBUG 
    "Enable debugging"
    ON
)

configure_file (
    "${PROJECT_SOURCE_DIR}/include/hemp/config.h.in"
    "${PROJECT_BINARY_DIR}/include/hemp/config.h"
)

include_directories("${PROJECT_BINARY_DIR}/include")

add_subdirectory(include)
add_subdirectory(library)
add_subdirectory(programs)
add_subdirectory(tests/core)

message(STATUS "Summary of configuration:") 
message(STATUS " + Install prefix ............... : ${CMAKE_INSTALL_PREFIX}") 
message(STATUS " + Readline library ............. : ${READLINE_LIBRARY}") 
message(STATUS " + Ncurses library .............. : ${NCURSES_LIBRARY}") 
