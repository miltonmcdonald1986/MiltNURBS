#ifndef GRAPHICS_SYSTEMS_GL_STATE_H
#define GRAPHICS_SYSTEMS_GL_STATE_H

namespace graphics::systems::gl_state
{

    struct GLState
    {
        bool        enable_depth_test   { true };
        bool        display_wireframe   { false };
        GLbitfield  clear_buffer_mask   { GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT };
        GLclampf    clear_color[4]      { 0.1f, 0.1f, 0.1f, 1.0f };
        GLenum      depth_func          { GL_LESS };
        GLenum      polygon_mode        { GL_FILL };
    };

}

#endif // GRAPHICS_SYSTEMS_GL_STATE_H
