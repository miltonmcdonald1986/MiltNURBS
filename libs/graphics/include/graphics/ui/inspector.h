#ifndef GRAPHICS_UI_INSPECTOR_H
#define GRAPHICS_UI_INSPECTOR_H

#include <functional>

#include <entt/entt.hpp>

#include <graphics/app/app.h>

namespace graphics::ui::inspector
{

	// A function that draws UI for a component on a specific entity
	using InspectorFn = std::function<void(entt::registry&, entt::entity)>;

	// Register all component inspectors (call once at startup)
	void register_inspectors();

	// Draw the inspector panel for the currently selected entity
	void draw_inspector(app::app::App& app);

}

#endif // GRAPHICS_UI_INSPECTOR_H
