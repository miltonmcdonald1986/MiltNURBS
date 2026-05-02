#include <graphics/ui/inspector.h>

#include <imgui.h>

#include <entt/entt.hpp>

#include <glm/gtc/type_ptr.hpp>

#include <graphics/components/color.h>
#include <graphics/components/flash.h>
#include <graphics/components/tags.h>
#include <graphics/components/shake.h>
#include <graphics/components/transform.h>
#include <graphics/engine/app_data.h>
#include <graphics/scene/scene.h>
#include <graphics/systems/ecs_observers.h>

namespace graphics::ui
{

    // ------------------------------------------------------------
    // Global inspector registry
    // ------------------------------------------------------------
    std::vector<std::pair<const char*, InspectorFn> > inspectors;

    // ------------------------------------------------------------
    // Component Inspectors
    // ------------------------------------------------------------

    void draw_color_inspector(entt::registry& reg, entt::entity e)
    {
        using components::Color;

        if (Color* color = reg.try_get<Color>(e))
        {
            if (ImGui::CollapsingHeader("Color"))
            {
                ImGui::ColorEdit4("Base", glm::value_ptr(color->rgba));
            }
        }
    }

    void draw_flash_inspector(entt::registry& reg, entt::entity e)
    {
        if (components::Flash* flash = reg.try_get<components::Flash>(e))
        {
            if (ImGui::CollapsingHeader("Flash"))
            {
                ImGui::SliderFloat("Speed", &flash->speed, 0.1f, 10.0f);
                ImGui::Text("t = %.3f", flash->t);
            }
        }
    }

    void draw_shake_inspector(entt::registry& reg, entt::entity e)
    {
        if (!reg.any_of<components::Shakeable>(e))
            return;

        components::Shake* shake = reg.try_get<components::Shake>(e);
        components::ShakeOnce* shakeOnce = reg.try_get<components::ShakeOnce>(e);
        components::Transform& transform = reg.get<components::Transform>(e);

        if (ImGui::CollapsingHeader("Shake", ImGuiTreeNodeFlags_DefaultOpen))
        {
            // ============================================================
            // Shared Intensity + Speed sliders
            // ============================================================
            ImGui::Text("Shake Parameters");
            ImGui::Separator();

            float intensity = shake ? shake->intensity :
                shakeOnce ? shakeOnce->intensity :
                0.05f;

            float speed = shake ? shake->speed :
                shakeOnce ? shakeOnce->speed :
                60.0f;

            if (ImGui::SliderFloat("Intensity", &intensity, 0.0f, 1.0f, "%.3f"))
            {
                if (shake)      shake->intensity = intensity;
                if (shakeOnce)  shakeOnce->intensity = intensity;
            }

            if (ImGui::SliderFloat("Speed", &speed, 0.0f, 200.0f, "%.1f"))
            {
                if (shake)      shake->speed = speed;
                if (shakeOnce)  shakeOnce->speed = speed;
            }

            ImGui::Spacing();

            // ============================================================
            // Continuous Shake
            // ============================================================
            if (shake)
            {
                ImGui::Separator();
                ImGui::Text("Base World (anchor)");

                // Extract translation from base_world
                glm::vec3 bw_pos = glm::vec3(
                    shake->base_world[3][0],
                    shake->base_world[3][1],
                    shake->base_world[3][2]
                );

                ImGui::Text("base_world pos: (%.3f, %.3f, %.3f)",
                    bw_pos.x, bw_pos.y, bw_pos.z);

                ImGui::Separator();

                ImGui::Text("Continuous Shake");
                ImGui::Separator();
                ImGui::PushID("shake");

                if (ImGui::Button("Remove Continuous Shake"))
                {
                    // Snap back to base (TransformSystem will recompute world)
                    transform.dirty = true;
                    reg.remove<components::Shake>(e);
                }

                ImGui::PopID();
                ImGui::Spacing();

                // IMPORTANT:
                // Do NOT show Shake Once UI when continuous shake is active.
                return;
            }
            else
            {
                if (ImGui::Button("Add Continuous Shake"))
                {
                    auto& s = reg.emplace<components::Shake>(e);
                    s.intensity = intensity;
                    s.speed = speed;
                }
                ImGui::Spacing();
            }

            // ============================================================
            // Shake Once (only when NOT shaking continuously)
            // ============================================================
            ImGui::Text("Shake Once");
            ImGui::Separator();
            ImGui::PushID("shakeonce");

            if (shakeOnce)
            {
                ImGui::SliderFloat("Duration", &shakeOnce->duration, 0.01f, 1.0f, "%.3f");

                if (ImGui::Button("Cancel Shake Once"))
                {
                    transform.dirty = true;
                    reg.remove<components::ShakeOnce>(e);
                }
            }
            else
            {
                static float onceDuration = 0.15f;

                ImGui::SliderFloat("Duration", &onceDuration, 0.01f, 1.0f, "%.3f");

                if (ImGui::Button("Trigger Shake Once"))
                {
                    auto& s = reg.emplace_or_replace<components::ShakeOnce>(e);
                    s.intensity = intensity;
                    s.duration = onceDuration;
                    s.time_left = onceDuration;
                    s.speed = speed;
                }
            }

            ImGui::PopID();
        }
    }

    void draw_transform_inspector(entt::registry& reg, entt::entity e)
    {
        components::Transform* transform = reg.try_get<components::Transform>(e);
        if (!transform)
            return;

        engine::AppData* p_data = systems::get_app(reg);

        scene::Scene* p_scene = p_data->p_active_scene;
        if (!p_scene)
            return;

        auto& initial_transforms = p_scene->initial_transforms;

        // Look up initial transform (if it exists)
        auto it = initial_transforms.find(e);
        const components::Transform* initial = (it != initial_transforms.end()) ? &it->second : nullptr;

        if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
        {
            // ============================================================
            // Position
            // ============================================================
            ImGui::Text("Position");
            ImGui::SameLine(120);
            ImGui::PushID("pos");

            // Determine what to display
            glm::vec3 displayPos;

            if (auto* shake = reg.try_get<components::Shake>(e))
            {
                displayPos = glm::vec3(
                    shake->base_world[3][0],
                    shake->base_world[3][1],
                    shake->base_world[3][2]
                );
            }
            else if (auto* shakeOnce = reg.try_get<components::ShakeOnce>(e))
            {
                displayPos = glm::vec3(
                    shakeOnce->base_world[3][0],
                    shakeOnce->base_world[3][1],
                    shakeOnce->base_world[3][2]
                );
            }
            else
            {
                displayPos = transform->get_position();
            }

            // Editable field
            if (ImGui::InputFloat3("##pos", &displayPos.x, "%.3f"))
                transform->set_position(displayPos);

            ImGui::SameLine();
            if (ImGui::Button("Reset") && initial)
                transform->set_position(initial->get_position());

            ImGui::PopID();

            // ============================================================
            // Rotation (degrees UI, radians internal)
            // ============================================================
            glm::vec3 rotDeg = glm::degrees(transform->get_rotation());

            ImGui::Text("Rotation");
            ImGui::SameLine(120);
            ImGui::PushID("rot");

            if (ImGui::InputFloat3("##rot", &rotDeg.x, "%.1f"))
                transform->set_rotation(glm::radians(rotDeg));

            ImGui::SameLine();
            if (ImGui::Button("Reset") && initial)
                transform->set_rotation(initial->get_rotation());

            ImGui::PopID();

            // ============================================================
            // Scale
            // ============================================================
            ImGui::Text("Scale");
            ImGui::SameLine(120);
            ImGui::PushID("scale");

            auto scale = transform->get_scale();
            if (ImGui::InputFloat3("##scale", &scale.x, "%.3f"))
                transform->set_scale(scale);

            ImGui::SameLine();
            if (ImGui::Button("Reset") && initial)
                transform->set_scale(initial->get_scale());

            ImGui::PopID();
        }
    }

    // ------------------------------------------------------------
    // Register all inspectors
    // ------------------------------------------------------------
    void register_inspectors()
    {
        inspectors.push_back({ "Color", draw_color_inspector });
        inspectors.push_back({ "Flash", draw_flash_inspector });
        inspectors.push_back({ "Shake", draw_shake_inspector });
        inspectors.push_back({ "Transform", draw_transform_inspector });
    }

    // ------------------------------------------------------------
    // Draw Inspector Panel
    // ------------------------------------------------------------
    void draw_inspector(engine::AppData* p_data)
    {
        if (!p_data)
            return;

        scene::Scene* p_scene = p_data->p_active_scene;
        if (!p_scene)
            return;

        entt::registry& reg = p_scene->reg;

        ImGui::Begin("Inspector");

        auto view = reg.view<components::Selected>();
        if (view.empty())
        {
            ImGui::Text("No entity selected");
            ImGui::End();
            return;
        }

        entt::entity entity = *view.begin();

        for (auto& [name, fn] : inspectors)
        {
            fn(reg, entity);
        }

        ImGui::End();
    }

}
