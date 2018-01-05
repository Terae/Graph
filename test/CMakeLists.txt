option(GRAPH_Sanitizer    "Build test suite with Clang sanitizer"          OFF)
option(GRAPH_Valgrind     "Execute test suite with Valgrind"               OFF)
option(GRAPH_NoExceptions "Build test suite without exceptions"            OFF)
option(GRAPH_Coverage     "Build the test suite with coverage information" OFF)

# Additional target to perform cppcheck run, requires cppcheck
include(cppcheck.cmake)

if(GRAPH_Sanitizer)
    message(STATUS "Building test suite with Clang sanitizer")
    if(NOT MSVC)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O2 -fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer")
    endif()
endif()

if(GRAPH_Valgrind)
    find_program(CMAKE_MEMORYCHECK_COMMAND valgrind)
    message(STATUS "Executing test suite with Valgrind (${CMAKE_MEMORYCHECK_COMMAND})")
    set(MEMORYCHECK_COMMAND_OPTIONS "--error-exitcode=1 --leak-check=full")
    set(memcheck_command "${CMAKE_MEMORYCHECK_COMMAND} ${CMAKE_MEMORYCHECK_COMMAND_OPTIONS}")
    separate_arguments(memcheck_command)
endif()

if(GRAPH_NoExceptions)
    message(STATUS "Building test suite without exceptions")
    if(NOT MSVC)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DGRAPH_NOEXCEPTION")
    endif()
    set(CATCH_TEST_FILTER -e)
endif()

if(GRAPH_Coverage)
    message(STATUS "Building test suite with coverage information")
    if(NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        message(FATAL_ERROR "GRAPH_Coverage requires GCC.")
    endif()

    find_program(GCOV_PATH gcov)
    find_program(LCOV_PATH lcov)
    find_program(GENHTML_PATH genhtml)

    # enable profiling
    set(CMAKE_CXX_FLAGS "--coverage -g -O0 -fprofile-arcs -ftest-coverage")

    # from https://github.com/RWTH-HPC/CMake-codecov/blob/master/cmake/FindGcov.cmake
    get_filename_component(COMPILER_PATH "${CMAKE_CXX_COMPILER}" PATH)
    string(REGEX MATCH "^[0-9]+" GCC_VERSION "${CMAKE_CXX_COMPILER_VERSION}")
    find_program(GCOV_BIN NAMES gcov-${GCC_VERSION} gcov HINTS ${COMPILER_PATH})

    # add target to collect coverage information and generate HTML file
    # (filter script from https://stackoverflow.com/a/43726240/266378)
    add_custom_target(lcov_html
                      # reset all executions counts to zero
                      ${LCOV_PATH} --directory . --zerocounters
                      # capture coverage data
                      COMMAND ${LCOV_PATH} --directory . --capture --output-file graph.info# --gcov-tool ${GCOV_BIN} --rc lcov_branch_coverage=1
                      COMMAND ${LCOV_PATH} -e graph.info ${CMAKE_SOURCE_DIR}/src/Graph.h --output-file graph.info.filtered# --rc lcov_branch_coverage=1
                      COMMAND ${CMAKE_SOURCE_DIR}/test/thirdparty/imapdl/filterbr.py graph.info.filtered > graph.info.filtered.noexcept
                      # generating the report
                      COMMAND ${GENHTML_PATH} --title "STL-like Graph class" --legend --demangle-cpp --output-directory html --show-details# --branch-coverage graph.info.filtered.noexcept
                      WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
                      COMMENT "Generating HTML report test/html/index.html"
                      )

    add_definitions(-DTEST_SINGLE_HEADER_FILE)
endif()

##########################################################
# Catch library with the main function to speed up build #
##########################################################

add_library(catch_main OBJECT "src/unit.cpp")

set_target_properties(catch_main PROPERTIES
                      CXX_STANDARD 11
                      CXX_STANDARD_REQUIRED ON
                      COMPILE_DEFINITIONS "$<$<CXX_COMPILER_ID:MSVC>:_SCL_SECURE_NO_WARNINGS>"
                      COMPILE_OPTIONS "$<$<CXX_COMPILER_ID:MSVC>:/EHsc;$<$<CONFIG:Release>:/Od>>"
                      )
target_include_directories(catch_main PRIVATE "third-party/catch/single_include")

# https://stackoverflow.com/questions/2368811/how-to-set-warning-level-in-cmake
if(MSVC)
	# Force to always compile with W4
	if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
		string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
	else()
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
	endif()

	# Disable warning C4389: '==': signed/unsigned mismatch
	# Disable warning C4309: 'static_cast': truncation of constant value
	# Disable warning C4566: character represented by universal-character-name '\uFF01' cannot be represented in the current code page (1252)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4389 /wd4309 /wd4566")
endif()

##########################################
# One executable for each unit test file #
##########################################

file(GLOB files "src/unit-*.cpp")
foreach(file ${files})
    get_filename_component(file_basename ${file} NAME_WE)
    string(REGEX REPLACE "unit-([^$]+)" "test-graph-\\1" testcase ${file_basename})

    add_executable(${testcase} $<TARGET_OBJECTS:catch_main> ${file})
    set_target_properties(${testcase} PROPERTIES
                          COMPILE_DEFINITIONS "$<$<CXX_COMPILER_ID:MSVC>:_SCL_SECURE_NO_WARNINGS>"
                          COMPILE_OPTIONS "$<$<CXX_COMPILER_ID:MSVC>:/EHsc;$<$<CONFIG:Release>:/Od>>"
                          )
    enable_all_warnings(${testcase})
    target_compile_definitions(${testcase} PRIVATE CATCH_CONFIG_FAST_COMPILE)
    target_include_directories(${testcase} PRIVATE "src" "third-party/catch/single_include")

    add_test(NAME "${testcase}_default"
             COMMAND ${testcase} ${CATCH_TEST_FILTER}
             WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
             )
    set_tests_properties("${testcase}_default" PROPERTIES LABELS "default")

    add_test(NAME "${testcase}_all"
             COMMAND ${testcase} ${CATCH_TEST_FILTER} "*"
             WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
             )
    set_tests_properties("${testcase}_all" PROPERTIES LABELS "all")

    if(GRAPH_Valgrind)
        add_test(NAME "${testcase}_valgrind"
                 COMMAND ${memcheck_command} ${CMAKE_CURRENT_BINARY_DIR}/${testcase} ${CATCH_TEST_FILTER}
                 WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                 )
        set_tests_properties("${testcase}_valgrind" PROPERTIES LABELS "valgrind")
    endif()

endforeach()

#######################################
# Only one graph unit test executable #
#######################################

set(GRAPH_UNITTEST_TARGET_NAME "check_graph")
add_executable(${GRAPH_UNITTEST_TARGET_NAME}
               $<TARGET_OBJECTS:catch_main>
               ${files})

enable_all_warnings("check_graph")

target_include_directories(${GRAPH_UNITTEST_TARGET_NAME} PRIVATE "src" "third-party/catch/single_include")

add_test(NAME "${GRAPH_UNITTEST_TARGET_NAME}_default"
         COMMAND ${GRAPH_UNITTEST_TARGET_NAME}
         WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

add_test(NAME "${GRAPH_UNITTEST_TARGET_NAME}_all"
         COMMAND ${GRAPH_UNITTEST_TARGET_NAME} "*"
         WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

add_executable(main
               "../main.cpp")
enable_all_warnings(main)
