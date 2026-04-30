#ifndef GRAPHICS_PLATFORM_TIME_H
#define GRAPHICS_PLATFORM_TIME_H

namespace graphics::platform::time
{

	struct Time
	{
		void update();

		double delta_time{ 0. };
		double last_time{ 0. };
	};

}

#endif // GRAPHICS_PLATFORM_TIME_H
