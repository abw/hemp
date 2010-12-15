add_test(action action)
add_executable(action action.c)
target_link_libraries(action hemp_library ${PROGRAM_LIBS})

add_test(codec codec)
add_executable(codec codec.c)
target_link_libraries(codec hemp_library ${PROGRAM_LIBS})

add_test(context context)
add_executable(context context.c)
target_link_libraries(context hemp_library ${PROGRAM_LIBS})

add_test(dialect dialect)
add_executable(dialect dialect.c)
target_link_libraries(dialect hemp_library ${PROGRAM_LIBS})

add_test(document document)
add_executable(document document.c)
target_link_libraries(document hemp_library ${PROGRAM_LIBS})

