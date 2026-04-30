#include <graphics/app/app.h>

#include <graphics/platform/glfw_callbacks.h>
#include <graphics/platform/window.h>

#include <print>

using graphics::app::app::App;
using graphics::platform::window::Window;

namespace graphics::platform::glfw_callbacks
{

    void glfw_cursor_pos_callback(GLFWwindow* win, double x, double y) {
        App* app = static_cast<App*>(glfwGetWindowUserPointer(win));
        auto& mouse = app->input.mouse;

        if (mouse.first_time) {
            mouse.x = x;
            mouse.y = y;
            mouse.first_time = false;
        }

        mouse.dx += x - mouse.x;
        mouse.dy += y - mouse.y;

        mouse.x = x;
        mouse.y = y;
    }

    void glfw_error_callback(int error_code, const char* description)
    {
        std::print("GLFW Error (code {}): {}\n", error_code, description);
    }

    void glfw_framebuffer_size_callback(GLFWwindow* pWindow, int w, int h)
    {
        App* p_app = static_cast<App*>(glfwGetWindowUserPointer(pWindow));
        if (!p_app)
            return;

        Window* p_window = p_app->p_window;
        if (!p_window)
            return;

        p_window->window_state.width = w;
        p_window->window_state.height = h;
        
        glViewport(0, 0, w, h);
    }

    void glfw_key_callback(GLFWwindow* win, int key, int /*scancode*/, int action, int mods) 
    {
        App* app = static_cast<App*>(glfwGetWindowUserPointer(win));
        auto& keys = app->input.keys;

        if (key >= 0 && key <= GLFW_KEY_LAST) {
            keys.down[key] = (action != GLFW_RELEASE);
        }

        // Update modifier state based on mods bitmask
        keys.down[GLFW_KEY_LEFT_SHIFT] = (mods & GLFW_MOD_SHIFT) != 0;
        keys.down[GLFW_KEY_RIGHT_SHIFT] = (mods & GLFW_MOD_SHIFT) != 0;

        keys.down[GLFW_KEY_LEFT_CONTROL] = (mods & GLFW_MOD_CONTROL) != 0;
        keys.down[GLFW_KEY_RIGHT_CONTROL] = (mods & GLFW_MOD_CONTROL) != 0;

        keys.down[GLFW_KEY_LEFT_ALT] = (mods & GLFW_MOD_ALT) != 0;
        keys.down[GLFW_KEY_RIGHT_ALT] = (mods & GLFW_MOD_ALT) != 0;
    }

    void glfw_mouse_button_callback(GLFWwindow* win, int button, int action, int mods) 
    {
        App* app = static_cast<App*>(glfwGetWindowUserPointer(win));
        
        auto& input = app->input;
        auto& mouse = input.mouse;
        auto& keys = input.keys;

        bool down = action == GLFW_PRESS;
        if (button == GLFW_MOUSE_BUTTON_LEFT)   mouse.left_is_down = down;
        if (button == GLFW_MOUSE_BUTTON_RIGHT)  mouse.right_is_down = down;
        if (button == GLFW_MOUSE_BUTTON_MIDDLE) mouse.middle_is_down = down;

        // Update modifier state based on mods bitmask
        keys.down[GLFW_KEY_LEFT_SHIFT] = (mods & GLFW_MOD_SHIFT) != 0;
        keys.down[GLFW_KEY_RIGHT_SHIFT] = (mods & GLFW_MOD_SHIFT) != 0;

        keys.down[GLFW_KEY_LEFT_CONTROL] = (mods & GLFW_MOD_CONTROL) != 0;
        keys.down[GLFW_KEY_RIGHT_CONTROL] = (mods & GLFW_MOD_CONTROL) != 0;

        keys.down[GLFW_KEY_LEFT_ALT] = (mods & GLFW_MOD_ALT) != 0;
        keys.down[GLFW_KEY_RIGHT_ALT] = (mods & GLFW_MOD_ALT) != 0;
    }

    void glfw_scroll_callback(GLFWwindow* win, double xoff, double yoff) 
    {
        App* app = static_cast<App*>(glfwGetWindowUserPointer(win));
        app->input.scroll.x += static_cast<float>(xoff);
        app->input.scroll.y += static_cast<float>(yoff);
    }

} // namespace graphics::glfw_callbacks
