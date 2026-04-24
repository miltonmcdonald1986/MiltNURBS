#include <graphics/systems/color.h>

#include <graphics/components/color.h>
#include <graphics/components/flash.h>

using graphics::components::color::Color;
using graphics::components::flash::Flash;

namespace graphics::systems::color
{

    void update_color_no_flash(entt::registry& reg)
    {
        auto view = reg.view<Color>(entt::exclude<Flash>);
        for (auto [entity, color] : view.each())
        {
            color.rgba[0] = color.base[0];
            color.rgba[1] = color.base[1];
            color.rgba[2] = color.base[2];
            color.rgba[3] = color.base[3];
        }
    }

}
