#pragma once

#include "geometry.h"
#include "tgaimage.h"
#include "face.h"
#include <vector>
#include <fstream>
#include <stdexcept>
using namespace std;

//Представляет модель в формате Wavefront OBJ.
class model
{
public:
	//Создает экземпляр model и считывает данные из файла OBJ.
	model(const char* modelPath);

	//Создает экземпляр model и считывает данные из файла OBJ и загружает текстуру модели.
	model(const char* modelPath, const char* texturePath);

	//Освобождает ресурсы.
	~model();

	//Возвращает массив, содержащий полигоны модели.
	vector<face>& get_faces();

	//Возвращает текстуру модели.
	TGAImage& get_texture();

private:
	//Поток файла OBJ.
	ifstream input;

	//Массив полигонов модели.
	vector<face> faces;

	//Указатель на текстуру модели.
	TGAImage* texture;

	//Читает файл OBJ и загружает данные в память.
	void load();

	/*void rotate(float angle, const vec3& vec);
	void scale(const vec3& vec);
	void translate(const vec3& vec);*/
};