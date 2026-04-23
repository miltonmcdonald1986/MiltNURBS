#ifndef GRAPHICS_SYSTEMS_RENDER_H
#define GRAPHICS_SYSTEMS_RENDER_H

#include <expected>
#include <string>

#include <graphics/app/app.h>

namespace graphics::systems::render
{

    std::expected<void, std::string> render_system_update(app::app::App& app);

}

#endif // GRAPHICS_SYSTEMS_RENDER_H