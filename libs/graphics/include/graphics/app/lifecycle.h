#ifndef GRAPHICS_APP_LIFECYCLE_H
#define GRAPHICS_APP_LIFECYCLE_H

#include "app.h"

#include <expected>
#include <string>

namespace graphics::app::lifecycle
{

	std::expected<void, std::string> init_engine(app::App& app);
    void shutdown();

}

#endif // GRAPHICS_APP_LIFECYCLE_H
