#pragma once

#include "vec4.h"
#include <stdexcept>
using namespace std;

//������������ ���������� ������� 4�4.
class matrix4
{
public:
	//���������� ��������� �������.
	static matrix4 identity();

	//���������� ������������ ������ ������� �� ���������.
	matrix4 multiply(matrix4& other);

	//���������� ������������ ������ ������� �� ��������� ������.
	vec4 multiply(vec4& other);

	//������������� ��������� ������ ��������� ��������.
	void set_row(vec4& row, int ind);

	//���������� ��������� ������.
	vec4 get_row(int ind);

	//������������� ��������� ������� ��������� ��������.
	void set_column(vec4& column, int ind);

	//���������� ��������� �������.
	vec4 get_column(int ind);

	//���������� ����������������� �������.
	matrix4 transponse();

	//���������� ������������ ������ ������� �� ���������.
	matrix4 operator*(matrix4& other);

	//���������� ������������ ������ ������� �� ��������� ������.
	vec4 operator*(vec4& other);

	//���������� ��������� ������.
	vec4& operator[](int ind);

	//��������� ������������� ��������� ������� ������.
	matrix4& operator=(matrix4& other);

private:
	//������ �������.
	const int size = 4;

	//������ �������.
	vec4 rows[4];
};