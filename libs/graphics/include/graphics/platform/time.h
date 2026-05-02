#ifndef GRAPHICS_PLATFORM_TIME_H
#define GRAPHICS_PLATFORM_TIME_H

namespace graphics::platform
{

	struct Time
	{
		void update();

		float dt{ 0. };
		double prev{ 0. };
	};

}

#endif // GRAPHICS_PLATFORM_TIME_H
