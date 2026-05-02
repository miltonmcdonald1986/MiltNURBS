#include <graphics/platform/window.h>

#include <graphics/platform/glfw_callbacks.h>

namespace graphics::platform
{

    Window::~Window()
    {
        terminate_glfw();
    }

    std::expected<void, std::string> Window::init_glfw(engine::AppData* p_app, int gl_version_major, int gl_version_minor, int gl_profile)
    {
        glfwSetErrorCallback(glfw_error_callback);

        if (glfwInit() != GLFW_TRUE)
            return std::unexpected("Failed to initialize GLFW");

        // Set the OpenGL version and profile we want to use.
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, gl_version_major);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, gl_version_minor);
        glfwWindowHint(GLFW_OPENGL_PROFILE, gl_profile);
#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

        // Create a window.
        if (GLFWwindow* window = glfwCreateWindow(window_state.width, window_state.height, window_config.title, window_config.pMonitor, window_config.pShare))
            window_state.pHandle = window;
        else
            return std::unexpected("Failed to create GLFW window");

        glfwSetWindowUserPointer(window_state.pHandle, p_app);

        glfwSetCursorPosCallback(window_state.pHandle, glfw_cursor_pos_callback);
        glfwSetFramebufferSizeCallback(window_state.pHandle, glfw_framebuffer_size_callback);
        glfwSetKeyCallback(window_state.pHandle, glfw_key_callback);
        glfwSetMouseButtonCallback(window_state.pHandle, glfw_mouse_button_callback);
        glfwSetScrollCallback(window_state.pHandle, glfw_scroll_callback);

        // Make the OpenGL context of our window current on the calling thread.
        glfwMakeContextCurrent(window_state.pHandle);

        // Enable V-Sync
        glfwSwapInterval(1);

        // Initialize OpenGL function pointers using gl3w (or any other OpenGL loader).
        if (gl3wInit() != 0)
            return std::unexpected("Failed to initialize gl3w");

        return {};
    }

    void Window::poll_events()
    {
        glfwPollEvents();
    }

    void Window::swap_buffers()
    {
        glfwSwapBuffers(window_state.pHandle);
    }

    void Window::terminate_glfw()
    {
        glfwDestroyWindow(window_state.pHandle);
        window_state.pHandle = nullptr;

        glfwTerminate();
    }

}
