#pragma once

#include "vec3.h"
#include "vec4.h"
#include "matrix4.h"

//�������� ����������� ������ ��� �������������� ��������������.
class geometry
{
public:
	//��������� ������ �� ���������� ��������� � ����������.
	static vec3 to_3d(vec4& vec);

	//��������� ������ �� ���������� ��������� � ����������.
	static vec4 to_4d(vec3& vec);

	//��������� ��������������� ���������� ����� � ������������.
	static vec3 barycentric_coords(vec3& v1, vec3& v2, vec3& v3, vec3& p);

private:
	//���������� ����������� ����������.
	static float e;
};
