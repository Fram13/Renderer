#include "matrix4.h"

matrix4 matrix4::identity()
{
	matrix4 res;

	res[0][0] = 1.0;
	res[1][1] = 1.0;
	res[2][2] = 1.0;
	res[3][3] = 1.0;

	return res;
}

matrix4 matrix4::multiply(matrix4& other)
{
	matrix4 res;

	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			res[i][j] = this->rows[i] * other.get_column(j);
		}
	}

	return res;
}

vec4 matrix4::multiply(vec4& other)
{
	vec4 res;

	res[0] = rows[0] * other;
	res[1] = rows[1] * other;
	res[2] = rows[2] * other;
	res[3] = rows[3] * other;

	return res;
}

void matrix4::set_row(vec4& row, int ind)
{
	if (ind < 0 || ind >= size)
	{
		throw out_of_range("index is out of range.");
	}

	rows[ind] = row;
}

vec4 matrix4::get_row(int ind)
{
	if (ind < 0 || ind >= size)
	{
		throw out_of_range("index is out of range.");
	}

	return rows[ind];
}

void matrix4::set_column(vec4& column, int ind)
{
	if (ind < 0 || ind >= size)
	{
		throw out_of_range("index is out of range.");
	}

	rows[0][ind] = column[0];
	rows[1][ind] = column[1];
	rows[2][ind] = column[2];
	rows[3][ind] = column[3];
}

vec4 matrix4::get_column(int ind)
{
	if (ind < 0 || ind >= size)
	{
		throw out_of_range("index is out of range.");
	}

	return vec4(rows[0][ind], rows[1][ind], rows[2][ind], rows[3][ind]);
}

matrix4 matrix4::transponse()
{
	matrix4 res;

	res.set_column(rows[0], 0);
	res.set_column(rows[1], 1);
	res.set_column(rows[2], 2);
	res.set_column(rows[3], 3);

	return res;
}

matrix4 matrix4::operator*(matrix4& other)
{
	return multiply(other);
}

vec4 matrix4::operator*(vec4& other)
{
	return multiply(other);
}

vec4& matrix4::operator[](int ind)
{
	if (ind < 0 || ind >= size)
	{
		throw out_of_range("index is out of range.");
	}

	return rows[ind];
}

matrix4& matrix4::operator=(matrix4& other)
{
	rows[0] = other[0];
	rows[1] = other[1];
	rows[2] = other[2];
	rows[3] = other[3];

	return *this;
}