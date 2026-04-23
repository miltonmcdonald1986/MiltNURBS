#ifndef GRAPHICS_FACTORIES_SHADER_FACTOIES_H
#define GRAPHICS_FACTORIES_SHADER_FACTOIES_H

#include <expected>
#include <filesystem>
#include <string>

#include <GL/gl3w.h>

namespace graphics::factories::shader_factories
{

	/// <summary>
	/// Creates a basic OpenGL shader that renders geometry with a fixed orange-brown color.
	/// 
	/// The shader expects vertex data to include 3D positions (x, y, z coordinates) as input.
	/// These positions are passed directly to determine where on screen each vertex will appear.
	/// All rendered surfaces will be colored with an orange-brown tint (RGB: 1.0, 0.5, 0.2)
	/// with full opacity, creating a flat, unshaded appearance.
	/// </summary>
	/// <returns>On success, contains a GLuint representing the created shader handle. On failure, contains a std::string with an error message.</returns>
	std::expected<GLuint, std::string> create_basic_shader();

	/// <summary>
	/// Creates an OpenGL shader program that renders geometry using a user‑specified RGBA color.
	///
	/// This shader expects vertex data to provide 3D positions (x, y, z coordinates) as input.
	/// The vertex shader passes these positions directly to the clip‑space output, determining
	/// where each vertex appears on screen.
	///
	/// Unlike <c>create_basic_shader</c>, which uses a fixed built‑in color, this shader exposes
	/// a uniform named <c>u_color</c> (type: vec4). Applications can update this uniform at
	/// runtime to control the rendered color dynamically. This enables effects such as animated
	/// color transitions, UI‑driven color selection, or per‑draw customization.
	///
	/// The resulting appearance is flat and unshaded: all fragments are colored solely by the
	/// value assigned to <c>u_color</c>, with no lighting or texturing applied.
	/// </summary>
	/// <returns>
	/// On success, contains a <c>GLuint</c> representing the created shader program handle.
	/// On failure, contains a <c>std::string</c> describing the compilation or linking error.
	/// </returns>
	std::expected<GLuint, std::string> create_color_shader();

	/// <summary>
	/// Creates an OpenGL shader program by reading, compiling, and linking a vertex and fragment shader from files.
	/// </summary>
	/// <param name="vertex_path">Path to the vertex shader source file.</param>
	/// <param name="fragment_path">Path to the fragment shader source file.</param>
	/// <returns>std::expected containing the GLuint program handle on success, or a std::string error message on failure.</returns>
	std::expected<GLuint, std::string> create_shader_from_files(const std::filesystem::path& vertex_path, const std::filesystem::path& fragment_path);

	/// <summary>
	/// Creates a basic textured shader program.
	///
	/// The shader expects two vertex attributes:
	///   layout(location = 0): vec3 position
	///   layout(location = 1): vec2 uv
	///
	/// The vertex shader passes UV coordinates to the fragment shader,
	/// which samples from a 2D texture bound to texture unit 0.
	/// The fragment shader outputs the sampled texel color directly,
	/// with no lighting, tinting, or additional processing.
	///
	/// This shader is suitable for rendering textured geometry such as
	/// sprites, quads, UI elements, or any mesh with position + UV data.
	/// </summary>
	/// <returns>
	/// On success, contains the OpenGL program ID for the linked shader.
	/// On failure, contains a descriptive error message.
	/// </returns>
	std::expected<GLuint, std::string> create_textured_shader();

	/// <summary>
	/// Creates a simple vertex-color shader program.
	///
	/// The shader expects two vertex attributes:
	///   layout(location = 0): vec3 position
	///   layout(location = 1): vec3 color
	///
	/// The vertex shader forwards each vertex's color to the fragment shader,
	/// which outputs it unchanged. No transforms, uniforms, or lighting are
	/// applied. This shader is useful for unlit geometry, debugging mesh data,
	/// or rendering primitives with baked-in vertex colors.
	/// </summary>
	/// <returns>
	/// On success, contains the OpenGL program ID for the linked shader.
	/// On failure, contains a descriptive error message.
	/// </returns>
	std::expected<GLuint, std::string> create_vertex_color_shader();

}

#endif // GRAPHICS_FACTORIES_SHADER_FACTOIES_H
