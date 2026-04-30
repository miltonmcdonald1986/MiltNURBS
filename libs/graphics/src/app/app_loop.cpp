#include <graphics/app/app_loop.h>

#include <print>

#include <graphics/app/app_guard.h>
#include <graphics/app/lifecycle.h>
#include <graphics/platform/window.h>
#include <graphics/rendering/renderer.h>
#include <graphics/scene/scene.h>
#include <graphics/systems/animation.h>
#include <graphics/systems/camera.h>
#include <graphics/systems/color.h>
#include <graphics/systems/transform.h>
#include <graphics/ui/imgui_layer.h>

using graphics::app::app::App;
using graphics::app::app_guard::AppGuard;
using graphics::app::lifecycle::init_engine;
using graphics::platform::window::Window;
using graphics::rendering::renderer::Renderer;
using graphics::scene::Scene;
using graphics::systems::animation::update_flash;
using graphics::systems::animation::update_shake;
using graphics::systems::animation::update_shake_once;
using graphics::systems::camera::update_camera_system;
using graphics::systems::color::update_color_no_flash;
using graphics::systems::transform::update_transform_dependents;
using graphics::systems::transform::update_transform_system;
using graphics::ui::imgui_layer::begin_imgui_frame;
using graphics::ui::imgui_layer::end_imgui_frame;

namespace graphics::app::app_loop
{

    int run_app(std::function<std::expected<void, std::string>(App&)> init_func, std::function<std::expected<void, std::string>(App&)> update_func)
    {
        App app{};

        Renderer renderer{};
        app.p_renderer = &renderer;

        Scene scene{};
        app.p_active_scene = &scene;

        Window window{};
        app.p_window = &window;

        if (auto result = init_engine(app); !result)
        {
            std::print("Failed to initialize engine: {}\n", result.error());
            return -1;
        }

        // RAII guard: runs Shutdown(app) when this scope ends.
        AppGuard guard{ app };

        if (auto result = init_func(app); !result)
        {
            std::print("Initialization error: {}\n", result.error());
            return 1;
        }

        run_app_loop(app, update_func);

        return 0;
    }

    void run_app_loop(App& app, std::function<std::expected<void, std::string>(App&)> update_fn)
    {
        Window* p_window = app.p_window;
        if (!p_window)
            return;

        while (!glfwWindowShouldClose(p_window->window_state.pHandle))
        {
            app.time.update();
            p_window->poll_events();
            begin_imgui_frame();

			// USER SYSTEMS
            if (auto result = update_fn(app); !result)
                std::print("Update error: {}\n", result.error());

            Renderer* p_renderer = app.p_renderer;
            if (!p_renderer)
                continue;

            Scene* p_scene = app.p_active_scene;
            if (!p_scene)
                continue;

            // ENGINE SYSTEMS (authoritative)
            update_transform_system(p_scene->reg);

			// ENGINE SYSTEMS (run-time effects)
            update_camera_system(p_scene->reg);
            update_color_no_flash(p_scene->reg);
            update_flash(p_scene->reg, app.time.delta_time);
			update_shake(p_scene->reg, app.time.delta_time);
			update_shake_once(p_scene->reg, app.time.delta_time);

            if (auto result = p_renderer->update(p_scene); !result)
                std::print("Render error: {}\n", result.error());

            app.input.reset_frame_accumulators();

            end_imgui_frame();
            p_window->swap_buffers();
        }
    }

}
