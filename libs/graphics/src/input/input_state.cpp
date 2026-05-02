#include <graphics/input/input_state.h>

namespace graphics::input
{

	void InputState::reset_frame_accumulators()
	{
		scroll.x = 0.0f;
		scroll.y = 0.0f;
		mouse.dx = 0.0;
		mouse.dy = 0.0;
	}

}
