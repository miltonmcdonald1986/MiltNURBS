#include <graphics/input/key_state.h>

namespace graphics::input
{

	bool KeyState::operator[](Key key) const
	{
		return down[idx(key)];
	}

	bool& KeyState::operator[](Key key)
	{
		return down[idx(key)];
	}

	bool KeyState::shift() const
	{
		return mod_is_down(Mod::Shift);
	}

	bool KeyState::ctrl() const
	{
		return mod_is_down(Mod::Control);
	}

	bool KeyState::alt() const
	{
		return mod_is_down(Mod::Alt);
	}

	bool KeyState::super() const
	{
		return mod_is_down(Mod::Super);
	}

	constexpr size_t KeyState::idx(Key key)
	{
		return static_cast<size_t>(key);
	}

	bool KeyState::mod_is_down(Mod mod) const
	{
		constexpr std::array<std::array<Key, 2>, 4> pairs
		{ {
			{ Key::LeftShift,   Key::RightShift   },
			{ Key::LeftControl, Key::RightControl },
			{ Key::LeftAlt,     Key::RightAlt     },
			{ Key::LeftSuper,   Key::RightSuper   }
		} };

		auto p = pairs[static_cast<size_t>(mod)];
		return (*this)[p[0]] || (*this)[p[1]];
	}

}
