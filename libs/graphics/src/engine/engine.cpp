#include <graphics/engine/engine.h>

#include <graphics/camera/camera_systems.h>
#include <graphics/engine/app_data.h>
#include <graphics/input/input_systems.h>
#include <graphics/platform/window.h>
#include <graphics/rendering/renderer.h>
#include <graphics/scene/scene.h>
#include <graphics/systems/animation.h>
#include <graphics/systems/color.h>
#include <graphics/systems/ecs_observers.h>
#include <graphics/systems/transform.h>
#include <graphics/ui/imgui_layer.h>
#include <graphics/ui/inspector.h>

using graphics::camera::update_camera_system;
using graphics::engine::AppData;
using graphics::input::update_input_system;
using graphics::platform::window::Window;
using graphics::rendering::renderer::Renderer;
using graphics::scene::Scene;
using graphics::systems::animation::update_flash;
using graphics::systems::animation::update_shake;
using graphics::systems::animation::update_shake_base_world;
using graphics::systems::animation::update_shake_once;
using graphics::systems::color::update_color_no_flash;
using graphics::systems::ecs_observers::register_transform_observers;
using graphics::systems::transform::update_transform_system;
using graphics::ui::imgui_layer::begin_imgui_frame;
using graphics::ui::imgui_layer::end_imgui_frame;
using graphics::ui::imgui_layer::init_imgui;
using graphics::ui::imgui_layer::terminate_imgui;
using graphics::ui::inspector::register_inspectors;

namespace
{

    struct ImGuiEndFrameGuard
    {
        ~ImGuiEndFrameGuard()
        {
            end_imgui_frame();
            p_window->swap_buffers();
        }

        Window* p_window;
    };

    struct ImGuiTerminateGuard
    {
        ~ImGuiTerminateGuard()
        {
            terminate_imgui();
        }
    };

    std::expected<void, std::string> init(AppData& data)
    {
        Window* p_window = data.p_window;
        if (!p_window)
            return std::unexpected("No window found");

        Renderer* p_renderer = data.p_renderer;
        if (!p_renderer)
            return std::unexpected("No renderer found");

        auto& gl_config = p_window->window_config.gl_config;
        if (auto result = p_window->init_glfw(&data, gl_config.version_major, gl_config.version_minor, gl_config.profile); !result)
            return std::unexpected(std::format("Failed to initialize GLFW: {}", result.error()));

        int w, h;
        glfwGetFramebufferSize(p_window->window_state.pHandle, &w, &h);
        if (auto result = p_renderer->init(w, h); !result)
            return std::unexpected(std::format("Failed to initialize OpenGL state: {}", result.error()));

        init_imgui(p_window->window_state.pHandle);

        register_inspectors();
        register_transform_observers(&data);

        return {};
    }

    std::expected<void, std::string> loop(AppData& data, graphics::engine::GraphicsUpdateFn update_fn)
    {
        Window* p_window = data.p_window;
        if (!p_window)
            return {};

        while (!glfwWindowShouldClose(p_window->window_state.pHandle))
        {
            data.time.update();
            p_window->poll_events();
            
            ImGuiEndFrameGuard imgui_frame_ender{ p_window };
            begin_imgui_frame();

            // USER SYSTEMS
            if (auto result = update_fn(&data); !result)
                return std::unexpected(std::format("Update error: {}\n", result.error()));

            Renderer* p_renderer = data.p_renderer;
            if (!p_renderer)
                continue;

            Scene* p_scene = data.p_active_scene;
            if (!p_scene)
                continue;

            // ENGINE SYSTEMS (authoritative)
            update_input_system(p_scene->reg);
            update_transform_system(p_scene->reg);

            // ENGINE SYSTEMS (run-time effects)
            update_camera_system(p_scene->reg, data.time.dt);
            update_color_no_flash(p_scene->reg);
            update_flash(p_scene->reg, data.time.dt);
            update_shake(p_scene->reg, data.time.dt);
            update_shake_once(p_scene->reg, data.time.dt);

            if (auto result = p_renderer->update(p_scene, p_window->window_state.aspect()); !result)
                return std::unexpected(std::format("Render error: {}", result.error()));

            data.input.reset_frame_accumulators();
        }

        return {};
    }

}

namespace graphics::engine
{
	
    std::expected<void, std::string> run(GraphicsInitFn init_fn, GraphicsUpdateFn update_fn)
    {
        AppData data{};

        Renderer renderer{};
        data.p_renderer = &renderer;

        Scene scene{};
        data.p_active_scene = &scene;

        Window window{};
        data.p_window = &window;

        ImGuiTerminateGuard imgui_terminator{};

        if (auto result = init(data); !result)
            return std::unexpected(std::format("Failed to initialize engine: {}", result.error()));

        if (auto result = init_fn(&data); !result)
            return std::unexpected(std::format("User init function failed: {}", result.error()));

        if (auto result = loop(data, update_fn); !result)
            return std::unexpected(std::format("Error in engine loop: {}", result.error()));

        return {};
    }

}
