#ifndef GRAPHICS_UI_RENDER_SETTINGS_WIDGET_H
#define GRAPHICS_UI_RENDER_SETTINGS_WIDGET_H

#include <graphics/app/app.h>

namespace graphics::ui::widgets
{

	void draw_flash_widget(app::app::App& app);
	void draw_per_entity_color_widget(app::app::App& app);
	void draw_render_settings_widget(app::app::App& app);
	void draw_shake_widget(app::app::App& app);
	void draw_shake_once_widget(app::app::App& app);

}

#endif // GRAPHICS_UI_RENDER_SETTINGS_WIDGET_H
