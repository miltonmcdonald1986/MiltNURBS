#ifndef GRAPHICS_UI_IMGUI_LAYER_H
#define GRAPHICS_UI_IMGUI_LAYER_H

#include <graphics/app/app.h>

namespace graphics::ui::imgui_layer
{
	
	void begin_imgui_frame();
	void end_imgui_frame();
	void init_imgui(GLFWwindow* p_window);

}

#endif // GRAPHICS_UI_IMGUI_LAYER_H