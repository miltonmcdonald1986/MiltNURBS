#ifndef GRAPHICS_UI_RENDER_SETTINGS_WIDGET_H
#define GRAPHICS_UI_RENDER_SETTINGS_WIDGET_H

namespace graphics::engine { struct AppData; }

namespace graphics::ui
{

	void draw_flash_widget(engine::AppData* p_data);
	void draw_per_entity_color_widget(engine::AppData* p_data);
	void draw_render_settings_widget(engine::AppData* p_data);
	void draw_shake_widget(engine::AppData* p_data);
	void draw_shake_once_widget(engine::AppData* p_data);

}

#endif // GRAPHICS_UI_RENDER_SETTINGS_WIDGET_H
