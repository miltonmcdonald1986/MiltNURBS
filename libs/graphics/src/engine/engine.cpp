#include <graphics/engine/engine.h>

#include <graphics/camera/camera_systems.h>
#include <graphics/engine/app_data.h>
#include <graphics/engine/error.h>
#include <graphics/engine/result.h>
#include <graphics/input/input_systems.h>
#include <graphics/platform/window.h>
#include <graphics/rendering/renderer.h>
#include <graphics/scene/scene.h>
#include <graphics/systems/animation.h>
#include <graphics/systems/ecs_observers.h>
#include <graphics/systems/transform.h>
#include <graphics/ui/imgui_layer.h>
#include <graphics/ui/inspector.h>

inline const std::string NAMESPACE = "graphics::engine";

namespace graphics::engine
{

    struct ImGuiEndFrameGuard
    {
        ~ImGuiEndFrameGuard()
        {
            ui::end_imgui_frame();
            p_window->swap_buffers();
        }

        platform::Window* p_window;
    };

    struct ImGuiTerminateGuard
    {
        ~ImGuiTerminateGuard()
        {
            ui::terminate_imgui();
        }
    };

    Status init(AppData& data)
    {
        platform::Window* p_window = data.p_window;
        if (!p_window)
            return std::unexpected(ERR("No window found"));

        rendering::Renderer* p_renderer = data.p_renderer;
        if (!p_renderer)
            return std::unexpected(ERR("No renderer found"));

        auto& gl_config = p_window->window_config.gl_config;
        if (auto result = p_window->init_glfw(&data, gl_config.version_major, gl_config.version_minor, gl_config.profile); !result)
            return std::unexpected(ERR("Failed to initialize GLFW"));

        int w, h;
        glfwGetFramebufferSize(p_window->window_state.pHandle, &w, &h);
        if (auto result = p_renderer->init(w, h); !result)
            return std::unexpected(ERR("Failed to initialize OpenGL state"));

        ui::init_imgui(p_window->window_state.pHandle);

        ui::register_inspectors();
        systems::register_transform_observers(&data);

        return {};
    }

    Status loop(AppData& data, UpdateFn update_fn)
    {
        platform::Window* p_window = data.p_window;
        if (!p_window)
            return {};

        while (!glfwWindowShouldClose(p_window->window_state.pHandle))
        {
            data.time.update();
            p_window->poll_events();

            ImGuiEndFrameGuard imgui_frame_ender{ p_window };
            ui::begin_imgui_frame();

            // USER SYSTEMS
            if (auto result = update_fn(&data); !result)
                return result;

            rendering::Renderer* p_renderer = data.p_renderer;
            if (!p_renderer)
                continue;

            scene::Scene* p_scene = data.p_active_scene;
            if (!p_scene)
                continue;

            // ENGINE SYSTEMS (authoritative)
            input::update_input_system(p_scene->reg);
            systems::update_transform_system(p_scene->reg);

            // ENGINE SYSTEMS (run-time effects)
            camera::update_camera_system(p_scene->reg, data.time.dt);
            systems::update_flash(p_scene->reg, data.time.dt);
            systems::update_shake(p_scene->reg, data.time.dt);
            systems::update_shake_once(p_scene->reg, data.time.dt);

            if (auto result = p_renderer->update(p_scene); !result)
                return result;

            data.input.reset_frame_accumulators();
        }

        return {};
    }
	
    Status run(InitFn init_fn, UpdateFn update_fn)
    {
        AppData data{};

        rendering::Renderer renderer{};
        data.p_renderer = &renderer;

        scene::Scene scene{};
        data.p_active_scene = &scene;

        platform::Window window{};
        data.p_window = &window;

        ImGuiTerminateGuard imgui_terminator{};

        if (Status result = init(data); !result)
            return result;

        if (auto result = init_fn(&data); !result)
            return result;

        if (auto result = loop(data, update_fn); !result)
            return result;

        return {};
    }

}
