set(CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/test)

macro(gen_test TESTNAME SOURCE)
    add_executable(${TESTNAME} ${SOURCE} ${ARGN})
    add_test(NAME ${TESTNAME} COMMAND ${TESTNAME})
    set_property(TARGET ${TESTNAME} PROPERTY CXX_STANDARD 11)
endmacro()

include_directories(
    ${PROJECT_SOURCE_DIR}/src
)