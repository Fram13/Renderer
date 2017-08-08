#pragma once

#include "renderer.h"

namespace graphics
{
	class shader
	{
	public:
		virtual void prepare() = 0;
		virtual vec4 vertex(const wavefront_model& model, int face_ind, int vert_ind) = 0;
		virtual TGAColor fragment(const wavefront_model& model, const vec3& barycentric) = 0;
	};
}
