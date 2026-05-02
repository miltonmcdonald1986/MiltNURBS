#include <graphics/platform/window_state.h>

namespace graphics::platform
{

    float WindowState::aspect() const
    {
        return float(width) / float(height);
    }

}
