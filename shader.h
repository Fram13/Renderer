#pragma once

#include "geometry.h"
#include "tgaimage.h"

class shader
{
public:
	virtual vec3 vertex(vec3& p) = 0;
	virtual TGAColor fragment(vec3& barycentric_coords) = 0;
};