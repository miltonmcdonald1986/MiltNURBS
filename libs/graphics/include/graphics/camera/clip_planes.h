#ifndef GRAPHICS_CAMERA_CLIP_PLANES_H
#define GRAPHICS_CAMERA_CLIP_PLANES_H

namespace graphics::camera
{

	struct ClipPlanes
	{
		float near = 0.1f;
		float far = 1000.f;
	};

}

#endif // GRAPHICS_CAMERA_CLIP_PLANES_H
