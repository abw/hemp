file(
    GLOB hemp_library_HEADERS 
    hemp/*.h
)

install (
    FILES 
    "${PROJECT_BINARY_DIR}/include/hemp.h" 
    DESTINATION 
    include
)
