#include <graphics/factories/prefab_factories.h>

#include <graphics/components/shader.h>
#include <graphics/factories/mesh_factories.h>
#include <graphics/factories/shader_factories.h>

namespace graphics::factories
{

    engine::Result<entt::entity> create_solid_color_triangle_ent(entt::registry& reg, const components::Color& color)
    {
        auto mesh_result = create_triangle_mesh();
		if (!mesh_result)
            return std::unexpected(mesh_result.error());

        components::MeshGL mesh = *mesh_result;

        auto color_shader_result = create_color_shader();
        if (!color_shader_result) 
            return std::unexpected(color_shader_result.error());
        
        GLuint color_shader = *color_shader_result;
        entt::entity ent_triangle = reg.create();
        reg.emplace<components::Color>(ent_triangle, color);
        reg.emplace<components::Shader>(ent_triangle, color_shader);
		reg.emplace<components::MeshGL>(ent_triangle, *mesh_result);

		return ent_triangle;
    }

	engine::Result<entt::entity> create_rainbow_triangle_ent(entt::registry& reg)
	{
        auto mesh_result = create_rainbow_triangle_mesh();
        if (!mesh_result)
            return std::unexpected(mesh_result.error());

        components::MeshGL mesh = *mesh_result;

        auto vertex_color_shader_result = create_vertex_color_shader();
        if (!vertex_color_shader_result) 
            return std::unexpected(vertex_color_shader_result.error());
        
        GLuint vertex_color_shader = *vertex_color_shader_result;

        entt::entity ent_rainbow_triangle = reg.create();
        reg.emplace<components::Shader>(ent_rainbow_triangle, vertex_color_shader);
        reg.emplace<components::MeshGL>(ent_rainbow_triangle, *mesh_result);

		return ent_rainbow_triangle;
	}

}
