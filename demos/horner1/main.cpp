#include <graphics/app/app.h>
#include <graphics/app/app_loop.h>
#include <graphics/components/color.h>
#include <graphics/components/flash.h>
#include <graphics/components/mesh_gl.h>
#include <graphics/components/parent.h>
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
using graphics::components::parent::Parent;
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

entt::entity parent;
entt::entity child;

std::expected<void, std::string> init(App& app)
{
    entt::registry& reg = app.reg;

    parent = reg.create();
    child = reg.create();

    Transform t{};
    t.rotation = glm::vec3(0.f);   // start with no rotation
    t.scale = glm::vec3(0.2f);   // shrink
    t.position = glm::vec3(0.f, 0.f, 0.f);
    reg.emplace<Transform>(parent, t);

    t.position = glm::vec3(0.5f, 0.f, 0.f);
    reg.emplace<Transform>(child, t);

    if (auto mesh_result = create_textured_quad_mesh())
    {
        reg.emplace<MeshGL>(parent, *mesh_result);
        reg.emplace<MeshGL>(child, *mesh_result);
    }
    else
        return std::unexpected("Failed to create quad mesh");

    if (auto shader_result = create_textured_color_mvp_shader())
    {
        reg.emplace<Shader>(parent, *shader_result);
        reg.emplace<Shader>(child, *shader_result);
    }
    else
        return std::unexpected("Failed to create MVP shader");

    if (auto tex_result = create_texture_from_file(R"(C:\Users\milto\Downloads\wall.jpg)"))
    {
        reg.emplace<Texture>(parent, *tex_result);
        reg.emplace<Texture>(child, *tex_result);
    }
    else
        return std::unexpected("Failed to load texture");

    reg.emplace<Shakeable>(parent);
    reg.emplace<Shakeable>(child);

    reg.emplace<Parent>(child, parent);

    return {};
}

std::expected<void, std::string> update(App& app)
{
    // Optional UI
    draw_entity_list(app);
    draw_inspector(app);

    auto& tParent = app.reg.get<Transform>(parent);
    tParent.rotation.z += 0.8f * static_cast<float>(app.delta_time);   // parent spin
    tParent.dirty = true;

    auto& tChild = app.reg.get<Transform>(child);
    OutputDebugStringA(std::format("child dirty = {}\n", tChild.dirty).c_str());

    return {};
}

int main(void)
{
    return run_app(init, update);
}
