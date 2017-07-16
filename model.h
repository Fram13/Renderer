#pragma once

#include "geometry.h"
#include "face.h"
#include "tgaimage.h"
#include <vector>
#include <fstream>

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
	std::vector<face>& get_faces();

	//Возвращает массив, содержащий вершинные координаты модели.
	std::vector<vec3>& get_vertices();

	//Возвращает массив, содержащий текстурные координаты модели.
	std::vector<vec3>& get_texture_vertices();

	//Возвращает массив, содержащий нормали модели.
	std::vector<vec3>& get_normals();

	//Возвращает текстуру модели.
	TGAImage& get_texture();

private:
	//Поток файла OBJ.
	std::ifstream input;

	//Массив полигонов модели.
	std::vector<face> faces;

	//Массив вершинных координат модели.
	std::vector<vec3> vertices;

	//Массив текстурных координат модели.
	std::vector<vec3> texture_vertices;

	//Массив нормалей модели.
	std::vector<vec3> normals;

	//Указатель на текстуру модели.
	TGAImage* texture;

	//Читает файл OBJ и загружает данные в память.
	void load();

	/*void rotate(float angle, const vec3& vec);
	void scale(const vec3& vec);
	void translate(const vec3& vec);*/
};