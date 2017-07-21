#pragma once

#include "matrix.h"

namespace graphics
{
	using vec3 = vector<3>;
	using vec4 = vector<4>;
	using matrix3 = matrix<3, 3>;
	using matrix4 = matrix<4, 4>;

	class geometry
	{
	public:
		//¬ычисл€ет барицетрические координаты точки в треугольнике.
		static vec3 barycentric(matrix3& vertices, vec3& p)
		{
			vec3 x = { vertices[0][1] - vertices[0][0], vertices[0][2] - vertices[0][0], vertices[0][0] - p[0] };
			vec3 y = { vertices[1][1] - vertices[1][0], vertices[1][2] - vertices[1][0], vertices[1][0] - p[1] };
			vec3 cross = vec3::cross_product(x, y);

			return vec3({ 1.0f - (cross[0] + cross[1]) / cross[2], cross[0] / cross[2], cross[1] / cross[2] });
		}

		//¬ычисл€ет глобальные барицентрические координаты на основе экранных барицентрических координат.
		static vec3 global_barycentric(vec3& screen_coords, matrix3& vertices, float r)
		{
			vec3 rz = vertices.get_row(2) * r + vec3({ 1.0f, 1.0f, 1.0f });

			return vec3({ screen_coords[0] / rz[0], screen_coords[1] / rz[1], screen_coords[2] / rz[2] }) * (1.0f / (screen_coords[0] / rz[0] + screen_coords[1] / rz[1] + screen_coords[2] / rz[2]));
		}
	};
}