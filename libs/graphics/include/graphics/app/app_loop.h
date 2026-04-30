#ifndef GRAPHICS_APP_APP_LOOP_H
#define GRAPHICS_APP_APP_LOOP_H

#include <expected>
#include <functional>
#include <string>

#include "app.h"

namespace graphics::app::app_loop
{

    int run_app(std::function<std::expected<void, std::string>(app::App&)> init_func, std::function<std::expected<void, std::string>(app::App&)> update_func);

    void run_app_loop(app::App& app, std::function<std::expected<void, std::string>(app::App&)> update_fn);

}

#endif // GRAPHICS_APP_APP_LOOP_H
