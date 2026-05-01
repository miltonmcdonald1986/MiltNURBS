#include <graphics/platform/glfw_callbacks.h>

#include <print>

#include <graphics/engine/app_data.h>
#include <graphics/input/glfw_key.h>
#include <graphics/platform/window.h>

using graphics::engine::AppData;
using graphics::input::Key;
using graphics::input::translate_glfw_key;
using graphics::input::translate_glfw_mouse_button;
using graphics::platform::window::Window;

namespace graphics::platform::glfw_callbacks
{

    void glfw_cursor_pos_callback(GLFWwindow* p_window, double x, double y) 
    {
        static bool first_time = true;

        AppData* p_data = static_cast<AppData*>(glfwGetWindowUserPointer(p_window));
        if (!p_data)
            return;

        auto& mouse = p_data->input.mouse;

        if (first_time) 
        {
            mouse.x = x;
            mouse.y = y;
            first_time = false;
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

    void glfw_framebuffer_size_callback(GLFWwindow* p_window, int w, int h)
    {
        AppData* p_data = static_cast<AppData*>(glfwGetWindowUserPointer(p_window));
        if (!p_data)
            return;

        Window* p_win = p_data->p_window;
        if (!p_win)
            return;

        p_win->window_state.width = w;
        p_win->window_state.height = h;
        
        glViewport(0, 0, w, h);
    }

    void glfw_key_callback(GLFWwindow* p_window, int glfw_key, int /*scancode*/, int action, int mods)
    {
        AppData* p_data = static_cast<AppData*>(glfwGetWindowUserPointer(p_window));
        if (!p_data)
            return;

        auto& keys = p_data->input.keys;

        // 1. Translate GLFW key → engine Key enum
        Key key = translate_glfw_key(glfw_key);

        // 2. Update key state (ignore Unknown)
        if (key != Key::Unknown)
            keys[key] = (action != GLFW_RELEASE);

        // 3. Update modifier keys from mods bitmask
        keys[Key::LeftShift] = ((mods & GLFW_MOD_SHIFT) != 0);
        keys[Key::RightShift] = ((mods & GLFW_MOD_SHIFT) != 0);

        keys[Key::LeftControl] = ((mods & GLFW_MOD_CONTROL) != 0);
        keys[Key::RightControl] = ((mods & GLFW_MOD_CONTROL) != 0);

        keys[Key::LeftAlt] = ((mods & GLFW_MOD_ALT) != 0);
        keys[Key::RightAlt] = ((mods & GLFW_MOD_ALT) != 0);

        keys[Key::LeftSuper] = ((mods & GLFW_MOD_SUPER) != 0);
        keys[Key::RightSuper] = ((mods & GLFW_MOD_SUPER) != 0);
    }

    void glfw_mouse_button_callback(GLFWwindow* p_window, int button, int action, int mods)
    {
        AppData* p_data = static_cast<AppData*>(glfwGetWindowUserPointer(p_window));
        if (!p_data)
            return;

        auto& input = p_data->input;
        auto& mouse = input.mouse;
        auto& keys = input.keys;

        bool is_down = (action == GLFW_PRESS);

        // 1. Update mouse button state
        switch (button)
        {
        case GLFW_MOUSE_BUTTON_LEFT:   mouse.left_is_down = is_down; break;
        case GLFW_MOUSE_BUTTON_RIGHT:  mouse.right_is_down = is_down; break;
        case GLFW_MOUSE_BUTTON_MIDDLE: mouse.middle_is_down = is_down; break;
        }

        // 2. Update mouse buttons in KeyState too (optional but recommended)
        Key mouse_key = translate_glfw_mouse_button(button);
        if (mouse_key != Key::Unknown)
            keys[mouse_key] = is_down;

        // 3. Update modifier keys from mods bitmask
        keys[Key::LeftShift] = ((mods & GLFW_MOD_SHIFT) != 0);
        keys[Key::RightShift] = ((mods & GLFW_MOD_SHIFT) != 0);

        keys[Key::LeftControl] = ((mods & GLFW_MOD_CONTROL) != 0);
        keys[Key::RightControl] = ((mods & GLFW_MOD_CONTROL) != 0);

        keys[Key::LeftAlt] = ((mods & GLFW_MOD_ALT) != 0);
        keys[Key::RightAlt] = ((mods & GLFW_MOD_ALT) != 0);

        keys[Key::LeftSuper] = ((mods & GLFW_MOD_SUPER) != 0);
        keys[Key::RightSuper] = ((mods & GLFW_MOD_SUPER) != 0);
    }


    void glfw_scroll_callback(GLFWwindow* p_window, double xoff, double yoff) 
    {
        AppData* p_data = static_cast<AppData*>(glfwGetWindowUserPointer(p_window));
        if (!p_data)
            return;

        p_data->input.scroll.x += static_cast<float>(xoff);
        p_data->input.scroll.y += static_cast<float>(yoff);
    }

} // namespace graphics::glfw_callbacks
