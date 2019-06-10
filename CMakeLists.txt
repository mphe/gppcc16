cmake_minimum_required(VERSION 2.6)
project("gppcc16")

# Options {{{
set(CMAKE_MODULE_PATH ${CMAKE_MODULEPATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

set(SFML_ROOT "" CACHE PATH "SFML library directory (optional)")

option(GPPCC16_BUILD_TESTS "Build tests" ON)
option(USE_CCACHE "Use ccache if available" ON)

# Enable warnings and colored compiler output
if (CMAKE_COMPILER_IS_GNUCC)
    add_definitions(
        -Wall
        -Wextra
        -Wno-unused-parameter
        -Wno-switch
        -Wno-sign-compare
        -Wno-parentheses
        -Wsuggest-override
        -Wsuggest-final-methods
        -Wsuggest-final-types
        -Werror=suggest-override
        -Werror=return-type
        -fdiagnostics-color=always
    )
elseif (MSVC)
    # Untested
    add_definitions(/W3)
endif()

# Use ccache if available
if(USE_CCACHE)
    find_program(CCACHE_FOUND ccache)
    if(CCACHE_FOUND)
        set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
        set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
    endif(CCACHE_FOUND)
endif(USE_CCACHE)

# }}} Options

# sfml
find_package(SFML 2.4 COMPONENTS system window graphics REQUIRED)
include_directories(${SFML_INCLUDE_DIR})

add_subdirectory("extlib")
add_subdirectory("src/gppcc16")

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()
