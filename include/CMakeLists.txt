file(
    GLOB        HEMP_HEADERS
    hemp/*.h
)

# NOTE: I tried using GLOB_RECURSE but that removes the relative path so
# all header files end up under include/hemp

file(
    GLOB        HEMP_LANGUAGE_HEADERS
    hemp/language/*.h
)

file(
    GLOB        HEMP_OS_HEADERS
    hemp/os/*.h
)

file(
    GLOB        HEMP_SCHEME_HEADERS
    hemp/scheme/*.h
)

file(
    GLOB        HEMP_TYPE_HEADERS
    hemp/type/*.h
)

install (
    FILES       "${PROJECT_SOURCE_DIR}/include/hemp.h" 
    DESTINATION include
)

install (
    FILES       "${PROJECT_BINARY_DIR}/include/hemp/config.h" 
    DESTINATION include/hemp
)

install(
    FILES       ${HEMP_HEADERS}
    DESTINATION include/hemp
)

install(
    FILES       ${HEMP_LANGUAGE_HEADERS}
    DESTINATION include/hemp/language
)

install(
    FILES       ${HEMP_OS_HEADERS}
    DESTINATION include/hemp/os
)

install(
    FILES       ${HEMP_SCHEME_HEADERS}
    DESTINATION include/hemp/scheme
)

install(
    FILES       ${HEMP_TYPE_HEADERS}
    DESTINATION include/hemp/type
)

