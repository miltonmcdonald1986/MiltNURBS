#include <graphics/ui/entity_list.h>

#include <string>

#include <imgui.h>

#include <graphics/components/tags.h>
#include <graphics/engine/app_data.h>
#include <graphics/scene/scene.h>

namespace graphics::ui
{

    void draw_entity_list(engine::AppData* p_data)
    {
        if (!p_data)
            return;

        scene::Scene* p_scene = p_data->p_active_scene;
        if (!p_scene)
            return;

        entt::registry& reg = p_scene->reg;

        ImGui::Begin("Entities");

        // Collect all entities into a vector
        std::vector<entt::entity> entities;
        auto view = reg.view<entt::entity>();
        for (auto entity : view)
            entities.push_back(entity);

        // Sort by numeric ID ascending
        std::sort(entities.begin(), entities.end(),
            [](entt::entity a, entt::entity b) {
                return (uint32_t)a < (uint32_t)b;
            });

        // Draw sorted list
        for (auto entity : entities)
        {
            uint32_t id = (uint32_t)entity;
            std::string label = "Entity " + std::to_string(id);

            bool is_selected = reg.all_of<components::Selected>(entity);

            if (ImGui::Selectable(label.c_str(), is_selected))
            {
                // Clear previous selection
                for (auto e : reg.view<components::Selected>())
                    reg.remove<components::Selected>(e);

                // Select this one
                reg.emplace<components::Selected>(entity);
            }
        }

        ImGui::End();
    }

}
