########################################
# One executable for each example file #
########################################

file(GLOB files "*.cpp")
foreach(file ${files})
    get_filename_component(file_basename ${file} NAME_WE)
    string(REGEX REPLACE "([^$]+)" "example-\\1" examplecase ${file_basename})

    add_executable(${examplecase} "../single_include/graph.hpp" ${file})
    set_target_properties(${examplecase} PROPERTIES
                          COMPILE_DEFINITIONS "$<$<CXX_COMPILER_ID:MSVC>:_SCL_SECURE_NO_WARNINGS>"
                          COMPILE_OPTIONS "$<$<CXX_COMPILER_ID:MSVC>:/EHsc;$<$<CONFIG:Release>:/Od>>"
                          )
    enable_all_warnings(${examplecase})
endforeach()
