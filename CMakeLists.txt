cmake_minimum_required (VERSION 2.8)

project (Hemp)

set(HEMP_VERSION_MAJOR     0)
set(HEMP_VERSION_MINOR     2)
set(HEMP_NAME              "hemp")
set(HEMP_AUTHOR            "Andy Wardley")
set(HEMP_EMAIL             "hemp@wardley.org")
set(HEMP_URL               "")
set(HEMP_MODULE_DIR        "modules")
set(HEMP_MODULE_EXT        ".hso")
set(HEMP_MODULE_PATH       ${PROJECT_BINARY_DIR}/modules)
set(HEMP_SOURCE_DIR        ${PROJECT_SOURCE_DIR})
set(HEMP_TEST_DIR          ${PROJECT_SOURCE_DIR}/tests)

#set(ENV{HEMP_MODULE_PATH} ${PROJECT_BINARY_DIR}/modules)

set(CMAKE_MODULE_PATH       ${PROJECT_SOURCE_DIR}/cmake)
set(CMAKE_C_FLAGS           "${CMAKE_C_FLAGS} -g -Wall")

#set(CMAKE_MODULE_LINKER_FLAGS "-Wl,-undefined,dynamic_lookup")
#set(CMAKE_SHARED_LINKER_FLAGS "-rdynamic -Wl,-undefined,error")

add_definitions( -DHEMP_DEBUG=4 )
enable_testing()

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

find_package( ReadLine REQUIRED )

# do we also need dl and/or other platform-specific library?
set(
    PROGRAM_LIBS 
    ${PROGRAM_LIBS} 
    readline m
)

# doesn't do anything yet
option (
    HEMP_DEBUG 
    "Enable debugging"
    ON
)

# Build the custom hemp/config.h header file. 
# This feature alone makes CMake worth the price of admission.
configure_file (
    "${PROJECT_SOURCE_DIR}/include/hemp/config.h.in"
    "${PROJECT_BINARY_DIR}/include/hemp/config.h"
)

# location of header files for build
include_directories(
    "${PROJECT_BINARY_DIR}/include"
    "${PROJECT_SOURCE_DIR}/include"
)

add_subdirectory(include)
add_subdirectory(library)
add_subdirectory(programs)
add_subdirectory(modules/codec)
add_subdirectory(modules/language)

# nothing in here yet
#add_subdirectory(modules/tag)

# disabled while testing languages
#add_subdirectory(tests)
add_subdirectory(tests/core)
add_subdirectory(tests/type)
add_subdirectory(tests/language/test)

message(STATUS "Summary of configuration:") 
message(STATUS " + Project source dir............ : ${PROJECT_SOURCE_DIR}") 
message(STATUS " + Project binary dir............ : ${PROJECT_BINARY_DIR}") 
message(STATUS " + Installation prefix .......... : ${CMAKE_INSTALL_PREFIX}") 
message(STATUS " + Readline library ............. : ${READLINE_LIBRARY}") 
message(STATUS " + Ncurses library .............. : ${NCURSES_LIBRARY}") 
message(STATUS " + C Flags ...................... : ${CMAKE_C_FLAGS}") 
message(STATUS " + Module link flags ............ : ${CMAKE_MODULE_LINKER_FLAGS}") 

# include(MaintainerMode)
