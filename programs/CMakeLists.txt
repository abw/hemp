add_executable(
    hemp_program
    hemp.c
)

set_target_properties(
    hemp_program
    PROPERTIES 
    OUTPUT_NAME hemp
)

target_link_libraries(
    hemp_program
    hemp_library
)

install (
    TARGETS     hemp_program
    DESTINATION bin
)

