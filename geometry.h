#pragma once

#include "vec3.h"
#include "vec4.h"
#include "matrix4.h"

//Содержит необходимые методы для геометрических преобразований.
class geometry
{
public:
	//Переводит вектор из однородных координат в трехмерные.
	static vec3 to_3d(vec4& vec);

	//Переводит вектор из трехмерных координат в однородные.
	static vec4 to_4d(vec3& vec);

	//Вычисляет барицетрические координаты точки в треугольнике.
	static vec3 barycentric_coords(vec3& v1, vec3& v2, vec3& v3, vec3& p);

private:
	//Допустимая погрешность вычислений.
	static float e;
};
