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
		geometry() = delete;

		static vec3 cross_product(const vec3& left, const vec3& right) noexcept
		{
			return vec3{ left[1] * right[2] - right[1] * left[2], -(left[0] * right[2] - right[0] * left[2]), left[0] * right[1] - right[0] * left[1] };
		}

		static vec3 barycentric(const matrix3& vertices, const vec3& p) noexcept
		{
			vec3 x = { vertices[0][1] - vertices[0][0], vertices[0][2] - vertices[0][0], vertices[0][0] - p[0] };
			vec3 y = { vertices[1][1] - vertices[1][0], vertices[1][2] - vertices[1][0], vertices[1][0] - p[1] };
			vec3 cross = geometry::cross_product(x, y);

			return vec3{ 1.0f - (cross[0] + cross[1]) / cross[2], cross[0] / cross[2], cross[1] / cross[2] };
		}
	};
}