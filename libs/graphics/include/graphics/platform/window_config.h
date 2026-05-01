#ifndef GRAPHICS_APP_WINDOW_CONFIG_H
#define GRAPHICS_APP_WINDOW_CONFIG_H

#include <graphics/platform/gl_config.h>
#include <graphics/platform/gl_includes.h>

namespace graphics::app::window_config
{
	
    struct WindowConfig
    {
        const char* title{ "OpenGL Window" };
        platform::GLConfig gl_config{};
        GLFWmonitor* pMonitor{ nullptr };
        GLFWwindow* pShare{ nullptr };
    };

}

#endif // GRAPHICS_APP_WINDOW_CONFIG_H
