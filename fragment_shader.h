#pragma once

#include "geometry.h"
#include "tgaimage.h"

class fragment_shader
{
public:
	virtual TGAColor fragment(vec3& bar, vec3& vt1, vec3& vt2, vec3& vt3, vec3& vn1, vec3& vn2, vec3& vn3, TGAImage& texture, vec3& light) = 0;
};
