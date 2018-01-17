message(STATUS "CMake version: ${CMAKE_VERSION}")
cmake_minimum_required(VERSION 2.8.8)

# Choose new behaviour for CMP0048
# See https://cmake.org/cmake/help/v3.0/policy/CMP0048.html for more details
if(POLICY CMP0048)
    cmake_policy(SET CMP0048 NEW)
endif()

##
## CONFIGURATION
##
set(LIBRARY_NAME                      "Graph")
set(LIBRARY_SOURCE_DIR                "single_include/")
set(LIBRARY_DEV_DIR                   "src/")
set(LIBRARY_CONFIG_INSTALL_DIR        "lib/cmake/${LIBRARY_NAME}")
set(LIBRARY_INCLUDE_INSTALL_DIR       "include")
set(LIBRARY_HEADER_INSTALL_DIR        "${LIBRARY_INCLUDE_INSTALL_DIR}/${LIBRARY_NAME}")
set(LIBRARY_TARGETS_EXPORT_NAME       "${LIBRARY_NAME}Targets")
set(LIBRARY_CMAKE_CONFIG_TEMPLATE     "cmake/config.cmake.in")
set(LIBRARY_CMAKE_CONFIG_DIR          "${CMAKE_CURRENT_BINARY_DIR}/cmake_config")
set(LIBRARY_CMAKE_VERSION_CONFIG_FILE "${LIBRARY_CMAKE_CONFIG_DIR}/${LIBRARY_NAME}ConfigVersion.cmake")
set(LIBRARY_CMAKE_PROJECT_CONFIG_FILE "${LIBRARY_CMAKE_CONFIG_DIR}/${LIBRARY_NAME}Config.cmake")

##
## PROJECT
##
project(${LIBRARY_NAME} LANGUAGES CXX VERSION 1.1.0)

##
## OPTIONS
##
option(ENABLE_EXAMPLES "Build examples." ON)
option(ENABLE_TESTING  "Build the unit tests." ON)

##
## TARGET
## create target and add include path
##
add_library(${LIBRARY_NAME} INTERFACE)
            #single_include/graph.hpp)

target_include_directories(
        ${LIBRARY_NAME}
        INTERFACE $<INSTALL_INTERFACE:single_include/>
)

# set the Standard version
include(CheckCXXCompilerFlag)
check_cxx_compiler_flag("-std=c++17" COMPILER_SUPPORTS_CXX17)
check_cxx_compiler_flag("-std=c++14" COMPILER_SUPPORTS_CXX14)
check_cxx_compiler_flag("-std=c++11" COMPILER_SUPPORTS_CXX11)
#check_cxx_compiler_flag("-std=c++0x" COMPILER_SUPPORTS_CXX0X)

if (COMPILER_SUPPORTS_CXX17)
    add_definitions(-std=c++17)
    if(CMAKE_VERSION VERSION_GREATER 3.8.1)
        set(CMAKE_CXX_STANDARD 17)
    endif()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -std=gnu++17")
    message(STATUS "The compiler ${CMAKE_CXX_COMPILER} supports C++17")
elseif (COMPILER_SUPPORTS_CXX14)
    add_definitions(-std=c++14)
    set(CMAKE_CXX_STANDARD 14)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -std=gnu++14")
    message(STATUS "The compiler ${CMAKE_CXX_COMPILER} supports C++14")
elseif (COMPILER_SUPPORTS_CXX11)
    add_definitions(-std=c++11)
    set(CMAKE_CXX_STANDARD 11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -std=gnu++11")
    message(STATUS "The compiler ${CMAKE_CXX_COMPILER} supports C++11")
#elseif (COMPILER_SUPPORTS_CXX0X)
#    add_definitions(-std=c++0x)
#    message(STATUS "The compiler ${CMAKE_CXX_COMPILER} supports C++0x")
else ()
    message(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif ()

# Update of Git submodules
execute_process(
        COMMAND git submodule update --init
        WORKING_DIRECTORY ${Graph_SOURCE_DIR}
)

function(enable_all_warnings TARGET)
    if("$CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        target_compile_options(${TARGET} PRIVATE
                               -Weverything
                               -Wno-c++98-compat
                               -Wno-c++98-compat-pedantic
                               -Wno-padded
                               -Wno-shadow
                               -Wno-weak-vtables
                               -pedantic
                               -pedantic-errors)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
	elseif(NOT MSVC)
        set_target_properties(${TARGET} PROPERTIES
                              COMPILE_FLAGS "-Wall"
                              COMPILE_FLAGS "-Wextra"
                              COMPILE_FLAGS "-Waddress"
                              COMPILE_FLAGS "-Wbuiltin-macro-redefined"
                              COMPILE_FLAGS "-Wconversion"
                              COMPILE_FLAGS "-Winit-self"
                              COMPILE_FLAGS "-Wnon-virtual-dtor"
                              COMPILE_FLAGS "-Wold-style-cast"
                              COMPILE_FLAGS "-Woverloaded-virtual"
                              COMPILE_FLAGS "-Wsuggest-attribute=const"
                              COMPILE_FLAGS "-Wsuggest-attribute=noreturn"
                              COMPILE_FLAGS "-Wsuggest-attribute=pure"
                              COMPILE_FLAGS "-Wswitch"
                              COMPILE_FLAGS "-Wunreachable-code"
                              COMPILE_FLAGS "-pedantic"
                              )
    endif()

    if(NOT COMPILER_SUPPORTS_CXX17 OR CMAKE_VERSION VERSION_GREATER 3.8.1)
        set_target_properties(${TARGET} PROPERTIES
                              CXX_STANDARD ${CMAKE_CXX_STANDARD}
                              CXX_STANDARD_REQUIRED ON)
    else()
        set_target_properties(${TARGET} PROPERTIES
                              CXX_STANDARD_REQUIRED ON)
    endif()
endfunction()

if(ENABLE_EXAMPLES)
    include_directories(${LIBRARY_SOURCE_DIR})
    add_subdirectory(doc/examples)
endif()

##
## TESTS
## create and configure the unit test target
##
if(ENABLE_TESTING)
    enable_testing()
    include_directories(${LIBRARY_DEV_DIR})
    add_subdirectory(test)
endif()

##
## INSTALLATION
## install header files, generate and install cmake config for find_package()
##
#include(CMakePackageConfigHelpers)
#write_basic_package_version_file(
#        ${LIBRARY_CMAKE_VERSION_CONFIG_FILE} COMPATIBILITY SameMajorVersion
#)
#configure_package_config_file(
#        ${LIBRARY_CMAKE_CONFIG_TEMPLATE}
#        ${LIBRARY_CMAKE_PROJECT_CONFIG_FILE}
#        INSTALL_DESTINATION ${LIBRARY_CONFIG_INSTALL_DIR}
#)
#install(
#        DIRECTORY ${LIBRARY_SOURCE_DIR}
#        DESTINATION ${LIBRARY_HEADER_INSTALL_DIR}
#)
#install(
#        FILES ${LIBRARY_CMAKE_PROJECT_CONFIG_FILE} ${LIBRARY_CMAKE_VERSION_CONFIG_FILE}
#        DESTINATION ${LIBRARY_CONFIG_INSTALL_DIR}
#)
#install(
#        TARGETS ${LIBRARY_NAME}
#        EXPORT ${LIBRARY_TARGETS_EXPORT_NAME}
#        INCLUDES DESTINATION ${LIBRARY_INCLUDE_INSTALL_DIR}
#)
#install(
#        EXPORT ${LIBRARY_TARGETS_EXPORT_NAME}
#        DESTINATION ${LIBRARY_CONFIG_INSTALL_DIR}
#)
