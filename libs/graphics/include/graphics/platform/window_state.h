#ifndef GRAPHICS_PLATFORM_WINDOW_STATE_H
#define GRAPHICS_PLATFORM_WINDOW_STATE_H

#include <graphics/platform/gl_includes.h>

namespace graphics::platform
{

    struct WindowState
    {
        float aspect() const;

        GLFWwindow* pHandle{ nullptr };
        int height{ 768 };
        int width{ 1024 };
    };

}

#endif // GRAPHICS_PLATFORM_WINDOW_STATE_H
