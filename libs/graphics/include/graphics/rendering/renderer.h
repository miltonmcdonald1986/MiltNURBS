#ifndef GRAPHICS_RENDERING_RENDERER_H
#define GRAPHICS_RENDERING_RENDERER_H

#include <expected>
#include <string>

#include <graphics/app/gl_config.h>
#include <graphics/scene/scene.h>
#include <graphics/systems/gl_state.h>

namespace graphics::rendering::renderer
{

	struct Renderer
	{
		std::expected<void, std::string> init(int framebuffer_width, int framebuffer_height);
		std::expected<void, std::string> update(scene::Scene* p_scene);

		systems::gl_state::GLState gl_state;
	};

}

#endif // GRAPHICS_RENDERING_RENDERER_H
