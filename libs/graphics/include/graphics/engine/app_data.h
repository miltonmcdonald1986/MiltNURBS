#ifndef GRAPHICS_ENGINE_APP_DATA_H
#define GRAPHICS_ENGINE_APP_DATA_H

#include <graphics/input/input_state.h>
#include <graphics/platform/time.h>

// Forward declarations
namespace graphics::platform::window { struct Window; }
namespace graphics::rendering::renderer { struct Renderer; }
namespace graphics::scene { struct Scene; }

namespace graphics::engine
{

	struct AppData
	{
		input::InputState input{};
		platform::time::Time time{};
		platform::window::Window* p_window{ nullptr };
		rendering::renderer::Renderer* p_renderer{ nullptr };
		scene::Scene* p_active_scene{ nullptr };
	};

}

#endif // GRAPHICS_ENGINE_APP_DATA_H
