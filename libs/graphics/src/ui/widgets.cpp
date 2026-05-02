#include <graphics/ui/widgets.h>

#include <string>

#include <glm/gtc/type_ptr.hpp>

#include <imgui.h>

#include <graphics/components/color.h>
#include <graphics/components/flash.h>
#include <graphics/components/shake.h>
#include <graphics/components/tags.h>
#include <graphics/components/transform.h>
#include <graphics/engine/app_data.h>
#include <graphics/rendering/renderer.h>
#include <graphics/scene/scene.h>

namespace graphics::ui
{

    void draw_flash_widget(engine::AppData* p_data)
    {
        if (!p_data)
            return;

        scene::Scene* p_scene = p_data->p_active_scene;
        if (!p_scene)
            return;

        entt::registry& reg = p_scene->reg;

        ImGui::SetNextWindowSize(ImVec2(300, 0), ImGuiCond_FirstUseEver);
        ImGui::Begin("Flash Settings");
        auto view = reg.view<components::Flash>();
        for (auto [entity, flash_component] : view.each()) {
            std::string label = "entity " + std::to_string((uint32_t)entity);
            ImGui::SliderFloat(label.c_str(), &flash_component.speed, 0.1f, 10.0f);
        }
		ImGui::End();
    }

    void draw_per_entity_color_widget(engine::AppData* p_data)
    {
        if (!p_data)
            return;

        scene::Scene* p_scene = p_data->p_active_scene;
        if (!p_scene)
            return;

        entt::registry& reg = p_scene->reg;

        ImGui::SetNextWindowSize(ImVec2(300, 0), ImGuiCond_FirstUseEver);
        ImGui::Begin("Entity Colors");
        auto view = reg.view<components::Color>();
        for (auto [entity, color_component] : view.each()) {
            std::string label = "entity " + std::to_string((uint32_t)entity);
            ImGui::ColorEdit4(label.c_str(), glm::value_ptr(color_component.rgba));
        }
        ImGui::End();
    }

	void draw_render_settings_widget(engine::AppData* p_data)
	{
        if (!p_data)
            return;

        rendering::Renderer* p_renderer = p_data->p_renderer;
        if (!p_renderer)
            return;

        ImGui::SetNextWindowSize(ImVec2(300, 0), ImGuiCond_FirstUseEver);
        ImGui::Begin("Render Settings");
        ImGui::Checkbox("Wireframe", &p_renderer->gl_state.display_wireframe);
        ImGui::ColorEdit3("Background", p_renderer->gl_state.clear_color);
        ImGui::End();
	}

    void draw_shake_widget(engine::AppData* p_data)
    {
        if (!p_data)
            return;

        scene::Scene* p_scene = p_data->p_active_scene;
        if (!p_scene)
            return;

        entt::registry& reg = p_scene->reg;

        ImGui::SetNextWindowSize(ImVec2(300, 0), ImGuiCond_FirstUseEver);
        ImGui::Begin("Shake Settings");

        auto view = reg.view<components::Shake>();
        for (auto [entity, shake] : view.each())
        {
            std::string label = "entity " + std::to_string((uint32_t)entity);

            // Intensity slider
            float intensity = shake.intensity;
            ImGui::SliderFloat((label + " intensity").c_str(), &intensity, 0.0f, 0.2f);
            shake.intensity = intensity;

            // Speed slider
            float speed = shake.speed;
            ImGui::SliderFloat((label + " speed").c_str(), &speed, 1.0f, 200.0f);
            shake.speed = speed;
        }

        ImGui::End();
    }

    void draw_shake_once_widget(engine::AppData* p_data)
    {
        scene::Scene* p_scene = p_data->p_active_scene;
        if (!p_scene)
            return;

        entt::registry& reg = p_scene->reg;

        ImGui::SetNextWindowSize(ImVec2(300, 0), ImGuiCond_FirstUseEver);
        ImGui::Begin("Shake Once");

        auto view = reg.view<components::Transform, components::Shakeable>();

        for (auto [entity, tform] : view.each())
        {
            uint32_t id = (uint32_t)entity;
            std::string base = "entity " + std::to_string(id);

            // Per-entity adjustable parameters (shared for now)
            static float intensity = 0.05f;
            static float speed = 60.0f;
            static float duration = 0.15f;

            ImGui::SliderFloat((base + " intensity").c_str(), &intensity, 0.0f, 0.2f);
            ImGui::SliderFloat((base + " speed").c_str(), &speed, 1.0f, 200.0f);
            ImGui::SliderFloat((base + " duration").c_str(), &duration, 0.01f, 1.0f);

            if (ImGui::Button((base + " shake").c_str()))
            {
                components::ShakeOnce s{};
                s.duration = duration;
                s.time_left = duration;
                s.intensity = intensity;
                s.speed = speed;

                reg.emplace_or_replace<components::ShakeOnce>(entity, s);
            }

            ImGui::Separator();
        }

        ImGui::End();
    }

}
