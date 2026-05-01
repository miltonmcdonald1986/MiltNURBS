#ifndef GRAPHICS_INPUT_KEY_STATE_H
#define GRAPHICS_INPUT_KEY_STATE_H

#include <array>

#include <graphics/input/key.h>

namespace graphics::input
{

	struct KeyState
	{
		bool operator[](Key key) const;
		bool& operator[](Key key);
		bool shift() const;
		bool ctrl() const;
		bool alt() const;
		bool super() const;

	private:
		
		enum class Mod { Shift, Control, Alt, Super };

		static constexpr size_t idx(Key key);
		bool mod_is_down(Mod mod) const;
		
		static constexpr size_t key_count = static_cast<size_t>(Key::COUNT);
		std::array<bool, key_count> down{};
	};

}

#endif // GRAPHICS_INPUT_KEY_STATE_H
