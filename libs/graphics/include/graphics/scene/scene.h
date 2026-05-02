#ifndef GRAPHICS_SCENE_SCENE_H
#define GRAPHICS_SCENE_SCENE_H

#include <entt/entt.hpp>
#include <graphics/components/transform.h>

namespace graphics::scene
{

    struct Scene 
    {
        entt::registry reg;
        std::unordered_map<entt::entity, components::Transform> initial_transforms;
    };

}

#endif // GRAPHICS_SCENE_SCENE_H
