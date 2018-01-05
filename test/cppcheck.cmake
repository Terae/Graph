# Target to perform cppchack run, requires cppcheck
set(ALL_SOURCE_FILES
    ${CMAKE_SOURCE_DIR}/single_include/graph.hpp

    ${CMAKE_SOURCE_DIR}/src/detail.hpp
    ${CMAKE_SOURCE_DIR}/src/Graph.cpp
    ${CMAKE_SOURCE_DIR}/src/Graph.h
    ${CMAKE_SOURCE_DIR}/src/Node.cpp
    ${CMAKE_SOURCE_DIR}/src/Node.h
    )

add_custom_target(
        cppcheck
        COMMAND /usr/bin/cppcheck
        --enable=warning,style,performance,portability,information,missingInclude
        --force
        --inconclusive
        --std=c++11
        --template=gcc#"[{severity}][{id}] {message} {callstack} \(On {file}:{line}\)"
        --error-exitcode=1
        ${ALL_SOURCE_FILES}
)
