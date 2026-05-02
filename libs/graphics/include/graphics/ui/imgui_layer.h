#ifndef GRAPHICS_UI_IMGUI_LAYER_H
#define GRAPHICS_UI_IMGUI_LAYER_H

struct GLFWwindow;

namespace graphics::ui
{
	
	void begin_imgui_frame();
	void end_imgui_frame();
	void init_imgui(GLFWwindow* p_window);
	void terminate_imgui();

}

#endif // GRAPHICS_UI_IMGUI_LAYER_H
