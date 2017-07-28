#pragma once

#include "renderer.h"

namespace graphics
{
	class shader
	{
	public:
		virtual void prepare() = 0;
		virtual vec4 vertex(wavefront_model& model, int face_ind, int vert_ind) = 0;
		virtual TGAColor fragment(wavefront_model& model, vec3& barycentric) = 0;
	};
}
