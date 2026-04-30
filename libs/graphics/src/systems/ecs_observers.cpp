#include <graphics/systems/ecs_observers.h>

#include <entt/entt.hpp>

#include <graphics/app/app.h>
#include <graphics/components/world_matrix.h>
#include <graphics/scene/scene.h>

using graphics::app::app::App;
using graphics::components::transform::Transform;
using graphics::components::world_matrix::WorldMatrix;
using graphics::scene::Scene;

namespace graphics::systems::ecs_observers
{

    App& get_app(entt::registry& reg)
    {
        return *reg.ctx().get<App*>();
    }

    void on_transform_constructed(entt::registry& reg, entt::entity e)
    {
        App& app = get_app(reg);
        
        Scene* scene = app.p_active_scene;
        if (!scene)
            return;

        // 1. Record initial transform (your existing behavior)
        const Transform& t = reg.get<Transform>(e);
        scene->initial_transforms[e] = t;

        // 2. Auto-add WorldMatrix if missing
        if (!reg.any_of<WorldMatrix>(e))
            reg.emplace<WorldMatrix>(e);

        // 4. Mark dirty so the TransformSystem recomputes world matrix
        reg.get<Transform>(e).dirty = true;
    }


    void on_transform_updated(entt::registry& reg, entt::entity e) 
    {
        App& app = get_app(reg);
        
        Scene* scene = app.p_active_scene;
        if (!scene)
            return;

        auto& initial_transforms = scene->initial_transforms;

        if (!initial_transforms.contains(e)) {
            const Transform& t = reg.get<Transform>(e);
            initial_transforms[e] = t;
        }
    }

    void on_transform_destroyed(entt::registry& reg, entt::entity e) 
    {
        App& app = get_app(reg);

        Scene* scene = app.p_active_scene;
        if (!scene)
            return;

        scene->initial_transforms.erase(e);
    }

    void register_transform_observers(App& app) 
    {
        // TODO: apply this to all scenes once we have a mechanism for more than one scene???

        Scene* scene = app.p_active_scene;
        if (!scene)
            return;

        scene->reg.ctx().emplace<App*>(&app);
        
        scene->reg.on_construct<Transform>().connect<&on_transform_constructed>();
        scene->reg.on_update<Transform>().connect<&on_transform_updated>();
        scene->reg.on_destroy<Transform>().connect<&on_transform_destroyed>();
    }

}
