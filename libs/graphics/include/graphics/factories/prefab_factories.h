#ifndef GRAPHICS_FACTORIES_PREFAB_FACTORIES_H
#define GRAPHICS_FACTORIES_PREFAB_FACTORIES_H

#include <expected>

#include <entt/entt.hpp>

#include <graphics/components/color.h>

namespace graphics::factories
{

	/// <summary>
	/// Creates an ECS entity representing a solid‑colored triangle.
	///
	/// This prefab:
	///   • creates a MeshGL using create_solid_color_triangle_mesh()
	///   • creates a flat‑color shader using create_flat_color_shader()
	///   • attaches MeshGL, Shader, and Color components to the entity
	///   • does not assign a Transform (caller may add one)
	///
	/// On success, returns an entt::entity containing MeshGL, Shader, and Color components.
	/// On failure, returns an error message describing the underlying issue.
	/// </summary>
	/// <param name="reg">The EnTT registry used to create and populate the entity.</param>
	/// <param name="color">The RGBA color to apply to the triangle.</param>
	/// <returns>
	/// std::expected containing:
	///   • entt::entity on success
	///   • std::string error message on failure
	/// </returns>
	std::expected<entt::entity, std::string> create_solid_color_triangle_ent(entt::registry& reg, const components::Color& color);


	/// <summary>
	/// Creates an ECS entity representing a rainbow-colored triangle.
	///
	/// This prefab:
	///   • creates a MeshGL using create_rainbow_triangle_mesh()
	///   • creates a vertex-color shader using create_vertex_color_shader()
	///   • attaches MeshGL and Shader components to the entity
	///   • does not assign a Transform (caller may add one)
	///
	/// On success, returns an entt::entity containing MeshGL and Shader components.
	/// On failure, returns an error message describing the underlying issue.
	/// </summary>
	/// <param name="reg">The EnTT registry used to create and populate the entity.</param>
	/// <returns>
	/// std::expected containing:
	///   • entt::entity on success
	///   • std::string error message on failure
	/// </returns>
	std::expected<entt::entity, std::string> create_rainbow_triangle_ent(entt::registry& reg);

}

#endif // GRAPHICS_FACTORIES_PREFAB_FACTORIES_H
