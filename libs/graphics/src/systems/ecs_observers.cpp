#include <graphics/systems/ecs_observers.h>

#include <entt/entt.hpp>

#include <graphics/components/world_matrix.h>
#include <graphics/engine/app_data.h>
#include <graphics/scene/scene.h>

namespace graphics::systems
{

    engine::AppData* get_app(const entt::registry& reg)
    {
        return reg.ctx().get<engine::AppData*>();
    }

    void on_transform_constructed(entt::registry& reg, entt::entity e)
    {
        engine::AppData* p_data = get_app(reg);
        if (!p_data)
            return;

        scene::Scene* scene = p_data->p_active_scene;
        if (!scene)
            return;

        // 1. Record initial transform (your existing behavior)
        const components::Transform& t = reg.get<components::Transform>(e);
        scene->initial_transforms[e] = t;

        // 2. Auto-add WorldMatrix if missing
        if (!reg.any_of<components::WorldMatrix>(e))
            reg.emplace<components::WorldMatrix>(e);

        // 4. Mark dirty so the TransformSystem recomputes world matrix
        reg.get<components::Transform>(e).dirty = true;
    }


    void on_transform_updated(entt::registry& reg, entt::entity e) 
    {
        engine::AppData* p_data = get_app(reg);
        if (!p_data)
            return;

        scene::Scene* scene = p_data->p_active_scene;
        if (!scene)
            return;

        auto& initial_transforms = scene->initial_transforms;

        if (!initial_transforms.contains(e)) {
            const components::Transform& t = reg.get<components::Transform>(e);
            initial_transforms[e] = t;
        }
    }

    void on_transform_destroyed(entt::registry& reg, entt::entity e) 
    {
        engine::AppData* p_data = get_app(reg);

        scene::Scene* scene = p_data->p_active_scene;
        if (!scene)
            return;

        scene->initial_transforms.erase(e);
    }

    void register_transform_observers(engine::AppData* p_data) 
    {
        // TODO: apply this to all scenes once we have a mechanism for more than one scene???

        if (!p_data)
            return;

        scene::Scene* scene = p_data->p_active_scene;
        if (!scene)
            return;

        scene->reg.ctx().emplace<engine::AppData*>(p_data);
        
        scene->reg.on_construct<components::Transform>().connect<&on_transform_constructed>();
        scene->reg.on_update<components::Transform>().connect<&on_transform_updated>();
        scene->reg.on_destroy<components::Transform>().connect<&on_transform_destroyed>();
    }

}
