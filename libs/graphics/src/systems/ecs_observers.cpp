#include <graphics/systems/ecs_observers.h>

#include <entt/entt.hpp>

#include <graphics/app/app.h>
#include <graphics/components/world_matrix.h>

using graphics::app::app::App;
using graphics::components::transform::Transform;
using graphics::components::world_matrix::WorldMatrix;

namespace graphics::systems::ecs_observers
{

    App& get_app(entt::registry& reg)
    {
        return *reg.ctx().get<App*>();
    }

    void on_transform_constructed(entt::registry& reg, entt::entity e)
    {
        App& app = get_app(reg);

        // 1. Record initial transform (your existing behavior)
        const Transform& t = reg.get<Transform>(e);
        app.initialTransforms[e] = t;

        // 2. Auto-add WorldMatrix if missing
        if (!reg.any_of<WorldMatrix>(e))
            reg.emplace<WorldMatrix>(e);

        // 4. Mark dirty so the TransformSystem recomputes world matrix
        reg.get<Transform>(e).dirty = true;
    }


    void on_transform_updated(entt::registry& reg, entt::entity e) 
    {
        App& app = get_app(reg);
        if (!app.initialTransforms.contains(e)) {
            const Transform& t = reg.get<Transform>(e);
            app.initialTransforms[e] = t;
        }
    }

    void on_transform_destroyed(entt::registry& reg, entt::entity e) 
    {
        App& app = get_app(reg);
        app.initialTransforms.erase(e);
    }

    void register_transform_observers(App& app) 
    {
        app.reg.ctx().emplace<App*>(&app);

        app.reg.on_construct<Transform>().connect<&on_transform_constructed>();
        app.reg.on_update<Transform>().connect<&on_transform_updated>();
        app.reg.on_destroy<Transform>().connect<&on_transform_destroyed>();
    }

}
