#include <graphics/ui/entity_list.h>

#include <string>

#include <imgui.h>

#include <graphics/components/tags.h>

using graphics::app::app::App;
using graphics::components::tags::Selected;

namespace graphics::ui::entity_list
{

    void draw_entity_list(App& app)
    {
        ImGui::Begin("Entities");

        // Collect all entities into a vector
        std::vector<entt::entity> entities;

        auto view = app.reg.view<entt::entity>();
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

            bool is_selected = app.reg.all_of<Selected>(entity);

            if (ImGui::Selectable(label.c_str(), is_selected))
            {
                // Clear previous selection
                for (auto e : app.reg.view<Selected>())
                    app.reg.remove<Selected>(e);

                // Select this one
                app.reg.emplace<Selected>(entity);
            }
        }

        ImGui::End();
    }

}
