#include <graphics/app/app.h>
#include <graphics/app/app_loop.h>
#include <graphics/components/mesh_gl.h>
#include <graphics/components/shader.h>
#include <graphics/components/texture.h>
#include <graphics/factories/mesh_factories.h>
#include <graphics/factories/texture_factories.h>
#include <graphics/factories/shader_factories.h>
#include <graphics/systems/animation.h>
#include <graphics/ui/widgets.h>

using graphics::app::app::App;
using graphics::app::app_loop::run_app;
using graphics::components::mesh_gl::MeshGL;
using graphics::components::shader::Shader;
using graphics::components::texture::Texture;
using graphics::factories::mesh_factories::create_textured_triangle_mesh;
using graphics::factories::texture_factories::create_texture_from_file;
using graphics::factories::shader_factories::create_textured_shader;
using graphics::systems::animation::update_flash;
using graphics::ui::widgets::draw_flash_widget;
using graphics::ui::widgets::draw_per_entity_color_widget;
using graphics::ui::widgets::draw_render_settings_widget;

std::expected<void, std::string> init(App& app)
{
    entt::entity e = app.reg.create();

    if (auto mesh_result = create_textured_triangle_mesh())
        app.reg.emplace<MeshGL>(e, *mesh_result);
    else
        return std::unexpected("Failed to create mesh");

    if (auto texture_result = create_texture_from_file(R"(C:\Users\milto\Downloads\wall.jpg)"))
        app.reg.emplace<Texture>(e, *texture_result);
    else
        return std::unexpected("Failed to create texture");

    if (auto shader_result = create_textured_shader())
        app.reg.emplace<Shader>(e, *shader_result);
	else
        return std::unexpected("Failed to create shader");

    return {};
}

std::expected<void, std::string> update(App& app)
{
	// draw widgets before updating animations so that UI changes are reflected immediately in the same frame
    draw_render_settings_widget(app);
    draw_per_entity_color_widget(app);
    draw_flash_widget(app);

	update_flash(app.reg, app.delta_time);

    return {};
}

int main(void)
{
    return run_app(init, update);
}
