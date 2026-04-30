#ifndef GRAPHICS_PLATFORM_PLATFORM_H
#define GRAPHICS_PLATFORM_PLATFORM_H

#ifdef _WIN32
	#define NOMINMAX
#endif

#include <graphics/platform/gl_includes.h>

namespace graphics::platform::platform
{

	///// <summary>
	///// Polls and processes pending events from the system or event queue.
	///// </summary>
	//void poll_events();

	///// <summary>
	///// Swaps the front and back buffers for the specified GLFW window, presenting the rendered frame.
	///// </summary>
	///// <param name="pWindow">Pointer to the GLFWwindow whose buffers will be swapped. Must be a valid, non-null window and typically called on the thread that has the window's graphics context current.</param>
	//void swap_buffers(GLFWwindow* pWindow);

}

#endif // GRAPHICS_PLATFORM_PLATFORM_H
