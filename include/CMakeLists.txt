file(
    GLOB        HEMP_HEADERS
    hemp/*.h
)

install (
    FILES       "${PROJECT_SOURCE_DIR}/include/hemp.h" 
    DESTINATION include
)

install(
    FILES       ${HEMP_HEADERS}
    DESTINATION include/hemp
)

install (
    FILES       "${PROJECT_BINARY_DIR}/include/hemp/config.h" 
    DESTINATION include/hemp
)
