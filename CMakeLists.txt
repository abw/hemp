cmake_minimum_required (VERSION 2.6)
project (Hemp)

set (HEMP_VERSION_MAJOR     0)
set (HEMP_VERSION_MINOR     2)
set (HEMP_NAME              "hemp")
set (HEMP_AUTHOR            "Andy Wardley")
set (HEMP_EMAIL             "hemp@wardley.org")
set (HEMP_URL               "")

set (EXTRA_LIBS ${EXTRA_LIBS} rl)

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

