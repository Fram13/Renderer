#pragma once

#include <numeric>
#include "geometry.h"
#include "tgaimage.h"
#include "model.h"
#include "fragment_shader.h"

//
class renderer
{
public:
	//������� ��������� renderer � �������������� ��� ���� ���������� ����������.
	renderer(int width, int height, fragment_shader* shader);
	
	//����������� �������.
	~renderer();

	//������������ ������ � ����.
	void render_model(model& m);

	//������������� ������ � �������� ����� � �������� �����������.
	void set_view(vec3& center, vec3& camera, vec3& up);

	//������������� ������ ��������� ��������� �����.
	void set_light(vec3& light);

	//���������� ����.
	TGAImage& get_frame();

private:
	//������� �����.
	const int DEPTH = 200;

	//������ �����.
	int width;

	//������ �����.
	int height;

	//���� �����������.
	TGAImage* frame;

	//����� ������� �����.
	int **zbuffer;

	//������� �����.
	matrix4 viewport;

	//������� ��������.
	matrix4 projection;
	
	//������� �������.
	matrix4 view;

	//������� �������������� ������� ��������� ��������� �����.
	matrix4 M;

	//������� �������������� ���������� ���������.
	matrix4 texture_viewport;

	//���������� �� ������ �� ����� ������ ��������� (���������� ��� �������� ����������� ��������).
	float distance;

	//������ ��������� ��������� �����.
	vec3 light;

	//����������� ������.
	fragment_shader* shader;

	void render_face(vec3& v1, vec3& v2, vec3& v3, vec3& vt1, vec3& vt2, vec3& vt3, vec3& vn1, vec3& vn2, vec3& vn3, TGAImage& texture);
	
	vec3 vertex_shader(vec3& vec);
};