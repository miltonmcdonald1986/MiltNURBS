#ifndef GRAPHICS_INPUT_INPUT_STATE_H
#define GRAPHICS_INPUT_INPUT_STATE_H

#include <graphics/input/key_state.h>
#include <graphics/input/mouse_state.h>
#include <graphics/input/scroll_state.h>

namespace graphics::input
{

	struct InputState 
	{
		input::MouseState mouse;
		input::ScrollState scroll;
		input::KeyState keys;

		// Called once per frame
		void reset_frame_accumulators() 
		{
			scroll.x = 0.0f;
			scroll.y = 0.0f;
			mouse.dx = 0.0;
			mouse.dy = 0.0;
		}
	};

}

#endif // GRAPHICS_INPUT_INPUT_STATE_H
