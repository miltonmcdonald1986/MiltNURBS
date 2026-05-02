#ifndef GRAPHICS_PLATFORM_WINDOW_H
#define GRAPHICS_PLATFORM_WINDOW_H

#include <expected>
#include <string>

#include <graphics/platform/window_config.h>
#include <graphics/platform/window_state.h>

// Forward declarations
namespace graphics::engine { struct AppData; }

namespace graphics::platform
{

	struct Window
	{
		~Window();

		std::expected<void, std::string> init_glfw(engine::AppData* p_app, int gl_version_major, int gl_version_minor, int gl_profile);
		void poll_events();
		void swap_buffers();
		void terminate_glfw();
		
		WindowConfig window_config{};
		WindowState window_state{};
	};

}

#endif // GRAPHICS_PLATFORM_WINDOW_H
