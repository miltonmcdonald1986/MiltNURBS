#ifndef GRAPHICS_COMPONENTS_MESH_GL_H
#define GRAPHICS_COMPONENTS_MESH_GL_H

#include <graphics/platform/gl_includes.h>

namespace graphics::components
{

    struct MeshGL 
    {
        GLenum primitive = GL_TRIANGLES;
        GLuint vao = 0;
        GLuint vbo = 0;
        GLuint ebo = 0;
        GLsizei vertexCount = 0; // for non-indexed
        GLsizei indexCount = 0;  // for indexed
    };

}

#endif // GRAPHICS_COMPONENTS_MESH_GL_H
