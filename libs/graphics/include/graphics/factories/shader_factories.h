#ifndef GRAPHICS_FACTORIES_SHADER_FACTOIES_H
#define GRAPHICS_FACTORIES_SHADER_FACTOIES_H

#include <expected>
#include <filesystem>
#include <string>

#include <graphics/engine/result.h>
#include <graphics/platform/gl_includes.h>

namespace graphics::factories
{

	engine::Result<GLuint> create_basic_shader();
	engine::Result<GLuint> create_color_shader();
	engine::Result<GLuint> create_shader_from_files(const std::filesystem::path& vertex_path, const std::filesystem::path& fragment_path);
	engine::Result<GLuint> create_textured_color_mvp_shader();
	engine::Result<GLuint> create_textured_mvp_shader();
	engine::Result<GLuint> create_textured_shader();
	engine::Result<GLuint> create_vertex_color_shader();

}

#endif // GRAPHICS_FACTORIES_SHADER_FACTOIES_H
