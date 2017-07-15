#pragma once

#include "geometry.h"
#include "tgaimage.h"
#include "face.h"
#include <vector>
#include <fstream>
#include <stdexcept>
using namespace std;

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
	vector<face>& get_faces();

	//���������� �������� ������.
	TGAImage& get_texture();

private:
	//����� ����� OBJ.
	ifstream input;

	//������ ��������� ������.
	vector<face> faces;

	//��������� �� �������� ������.
	TGAImage* texture;

	//������ ���� OBJ � ��������� ������ � ������.
	void load();

	/*void rotate(float angle, const vec3& vec);
	void scale(const vec3& vec);
	void translate(const vec3& vec);*/
};