#pragma once

#include "renderer.h"

class shader
{
public:
	virtual matrix3 vertex(wavefront_model& m, int face_ind) = 0;
	virtual TGAColor fragment(wavefront_model& model, vec3& barycentric) = 0;
};
