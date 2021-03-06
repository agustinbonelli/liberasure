cmake_minimum_required( VERSION 3.2 )
set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake/modules/ )

include(CMakeParseArguments)
message(STATUS ${CMAKE_MODULE_PATH})
include(assert_build_fails)

#########################
# function definitions  #
#########################

# add_markdown
# ------------
#
# Add a markdown target that compiles into an html file.
#
# Parameters:
# OPT_TARGET     -- the target being defined (relative to binary dir)
# OPT_SOURCE     -- the main source file (relative to source dir)
# OPT_STYLESHEET -- the html stylesheet (css) that will be used to render the
#                   html document. (relative to source dir)
#
# Other dependencies:
# MD -- the markdown compiler script
#       (has to accept the three arguments in SOURCE, STYLESHEET, TARGET order)
#
set( MD "${CMAKE_SOURCE_DIR}/build_markdown.sh" )
function(add_markdown OPT_TARGET OPT_SOURCE OPT_STYLESHEET)
  set(TARGET "${CMAKE_CURRENT_BINARY_DIR}/${OPT_TARGET}")
  set(SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/${OPT_SOURCE}")
  set(STYLESHEET "${CMAKE_CURRENT_SOURCE_DIR}/${OPT_STYLESHEET}")

  add_custom_command(
    OUTPUT "${TARGET}"
    COMMAND "${MD}" "${SOURCE}" "${STYLESHEET}" "${TARGET}"
    MAIN_DEPENDENCY "${SOURCE}"
    DEPENDS "${STYLESHEET}" "${MD}"
    VERBATIM)
endfunction(add_markdown)
##############################
#  end function definitions  #
##############################

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

# BUILD TYPE DEFAULTING / PARSING
# If the user specifies -DCMAKE_BUILD_TYPE=BuildType on the command line, take
# their definition and dump it in the cache along with proper documentation,
# otherwise set CMAKE_BUILD_TYPE to Debug prior to calling project(). This is
# the only variable that should be set before the build.
set(CMAKE_BUILD_TYPE_HELP
    "Choose the type of build, options are: "
    " Debug (default), Release, RelWithDebInfo.")
if(DEFINED CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "${CMAKE_BUILD_TYPE}" cache string
      "${CMAKE_BUILD_TYPE_HELP}")
else()
   set(CMAKE_BUILD_TYPE "Debug" cache string "${CMAKE_BUILD_TYPE_HELP}")
endif()

project(liberasure)

enable_testing()

# enable c++14
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -g ")

if (CMAKE_LTO MATCHES "OFF")
  message(STATUS "Turning off -flto")
else()
  message(STATUS "LTO is on, CMAKE_LTO is ${CMAKE_LTO}")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -flto")
endif()

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ferror-limit=2")
endif()

include_directories("${CMAKE_SOURCE_DIR}/include")
include_directories("${CMAKE_SOURCE_DIR}")
add_executable(type_erasure_example       examples/type_erasure_example.cpp)

# movable test & negatives
add_executable(test_type_erasure_movable  test/test_type_erasure_movable.cpp)
add_executable(test_meta test/test_meta.cpp)
add_test(NAME  test_type_erasure_movable COMMAND test_type_erasure_movable)
assert_build_fails(TEST_NAME negative_test_copyable
                   TARGET negative_test_copyable test/test_type_erasure_movable.cpp
                   DEFINITIONS NOCOMPILE_COPYABLE_TEST
                   )
assert_build_fails(TEST_NAME negative_test_movable
                   TARGET negative_test_movable test/test_type_erasure_movable.cpp
                   DEFINITIONS NOCOMPILE_MOVABLE_TEST
                   )

# dereferencable test
add_executable(test_dereferencable        test/test_dereferencable.cpp)
add_test(NAME  test_dereferencable COMMAND test_dereferencable)

# callable test
add_executable(test_callable              test/test_callable.cpp)
add_test(NAME  test_callable COMMAND test_callable)

add_test(NAME type_erasure_example_runs COMMAND type_erasure_example)
