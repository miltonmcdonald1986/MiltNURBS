#ifndef GRAPHICS_APP_LIFECYCLE_H
#define GRAPHICS_APP_LIFECYCLE_H

#include "app.h"

#include <expected>

namespace graphics::app::lifecycle
{

	std::expected<void, std::string> init_engine(app::App& app);
    std::expected<void, std::string> init_gl_context(app::App& app);
    std::expected<void, std::string> init_gl_state(app::App& app);
    std::expected<void, std::string> init_platform(app::App& app);
    void shutdown(app::App& app);
	void update_delta_time(app::App& app);

}

#endif // GRAPHICS_APP_LIFECYCLE_H
