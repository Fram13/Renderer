#pragma once

#include "geometry.h"
#include "face.h"
#include "tgaimage.h"
#include <vector>
#include <fstream>

//������������ ������ � ������� Wavefront OBJ.
class model
{
public:
	//������� ��������� model � ��������� ������ �� ����� OBJ.
	model(const char* modelPath);

	//������� ��������� model � ��������� ������ �� ����� OBJ � ��������� �������� ������.
	model(const char* modelPath, const char* texturePath);

	//����������� �������.
	~model();

	//���������� ������, ���������� �������� ������.
	std::vector<face>& get_faces();

	//���������� ������, ���������� ��������� ���������� ������.
	std::vector<vec3>& get_vertices();

	//���������� ������, ���������� ���������� ���������� ������.
	std::vector<vec3>& get_texture_vertices();

	//���������� ������, ���������� ������� ������.
	std::vector<vec3>& get_normals();

	//���������� �������� ������.
	TGAImage& get_texture();

private:
	//����� ����� OBJ.
	std::ifstream input;

	//������ ��������� ������.
	std::vector<face> faces;

	//������ ��������� ��������� ������.
	std::vector<vec3> vertices;

	//������ ���������� ��������� ������.
	std::vector<vec3> texture_vertices;

	//������ �������� ������.
	std::vector<vec3> normals;

	//��������� �� �������� ������.
	TGAImage* texture;

	//������ ���� OBJ � ��������� ������ � ������.
	void load();

	/*void rotate(float angle, const vec3& vec);
	void scale(const vec3& vec);
	void translate(const vec3& vec);*/
};