find_package(OpenGL REQUIRED)

# ----------------------------------------
# Collect headers and sources automatically
# ----------------------------------------

# Public headers (installed)
file(GLOB_RECURSE GRAPHICS_HEADERS
    CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/include/graphics/*.h"
)

# Private sources
file(GLOB_RECURSE GRAPHICS_SOURCES
    CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

# ----------------------------------------
# Library target
# ----------------------------------------

add_library(graphics STATIC)

target_sources(graphics
    PRIVATE
        ${GRAPHICS_SOURCES}
    PUBLIC
        FILE_SET HEADERS
        BASE_DIRS include
        FILES ${GRAPHICS_HEADERS}
)

target_include_directories(graphics
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${stb_SOURCE_DIR}
)

# ----------------------------------------
# Compiler warnings
# ----------------------------------------

if(MSVC)
    target_compile_options(graphics PRIVATE /W4 /WX)
else()
    target_compile_options(graphics PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

# ----------------------------------------
# Linking
# ----------------------------------------

target_link_libraries(graphics
    PUBLIC
        EnTT::EnTT
        glfw
        gl3w
        imgui
        OpenGL::GL
)
