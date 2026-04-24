#include <graphics/ui/inspector.h>

#include <imgui.h>

#include <entt/entt.hpp>

#include <graphics/app/app.h>
#include <graphics/components/color.h>
#include <graphics/components/flash.h>
#include <graphics/components/tags.h>
#include <graphics/components/shake.h>
#include <graphics/components/transform.h>
#include <graphics/systems/ecs_observers.h>

using graphics::app::app::App;
using graphics::components::color::Color;
using graphics::components::flash::Flash;
using graphics::components::shake::Shake;
using graphics::components::shake::ShakeOnce;
using graphics::components::tags::Selected;
using graphics::components::tags::Shakeable;
using graphics::components::transform::Transform;
using graphics::systems::ecs_observers::get_app;
using graphics::ui::inspector::InspectorFn;

namespace
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
        if (Color* color = reg.try_get<Color>(e))
        {
            if (ImGui::CollapsingHeader("Color"))
            {
                ImGui::ColorEdit4("Base", color->base);
            }
        }
    }

    void draw_flash_inspector(entt::registry& reg, entt::entity e)
    {
        if (Flash* flash = reg.try_get<Flash>(e))
        {
            if (ImGui::CollapsingHeader("Flash"))
            {
                float speed = static_cast<float>(flash->speed);
                if (ImGui::SliderFloat("Speed", &speed, 0.1f, 10.0f))
                    flash->speed = speed;

                 // Optional: show internal time for debugging
                 ImGui::Text("t = %.3f", flash->t);
            }
        }
    }

    void draw_shake_inspector(entt::registry& reg, entt::entity e)
    {
        if (!reg.any_of<Shakeable>(e))
            return;

        Shake* shake = reg.try_get<Shake>(e);
        ShakeOnce* shakeOnce = reg.try_get<ShakeOnce>(e);
        Transform& transform = reg.get<Transform>(e);

        if (ImGui::CollapsingHeader("Shake", ImGuiTreeNodeFlags_DefaultOpen))
        {
            // ============================================================
            // Shared Intensity + Speed sliders
            // ============================================================
            ImGui::Text("Shake Parameters");
            ImGui::Separator();

            // These values feed into whichever shake mode is active
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
                ImGui::Text("Continuous Shake");
                ImGui::Separator();
                ImGui::PushID("shake");

                if (ImGui::Button("Remove Continuous Shake"))
                {
                    // Snap back to base
                    transform.position = shake->base_position;
                    transform.dirty = true;

                    reg.remove<Shake>(e);
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
                    auto& s = reg.emplace<Shake>(e);
                    s.base_position = transform.position;
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
                // Duration is unique to ShakeOnce
                ImGui::SliderFloat("Duration", &shakeOnce->duration, 0.01f, 1.0f, "%.3f");

                if (ImGui::Button("Cancel Shake Once"))
                {
                    transform.position = shakeOnce->base_position;
                    transform.dirty = true;

                    reg.remove<ShakeOnce>(e);
                }
            }
            else
            {
                static float onceDuration = 0.15f;

                ImGui::SliderFloat("Duration", &onceDuration, 0.01f, 1.0f, "%.3f");

                if (ImGui::Button("Trigger Shake Once"))
                {
                    auto& s = reg.emplace_or_replace<ShakeOnce>(e);
                    s.intensity = intensity;
                    s.duration = onceDuration;
                    s.time_left = onceDuration;
                    s.speed = speed;
                    s.base_position = transform.position;
                }
            }

            ImGui::PopID();
        }
    }

    void draw_transform_inspector(entt::registry& reg, entt::entity e)
    {
        Transform* transform = reg.try_get<Transform>(e);
        if (!transform)
            return;

        App& app = get_app(reg);

        // Look up initial transform (if it exists)
        auto it = app.initialTransforms.find(e);
        const Transform* initial = (it != app.initialTransforms.end()) ? &it->second : nullptr;

        if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
        {
            // -------------------------
            // Position
            // -------------------------
            ImGui::Text("Position");
            ImGui::SameLine(120);
            ImGui::PushID("pos");

            Shake* shake = reg.try_get<Shake>(e);
            ShakeOnce* shake_once = reg.try_get<ShakeOnce>(e);
            auto displayPos = shake ? shake->base_position : shake_once ? shake_once->base_position : transform->position;
            if (ImGui::InputFloat3("##pos", &displayPos.x, "%.3f"))
            {
                transform->position = displayPos; // write back only on edit
                transform->dirty = true;
            }

            ImGui::SameLine();
            if (ImGui::Button("Reset") && initial)
            {
                transform->position = initial->position;
                transform->dirty = true;
            }

            ImGui::PopID();

            // -------------------------
            // Rotation (degrees UI, radians internal)
            // -------------------------
            glm::vec3 rotDeg = glm::degrees(transform->rotation);

            ImGui::Text("Rotation");
            ImGui::SameLine(120);
            ImGui::PushID("rot");

            if (ImGui::InputFloat3("##rot", &rotDeg.x, "%.1f"))
            {
                transform->rotation = glm::radians(rotDeg);
                transform->dirty = true;
            }

            ImGui::SameLine();
            if (ImGui::Button("Reset") && initial)
            {
                transform->rotation = initial->rotation;
                transform->dirty = true;
            }

            ImGui::PopID();

            // -------------------------
            // Scale
            // -------------------------
            ImGui::Text("Scale");
            ImGui::SameLine(120);
            ImGui::PushID("scale");

            if (ImGui::InputFloat3("##scale", &transform->scale.x, "%.3f"))
            {
                transform->dirty = true;
            }

            ImGui::SameLine();
            if (ImGui::Button("Reset") && initial)
            {
                transform->scale = initial->scale;
                transform->dirty = true;
            }

            ImGui::PopID();
        }
    }

}

namespace graphics::ui::inspector
{

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
    void draw_inspector(App& app)
    {
        ImGui::Begin("Inspector");

        auto view = app.reg.view<Selected>();
        if (view.empty())
        {
            ImGui::Text("No entity selected");
            ImGui::End();
            return;
        }

        entt::entity entity = *view.begin();

        for (auto& [name, fn] : inspectors)
        {
            fn(app.reg, entity);
        }

        ImGui::End();
    }

}
