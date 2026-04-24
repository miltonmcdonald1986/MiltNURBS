#include <graphics/app/app.h>
#include <graphics/app/app_loop.h>
#include <graphics/components/color.h>
#include <graphics/components/flash.h>
#include <graphics/components/mesh_gl.h>
#include <graphics/components/shader.h>
#include <graphics/components/shake.h>
#include <graphics/components/tags.h>
#include <graphics/components/texture.h>
#include <graphics/components/transform.h>
#include <graphics/factories/mesh_factories.h>
#include <graphics/factories/texture_factories.h>
#include <graphics/factories/shader_factories.h>
#include <graphics/systems/animation.h>
#include <graphics/ui/entity_list.h>
#include <graphics/ui/inspector.h>
#include <graphics/ui/widgets.h>

using graphics::app::app::App;
using graphics::app::app_loop::run_app;
using graphics::components::color::Color;
using graphics::components::flash::Flash;
using graphics::components::mesh_gl::MeshGL;
using graphics::components::shader::Shader;
using graphics::components::shake::Shake;
using graphics::components::shake::ShakeOnce;
using graphics::components::tags::Shakeable;
using graphics::components::texture::Texture;
using graphics::components::transform::Transform;
using graphics::factories::mesh_factories::create_textured_quad_mesh;
using graphics::factories::mesh_factories::create_textured_triangle_mesh;
using graphics::factories::texture_factories::create_texture_from_file;
using graphics::factories::shader_factories::create_textured_color_mvp_shader;
using graphics::factories::shader_factories::create_textured_mvp_shader;
using graphics::systems::animation::update_flash;
using graphics::systems::animation::update_shake_once;
using graphics::ui::entity_list::draw_entity_list;
using graphics::ui::inspector::draw_inspector;
using graphics::ui::widgets::draw_flash_widget;
using graphics::ui::widgets::draw_shake_widget;
using graphics::ui::widgets::draw_shake_once_widget;
using graphics::ui::widgets::draw_per_entity_color_widget;
using graphics::ui::widgets::draw_render_settings_widget;

std::expected<void, std::string> init(App& app)
{
    entt::entity left = app.reg.create();
	entt::entity middle = app.reg.create();
	entt::entity right = app.reg.create();

    // --- Mesh: textured quad ---
    if (auto mesh_result = create_textured_quad_mesh())
    {
        app.reg.emplace<MeshGL>(left, *mesh_result);
		app.reg.emplace<MeshGL>(middle, *mesh_result);
		app.reg.emplace<MeshGL>(right, *mesh_result);
    }
    else
        return std::unexpected("Failed to create quad mesh");

    // --- Texture ---
    if (auto tex_result = create_texture_from_file(R"(C:\Users\milto\Downloads\wall.jpg)"))
    {
        app.reg.emplace<Texture>(left, *tex_result);
		app.reg.emplace<Texture>(middle, *tex_result);
		app.reg.emplace<Texture>(right, *tex_result);
    }
    else
        return std::unexpected("Failed to load texture");

    // --- Shader: MVP textured shader ---
    if (auto shader_result = create_textured_color_mvp_shader())
    {
        app.reg.emplace<Shader>(left, *shader_result);
		app.reg.emplace<Shader>(middle, *shader_result);
		app.reg.emplace<Shader>(right, *shader_result);
    }
    else
        return std::unexpected("Failed to create MVP shader");

    
    // --- Transform ---
    Transform t{};
    t.rotation = glm::vec3(0.f);   // start with no rotation
    t.scale = glm::vec3(0.25f);   // full size
    
    t.position = glm::vec3(-0.5f, 0.f, 0.f);
    app.reg.emplace<Transform>(left, t);

	t.position = glm::vec3(0.f, 0.f, 0.f);
	app.reg.emplace<Transform>(middle, t);

	t.position = glm::vec3(0.5f, 0.f, 0.f);
	app.reg.emplace<Transform>(right, t);

    // --- Color (required for flashing) ---
    Color c(1.f, 1.f, 1.f, 1.f);
    app.reg.emplace<Color>(left, c);
    app.reg.emplace<Color>(middle, c);
    app.reg.emplace<Color>(right, c);

    app.reg.emplace<Shakeable>(middle);
    app.reg.emplace<Shakeable>(right);

    app.reg.emplace<Shake>(middle, Shake{});

    //// --- Flash (animates the color tint) ---
    app.reg.emplace<Flash>(left, Flash{});

    return {};
}

std::expected<void, std::string> update(App& app)
{
    // Optional UI
    draw_entity_list(app);
    draw_inspector(app);

    return {};
}

int main(void)
{
    return run_app(init, update);
}
