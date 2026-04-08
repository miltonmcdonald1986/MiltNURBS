cmake_minimum_required(VERSION 3.25)

project(MiltNURBS
    VERSION 0.1.0
    DESCRIPTION "A modern C++ NURBS library"
    LANGUAGES CXX
)

# Set C++23 standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Generate compile_commands.json for IDE support
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Options
option(MiltNURBS_BUILD_TESTS "Build unit tests" ON)
option(MiltNURBS_BUILD_EXAMPLES "Build examples" OFF)

set(MiltNURBS_HEADERS
	include/miltnurbs/bernstein.h
    include/miltnurbs/bezier.h
    include/miltnurbs/bmp.h
	include/miltnurbs/horner.h
    include/miltnurbs/vector.h
)

# Create INTERFACE library target (header-only)
add_library(miltnurbs INTERFACE)

# Set include directories
target_include_directories(miltnurbs
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# Compiler warnings
if(MSVC)
    target_compile_options(miltnurbs INTERFACE /W4 /WX)
else()
    target_compile_options(miltnurbs INTERFACE -Wall -Wextra -Wpedantic -Werror)
endif()

target_sources(miltnurbs INTERFACE FILE_SET HEADERS BASE_DIRS include FILES ${MiltNURBS_HEADERS})

# Unit tests
if(MiltNURBS_BUILD_TESTS)
    enable_testing()
    
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.14.0
    )
    
    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
    
    add_subdirectory(tests)
endif()

# Examples
if(MiltNURBS_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
