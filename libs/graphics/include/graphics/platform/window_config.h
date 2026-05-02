#ifndef GRAPHICS_PLATFORM_WINDOW_CONFIG_H
#define GRAPHICS_PLATFORM_WINDOW_CONFIG_H

#include <graphics/platform/gl_config.h>
#include <graphics/platform/gl_includes.h>

namespace graphics::platform
{
	
    struct WindowConfig
    {
        const char* title{ "OpenGL Window" };
        platform::GLConfig gl_config{};
        GLFWmonitor* pMonitor{ nullptr };
        GLFWwindow* pShare{ nullptr };
    };

}

#endif // GRAPHICS_PLATFORM_WINDOW_CONFIG_H
