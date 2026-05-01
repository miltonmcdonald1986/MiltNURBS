#ifndef GRAPHICS_PLATFORM_WINDOW_STATE_H
#define GRAPHICS_PLATFORM_WINDOW_STATE_H

#include <graphics/platform/gl_includes.h>

namespace graphics::platform::window_state
{

    struct WindowState
    {
        float aspect() const
        {
            return float(width) / float(height);
        }

        GLFWwindow* pHandle{ nullptr };
        int height{ 768 };
        int width{ 1024 };
    };

}

#endif // GRAPHICS_PLATFORM_WINDOW_STATE_H
