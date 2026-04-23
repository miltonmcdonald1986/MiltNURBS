#ifndef GRAPHICS_APP_WINDOW_CONFIG_H
#define GRAPHICS_APP_WINDOW_CONFIG_H

#include <GLFW/glfw3.h>

namespace graphics::app::window_config
{
	
    struct WindowConfig
    {
        const char* title{ "OpenGL Window" };
        GLFWmonitor* pMonitor{ nullptr };
        GLFWwindow* pShare{ nullptr };
    };

}

#endif // GRAPHICS_APP_WINDOW_CONFIG_H
