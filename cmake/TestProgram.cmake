macro(test_program name)
  add_test(${name} ${name})
  add_executable(${name} ${name}.c)
  target_link_libraries(${name} hemp_library ${PROGRAM_LIBS})
#  set_target_properties(${name} PROPERTIES COMPILE_FLAGS "-DHEMP_TESTDIR=${HEMP_TESDIR}")

endmacro(test_program)
