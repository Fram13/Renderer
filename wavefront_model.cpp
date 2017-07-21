#include "wavefront_model.h"
using namespace graphics;

wavefront_model::wavefront_model(std::string path)
{
	read_obj_file(path);
	std::string name = path.replace(path.length() - 4, 4, "");
	_texture_map = load_map(name + "_diffuse.tga");
	_normal_map = load_map(name + "_nm.tga");
	_normal_tangent_map = load_map(name + "_nm_tangent.tga");
	_specular_map = load_map(name + "_spec.tga");
}

wavefront_model::~wavefront_model()
{
	if (_texture_map != nullptr)
	{
		delete _texture_map;
		_texture_map = nullptr;
	}

	if (_normal_map != nullptr)
	{
		delete _normal_map;
		_normal_map = nullptr;
	}

	if (_normal_tangent_map != nullptr)
	{
		delete _normal_tangent_map;
		_normal_tangent_map = nullptr;
	}

	if (_specular_map != nullptr)
	{
		delete _specular_map;
		_specular_map = nullptr;
	}
}

face wavefront_model::get_face(int ind)
{
	return faces[ind];
}

vec3 wavefront_model::vertex(int ind)
{
	return vertices[ind];
}

vec3 wavefront_model::texture_vertex(int ind)
{
	return texture_vertices[ind];
}

vec3 wavefront_model::normal(int ind)
{
	return normals[ind];
}

TGAColor wavefront_model::texture_map(vec3& coords)
{
	int x = (int)(coords[0] * _texture_map->get_width());
	int y = (int)(_texture_map->get_height() - coords[1] * _texture_map->get_height());

	return _texture_map->get(x, y);
}

vec3 wavefront_model::normal_map(vec3& coords)
{
	int x = (int)(coords[0] * _normal_map->get_width());
	int y = (int)(_normal_map->get_height() - coords[1] * _normal_map->get_height());

	TGAColor color = _normal_map->get(x, y);
	return vec3({ color.r - 127.0f, color.g - 127.0f, color.b - 127.0f }).normalize();
}

vec3 wavefront_model::normal_tangent_map(vec3& coords)
{
	int x = (int)(coords[0] * _normal_tangent_map->get_width());
	int y = (int)(_normal_tangent_map->get_height() - coords[1] * _normal_tangent_map->get_height());

	TGAColor color = _normal_tangent_map->get(x, y);
	return vec3({ color.r - 127.0f, color.g - 127.0f, color.b - 127.0f }).normalize();
}

float wavefront_model::specular_map(vec3& coords)
{
	int x = (int)(coords[0] * _specular_map->get_width());
	int y = (int)(_specular_map->get_height() - coords[1] * _specular_map->get_height());

	return _specular_map->get(x, y).r;
}

int wavefront_model::faces_num()
{
	return faces.size();
}

void wavefront_model::read_obj_file(std::string path)
{
	std::ifstream input(path);
	char s;
	float x, y, z;

	while (!input.eof())
	{
		input >> s;

		if (s == 'v')
		{
			if (isalpha(input.peek()))
			{
				input >> s;
			}

			input >> x >> y >> z;

			if (s == 't')
			{
				texture_vertices.push_back(vec3({ x, y, 1.0f }));
			}
			else if (s == 'n')
			{
				normals.push_back(vec3({ x, y, z }));
			}
			else
			{
				vertices.push_back(vec3({ x, y, z }));
			}
		}
		else if (s == 'f')
		{
			int v1, v2, v3, t1, t2, t3, n1, n2, n3;

			input >> v1 >> s >> t1 >> s >> n1;
			input >> v2 >> s >> t2 >> s >> n2;
			input >> v3 >> s >> t3 >> s >> n3;

			faces.push_back(face(--v1, --v2, --v3, --t1, --t2, --t3, --n1, --n2, --n3));
		}
		else
		{
			while ((s = input.peek()) != '\n' && s != EOF)
				input.get(s);
		}
	}

	input.close();
}

TGAImage* wavefront_model::load_map(std::string path)
{
	TGAImage* map = new TGAImage();

	if (!map->read_tga_file(path))
	{
		delete map;
		map = nullptr;
	}

	return map;
}