#ifndef GRAPHICS_PLATFORM_GLFW_CALLBACKS_H
#define GRAPHICS_PLATFORM_GLFW_CALLBACKS_H

#include <graphics/platform/gl_includes.h>

namespace graphics::platform
{

	void glfw_cursor_pos_callback(GLFWwindow* window, double xpos, double ypos);

	/// <summary>
	/// Callback invoked by GLFW to report an error.
	/// </summary>
	/// <param name="error_code">Numeric code identifying the error.</param>
	/// <param name="description">Null-terminated string containing a human-readable description of the error.</param>
	void glfw_error_callback(int error_code, const char* description);

	/// <summary>
	/// Callback invoked when a GLFW window's framebuffer is resized; typically used to update the OpenGL viewport or other rendering state.
	/// </summary>
	/// <param name="pWindow">Pointer to the GLFWwindow whose framebuffer size changed.</param>
	/// <param name="w">New framebuffer width in pixels.</param>
	/// <param name="h">New framebuffer height in pixels.</param>
	void glfw_framebuffer_size_callback(GLFWwindow* pWindow, int w, int h);

	void glfw_key_callback(GLFWwindow* win, int key, int scancode, int action, int mods);

	void glfw_mouse_button_callback(GLFWwindow* win, int button, int action, int mods);

	void glfw_scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

}

#endif // GRAPHICS_PLATFORM_GLFW_CALLBACKS_H
