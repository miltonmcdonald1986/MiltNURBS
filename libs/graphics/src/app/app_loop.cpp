#include <graphics/app/app_loop.h>

#include <print>

#include <graphics/app/app_guard.h>
#include <graphics/app/lifecycle.h>
#include <graphics/platform/platform.h>
#include <graphics/systems/animation.h>
#include <graphics/systems/color.h>
#include <graphics/systems/render.h>
#include <graphics/systems/transform.h>
#include <graphics/ui/imgui_layer.h>

using graphics::app::app::App;
using graphics::app::app_guard::AppGuard;
using graphics::app::lifecycle::init_engine;
using graphics::app::lifecycle::update_delta_time;
using graphics::platform::platform::poll_events;
using graphics::platform::platform::swap_buffers;
using graphics::systems::animation::update_flash;
using graphics::systems::animation::update_shake;
using graphics::systems::animation::update_shake_once;
using graphics::systems::color::update_color_no_flash;
using graphics::systems::render::render_system_update;
using graphics::systems::transform::update_transform_dependents;
using graphics::ui::imgui_layer::begin_imgui_frame;
using graphics::ui::imgui_layer::end_imgui_frame;

namespace graphics::app::app_loop
{

    int run_app(std::function<std::expected<void, std::string>(App&)> init_func, std::function<std::expected<void, std::string>(App&)> update_func)
    {
        App app{};
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
        auto* pWin = app.winState.pHandle;
        while (!glfwWindowShouldClose(pWin))
        {
            update_delta_time(app);
            poll_events();
            begin_imgui_frame();

			// USER SYSTEMS
            if (auto result = update_fn(app); !result)
                std::print("Update error: {}\n", result.error());

            // ENGINE SYSTEMS (authoritative)
            update_transform_dependents(app.reg);

			// ENGINE SYSTEMS (run-time effects)
            update_color_no_flash(app.reg);
            update_flash(app.reg, app.delta_time);
			update_shake(app.reg, app.delta_time);
			update_shake_once(app.reg, app.delta_time);

            if (!render_system_update(app))
                std::print("Render error: {}\n", render_system_update(app).error());

            end_imgui_frame();
            swap_buffers(pWin);
        }
    }

}
