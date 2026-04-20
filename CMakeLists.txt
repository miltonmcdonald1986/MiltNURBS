cmake_minimum_required(VERSION 3.25)


project(MiltNURBS
    VERSION 0.1.0
    DESCRIPTION "A modern C++ NURBS library"
    LANGUAGES CXX C
)

# Set C++23 standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Generate compile_commands.json for IDE support
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Options
option(MiltNURBS_BUILD_TESTS "Build unit tests" ON)
option(MiltNURBS_BUILD_DEMOS "Build demo applications" ON)

# Build the library
add_subdirectory(miltnurbs)

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

# Demos
if(MiltNURBS_BUILD_DEMOS)
    include(FetchContent)
        
    # Fetch GLFW
    FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG 3.4
    )
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(glfw)
	
	# Fetch gl3w
	FetchContent_Declare(
		gl3w
		GIT_REPOSITORY https://github.com/skaslev/gl3w.git
		GIT_TAG master
	)
	FetchContent_MakeAvailable(gl3w)
    
    # Fetch ImGui
    FetchContent_Declare(
        imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
        GIT_TAG v1.90.4
    )
    FetchContent_MakeAvailable(imgui)
    
    # Create ImGui library target (ImGui doesn't provide CMakeLists.txt)
    add_library(imgui STATIC
        ${imgui_SOURCE_DIR}/imgui.cpp
        ${imgui_SOURCE_DIR}/imgui_demo.cpp
        ${imgui_SOURCE_DIR}/imgui_draw.cpp
        ${imgui_SOURCE_DIR}/imgui_tables.cpp
        ${imgui_SOURCE_DIR}/imgui_widgets.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
    )
    target_include_directories(imgui PUBLIC
        ${imgui_SOURCE_DIR}
        ${imgui_SOURCE_DIR}/backends
    )
    target_link_libraries(imgui PUBLIC glfw gl3w)
    
    # Find OpenGL
    find_package(OpenGL REQUIRED)
    target_link_libraries(imgui PUBLIC OpenGL::GL)
    
    add_subdirectory(demos)
endif()
