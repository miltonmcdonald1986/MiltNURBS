#include <graphics/systems/transform.h>

#include <glm/gtc/matrix_transform.hpp>

#include <graphics/components/parent.h>
#include <graphics/components/shake.h>
#include <graphics/components/world_matrix.h>
#include <graphics/systems/animation.h>

namespace graphics::systems
{
	
    glm::mat4 compute_model_matrix(const components::Transform& t)
    {
        glm::mat4 m = glm::mat4(1.0f);

        m = glm::translate(m, t.get_position());

        auto rot = t.get_rotation();
        m = glm::rotate(m, rot.x, glm::vec3(1, 0, 0));
        m = glm::rotate(m, rot.y, glm::vec3(0, 1, 0));
        m = glm::rotate(m, rot.z, glm::vec3(0, 0, 1));

        m = glm::scale(m, t.get_scale());

        return m;
    }

    void update_transform_dependents(entt::registry& reg)
    {
        auto view = reg.view<components::Transform, components::WorldMatrix>();

        for (auto [e, transform, world_matrix] : view.each())
        {
            update_shake_base_world(reg, e, world_matrix.value);
        }
    }

    void update_transform_system(entt::registry& reg)
    {
        // ------------------------------------------------------------
        // Build child lists (Parent → children)
        // ------------------------------------------------------------
        std::unordered_map<entt::entity, std::vector<entt::entity>> children;

        auto parentView = reg.view<components::Parent>();
        for (auto [e, p] : parentView.each())
        {
            children[p.parent].push_back(e);
        }

        // ------------------------------------------------------------
        // Find roots (entities with Transform + WorldMatrix but NO Parent)
        // ------------------------------------------------------------
        auto roots = reg.view<components::Transform, components::WorldMatrix>(entt::exclude<components::Parent>);

        // ------------------------------------------------------------
        // Update each root recursively
        // ------------------------------------------------------------
        for (auto [e, _, __] : roots.each())
        {
            update_world_recursive(reg, e, glm::mat4(1.0f), false, children);
        }

        update_transform_dependents(reg);
    }

    void update_world_recursive(
        entt::registry& reg,
        entt::entity e,
        const glm::mat4& parentWorld,
        bool parent_was_dirty,
        const std::unordered_map<entt::entity, std::vector<entt::entity> >& children)
    {
        auto& t = reg.get<components::Transform>(e);
        auto& wm = reg.get<components::WorldMatrix>(e);

        // Shake forces world recompute
        bool force = reg.any_of<components::Shake, components::ShakeOnce>(e);

        // Always compute local TRS (cheap + required for correctness)
        glm::mat4 local = compute_model_matrix(t);

        // Determine if this entity must update its world matrix
        bool must_update_world =
            t.dirty || parent_was_dirty || force;

        if (must_update_world)
        {
            // Remove scale from parent before applying child local transform
            glm::mat4 parentNoScale = parentWorld;

            for (int i = 0; i < 3; ++i)
            {
                glm::vec3 v = glm::vec3(parentNoScale[i]);     // extract XYZ
                v = glm::normalize(v);                         // normalize
                parentNoScale[i] = glm::vec4(v, parentNoScale[i].w); // restore W
            }

            wm.value = parentNoScale * local;
        }

        // Clear dirty flag
        t.dirty = false;

        // Recurse into children
        if (auto it = children.find(e); it != children.end())
        {
            for (entt::entity child : it->second)
            {
                update_world_recursive(
                    reg,
                    child,
                    wm.value,
                    must_update_world,   // propagate dirty downward
                    children
                );
            }
        }
    }

}
