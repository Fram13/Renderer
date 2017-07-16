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
	//Создает экземпляр renderer и инициализирует его поля указанными значениями.
	renderer(int width, int height, fragment_shader* shader);
	
	//Освобождает ресурсы.
	~renderer();

	//Отрисовывает модель в кадр.
	void render_model(model& m);

	//Устанавливает камеру в заданную точку в заданном направлении.
	void set_view(vec3& center, vec3& camera, vec3& up);

	//Устанавливает вектор точечного источника света.
	void set_light(vec3& light);

	//Возвращает кадр.
	TGAImage& get_frame();

private:
	//Глубина кадра.
	const int DEPTH = 200;

	//Ширина кадра.
	int width;

	//Высота кадра.
	int height;

	//Кадр отрисовщика.
	TGAImage* frame;

	//Буфер глубины кадра.
	int **zbuffer;

	//Матрица кадра.
	matrix4 viewport;

	//Матрица проекции.
	matrix4 projection;
	
	//Видовая матрица.
	matrix4 view;

	//Матрица преобразования вектора точечного источника света.
	matrix4 M;

	//Матрица преобразования текстурных координат.
	matrix4 texture_viewport;

	//Расстояние от камеры до точки начала координат (необходимо для создания центральной проекции).
	float distance;

	//Вектор точечного источника света.
	vec3 light;

	//Фрагментный шейдер.
	fragment_shader* shader;

	void render_face(vec3& v1, vec3& v2, vec3& v3, vec3& vt1, vec3& vt2, vec3& vt3, vec3& vn1, vec3& vn2, vec3& vn3, TGAImage& texture);
	
	vec3 vertex_shader(vec3& vec);
};