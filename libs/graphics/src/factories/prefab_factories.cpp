#include <graphics/factories/prefab_factories.h>

#include <graphics/components/shader.h>
#include <graphics/factories/mesh_factories.h>
#include <graphics/factories/shader_factories.h>

namespace graphics::factories
{

    std::expected<entt::entity, std::string> create_solid_color_triangle_ent(entt::registry& reg, const components::Color& color)
    {
        auto meshResult = create_triangle_mesh();
		if (!meshResult)
            return std::unexpected(std::format("Failed to create mesh: {}\n", meshResult.error()));

        components::MeshGL mesh = *meshResult;

        auto color_shader_result = create_color_shader();
        if (!color_shader_result) 
            return std::unexpected(std::format("Failed to create shader: {}\n", color_shader_result.error()));
        
        GLuint color_shader = *color_shader_result;
        entt::entity ent_triangle = reg.create();
        reg.emplace<components::Color>(ent_triangle, color);
        reg.emplace<components::Shader>(ent_triangle, color_shader);
		reg.emplace<components::MeshGL>(ent_triangle, *meshResult);

		return ent_triangle;
    }

	std::expected<entt::entity, std::string> create_rainbow_triangle_ent(entt::registry& reg)
	{
        auto meshResult = create_rainbow_triangle_mesh();
        if (!meshResult)
            return std::unexpected(std::format("Failed to create mesh: {}\n", meshResult.error()));

        components::MeshGL mesh = *meshResult;

        auto vertex_color_shader_result = create_vertex_color_shader();
        if (!vertex_color_shader_result) 
            return std::unexpected(std::format("Failed to create shader: {}\n", vertex_color_shader_result.error()));
        
        GLuint vertex_color_shader = *vertex_color_shader_result;

        entt::entity ent_rainbow_triangle = reg.create();
        reg.emplace<components::Shader>(ent_rainbow_triangle, vertex_color_shader);
        reg.emplace<components::MeshGL>(ent_rainbow_triangle, *meshResult);

		return ent_rainbow_triangle;
	}

}
