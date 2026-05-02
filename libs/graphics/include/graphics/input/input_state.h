#ifndef GRAPHICS_INPUT_INPUT_STATE_H
#define GRAPHICS_INPUT_INPUT_STATE_H

#include <graphics/input/key_state.h>
#include <graphics/input/mouse_state.h>
#include <graphics/input/scroll_state.h>

namespace graphics::input
{

	struct InputState 
	{
		void reset_frame_accumulators();

		input::MouseState mouse;
		input::ScrollState scroll;
		input::KeyState keys;
	};

}

#endif // GRAPHICS_INPUT_INPUT_STATE_H
