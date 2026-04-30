#ifndef GRAPHICS_APP_APP_H
#define GRAPHICS_APP_APP_H

#include <graphics/app/input_state.h>
#include <graphics/platform/time.h>

// Forward declarations
namespace graphics::platform::window { struct Window; }
namespace graphics::rendering::renderer { struct Renderer; }
namespace graphics::scene { struct Scene; }

namespace graphics::app::app
{

	struct App
	{
		input_state::InputState input{};
		platform::time::Time time{};
		platform::window::Window* p_window{ nullptr };
		rendering::renderer::Renderer* p_renderer{ nullptr };
		scene::Scene* p_active_scene{ nullptr };
	};

}

#endif // GRAPHICS_APP_APP_H
