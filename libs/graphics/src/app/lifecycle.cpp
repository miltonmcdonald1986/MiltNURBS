#include <graphics/app/lifecycle.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <graphics/platform/glfw_callbacks.h>
#include <graphics/platform/window.h>
#include <graphics/rendering/renderer.h>
#include <graphics/systems/ecs_observers.h>
#include <graphics/ui/imgui_layer.h>
#include <graphics/ui/inspector.h>

using graphics::app::app::App;
using graphics::platform::glfw_callbacks::glfw_cursor_pos_callback;
using graphics::platform::glfw_callbacks::glfw_error_callback;
using graphics::platform::glfw_callbacks::glfw_framebuffer_size_callback;
using graphics::platform::glfw_callbacks::glfw_key_callback;
using graphics::platform::glfw_callbacks::glfw_mouse_button_callback;
using graphics::platform::glfw_callbacks::glfw_scroll_callback;
using graphics::platform::window::Window;
using graphics::rendering::renderer::Renderer;
using graphics::systems::ecs_observers::register_transform_observers;
using graphics::ui::imgui_layer::init_imgui;
using graphics::ui::inspector::register_inspectors;

namespace graphics::app::lifecycle
{

    std::expected<void, std::string> init_engine(App& app)
    {
        Window* p_window = app.p_window;
        if (!p_window)
            return std::unexpected("No window found");

        Renderer* p_renderer = app.p_renderer;
        if (!p_renderer)
            return std::unexpected("No renderer found");

        auto& gl_config = p_window->window_config.gl_config;
        if (auto result = p_window->init_glfw(&app, gl_config.version_major, gl_config.version_minor, gl_config.profile); !result)
            return std::unexpected(std::format("Failed to initialize GLFW: {}", result.error()));

        int w, h;
        glfwGetFramebufferSize(p_window->window_state.pHandle, &w, &h);
        if (auto result = p_renderer->init(w, h); !result)
            return std::unexpected(std::format("Failed to initialize OpenGL state: {}", result.error()));

        init_imgui(p_window->window_state.pHandle);

        register_inspectors();
        register_transform_observers(app);

		return {};
    }

    void shutdown()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

}
