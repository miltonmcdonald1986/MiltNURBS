#ifndef GRAPHICS_APP_APP_H
#define GRAPHICS_APP_APP_H

#include <entt/entt.hpp>

// Include these at the topmost level to avoid forcing downstream code to include them.
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

#include <graphics/app/gl_config.h>
#include <graphics/app/window_config.h>
#include <graphics/components/transform.h>
#include <graphics/platform/window_state.h>
#include <graphics/systems/gl_state.h>

namespace graphics::app::app
{

	struct App
	{
		double																			delta_time			{ 0. };
		double																			last_time			{ 0. };	
		entt::registry																	reg					{};
		graphics::app::gl_config::GLConfig												glConfig			{};
		graphics::systems::gl_state::GLState											glState				{};
		graphics::app::window_config::WindowConfig										winConfig			{};
		graphics::platform::window_state::WindowState									winState			{};
		std::unordered_map<entt::entity, graphics::components::transform::Transform>	initialTransforms	{};
	};

}

#endif // GRAPHICS_APP_APP_H
