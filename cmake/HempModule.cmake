macro(hemp_module type name)
    add_library(
        hemp_${type}_${name} 
        MODULE 
        ${name}.c
    )
    set_target_properties(
        hemp_${type}_${name}
        PROPERTIES
        OUTPUT_NAME ${name}
        PREFIX      ""
        SUFFIX      ${HEMP_MODULE_EXT}
    )
    target_link_libraries(
        hemp_${type}_${name}
        hemp_library
    )
endmacro(hemp_module)
