#ifndef GRAPHICS_APP_INPUT_STATE_H
#define GRAPHICS_APP_INPUT_STATE_H

#include <graphics/platform/gl_includes.h>

namespace graphics::app::input_state
{

	struct MouseState
	{
		double x = 0.;
		double y = 0.;
		double dx = 0.;
		double dy = 0.;
		bool first_time = true;
		bool left_is_down = false;
		bool right_is_down = false;
		bool middle_is_down = false;
	};

	struct ScrollState {
		float x = 0.0f;
		float y = 0.0f;
	};

	struct KeyState 
	{
		bool down[GLFW_KEY_LAST + 1] = {};

		bool is_down(int key) const 
		{
			return down[key];
		}

		bool shift_is_down() const 
		{
			return down[GLFW_KEY_LEFT_SHIFT] || down[GLFW_KEY_RIGHT_SHIFT];
		}

		bool ctrl_is_down() const 
		{
			return down[GLFW_KEY_LEFT_CONTROL] || down[GLFW_KEY_RIGHT_CONTROL];
		}

		bool alt_is_down() const 
		{
			return down[GLFW_KEY_LEFT_ALT] || down[GLFW_KEY_RIGHT_ALT];
		}
	};

	struct InputState 
	{
		MouseState mouse;
		ScrollState scroll;
		KeyState keys;

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

#endif // GRAPHICS_APP_INPUT_STATE_H
