#include <graphics/systems/ecs_observers.h>

#include <entt/entt.hpp>

#include <graphics/app/app.h>

using graphics::app::app::App;
using graphics::components::transform::Transform;

namespace graphics::systems::ecs_observers
{

    App& get_app(entt::registry& reg)
    {
        return *reg.ctx().get<App*>();
    }

    void on_transform_constructed(entt::registry& reg, entt::entity e) 
    {
        App& app = get_app(reg);
        const Transform& t = reg.get<Transform>(e);
        app.initialTransforms[e] = t;
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
