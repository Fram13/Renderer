#pragma once

#include "matrix.h"

using vec3 = vector<3>;
using vec4 = vector<4>;
using matrix3 = matrix<3, 3>;
using matrix4 = matrix<4, 4>;

class geometry
{
public:
	//¬ычисл€ет барицетрические координаты точки в треугольнике.
	static vec3 barycentric(matrix3& vert, vec3& p)
	{
		vec3 x = { vert[1][0] - vert[0][0], vert[2][0] - vert[0][0], vert[0][0] - p[0] };
		vec3 y = { vert[1][1] - vert[0][1], vert[2][1] - vert[0][1], vert[0][1] - p[1] };
		vec3 cross = vec3::cross_product(x, y);

		return vec3({ 1.0f - (cross[0] + cross[1]) / cross[2], cross[0] / cross[2], cross[1] / cross[2] });
	}

	//¬ычисл€ет глобальные барицентрические координаты на основе экранных барицентрических координат.
	static vec3 global_barycentric(vec3& screen_coords, matrix3& vertices, float r)
	{
		float rz1 = r * vertices[0][2] + 1.0f;
		float rz2 = r * vertices[1][2] + 1.0f;
		float rz3 = r * vertices[2][2] + 1.0f;

		return vec3({ screen_coords[0] / rz1, screen_coords[1] / rz2, screen_coords[2] / rz3 }) * (1.0f / (screen_coords[0] / rz1 + screen_coords[1] / rz2 + screen_coords[2] / rz3));
	}
};