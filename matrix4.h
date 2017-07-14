#pragma once

#include "vec4.h"
#include <stdexcept>
using namespace std;

//ѕредставл€ет квадратную матрицу 4х4.
class matrix4
{
public:
	//¬озвращает единичную матрицу.
	static matrix4 identity();

	//¬озвращает произведение данной матрицы на указанную.
	matrix4 multiply(matrix4& other);

	//¬озвращает произведение данной матрицы на указанный вектор.
	vec4 multiply(vec4& other);

	//”станавливает указанную строку указанным вектором.
	void set_row(vec4& row, int ind);

	//¬озвращает указанную строку.
	vec4 get_row(int ind);

	//”станавливает указанный столбец указанным вектором.
	void set_column(vec4& column, int ind);

	//¬озвращает указанный столбец.
	vec4 get_column(int ind);

	//¬озвращает транспонированную матрицу.
	matrix4 transponse();

	//¬озвращает произведение данной матрицы на указанную.
	matrix4 operator*(matrix4& other);

	//¬озвращает произведение данной матрицы на указанный вектор.
	vec4 operator*(vec4& other);

	//¬озвращает указанную строку.
	vec4& operator[](int ind);

	//¬ыполн€ет присванивание указанной матрицы данной.
	matrix4& operator=(matrix4& other);

private:
	//–азмер матрицы.
	const int size = 4;

	//—троки матрицы.
	vec4 rows[4];
};