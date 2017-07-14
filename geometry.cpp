#include "geometry.h"

double geometry::e = 1e-9;

vec3 geometry::to_3d(vec4& vec)
{
	vec3 res(vec.x, vec.y, vec.z);

	if (abs(vec.w) > e)
	{
		res = res * (1.0 / vec.w);
	}

	return res;
}

vec4 geometry::to_4d(vec3& vec)
{
	return vec4(vec.x, vec.y, vec.z, 1.0);
}