#include <graphics/ui/widgets.h>

#include <string>

#include <imgui.h>

#include <graphics/components/color.h>
#include <graphics/components/flash.h>
#include <graphics/components/shake.h>
#include <graphics/components/tags.h>
#include <graphics/components/transform.h>

using graphics::app::app::App;
using graphics::components::color::Color;
using graphics::components::flash::Flash;
using graphics::components::shake::Shake;
using graphics::components::tags::Selected;
using graphics::components::tags::Shakeable;
using graphics::components::shake::ShakeOnce;
using graphics::components::transform::Transform;

namespace graphics::ui::widgets
{

    void draw_flash_widget(App& app)
    {
        ImGui::SetNextWindowSize(ImVec2(300, 0), ImGuiCond_FirstUseEver);
        ImGui::Begin("Flash Settings");
        auto view = app.reg.view<Flash>();
        for (auto [entity, flash] : view.each()) {
			float speed = static_cast<float>(flash.speed);
            std::string label = "entity " + std::to_string((uint32_t)entity);
            ImGui::SliderFloat(label.c_str(), &speed, 0.1f, 10.0f);
            flash.speed = speed;
        }
		ImGui::End();
    }

    void draw_per_entity_color_widget(App& app)
    {
        ImGui::SetNextWindowSize(ImVec2(300, 0), ImGuiCond_FirstUseEver);
        ImGui::Begin("Entity Colors");
        auto view = app.reg.view<Color>();
        for (auto [entity, color] : view.each()) {
            std::string label = "entity " + std::to_string((uint32_t)entity);
            ImGui::ColorEdit4(label.c_str(), color.base);
        }
        ImGui::End();
    }

	void draw_render_settings_widget(App& app)
	{
        ImGui::SetNextWindowSize(ImVec2(300, 0), ImGuiCond_FirstUseEver);
        ImGui::Begin("Render Settings");
        ImGui::Checkbox("Wireframe", &app.glState.display_wireframe);
        ImGui::ColorEdit3("Background", app.glState.clear_color);
        ImGui::End();
	}

    void draw_shake_widget(App& app)
    {
        ImGui::SetNextWindowSize(ImVec2(300, 0), ImGuiCond_FirstUseEver);
        ImGui::Begin("Shake Settings");

        auto view = app.reg.view<Shake>();
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

    void draw_shake_once_widget(App& app)
    {
        ImGui::SetNextWindowSize(ImVec2(300, 0), ImGuiCond_FirstUseEver);
        ImGui::Begin("Shake Once");

        auto view = app.reg.view<Transform, Shakeable>();

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
                ShakeOnce s{};
                s.duration = duration;
                s.time_left = duration;
                s.intensity = intensity;
                s.speed = speed;
                s.base_position = tform.position;

                app.reg.emplace_or_replace<ShakeOnce>(entity, s);
            }

            ImGui::Separator();
        }

        ImGui::End();
    }

}
