#include "wavefront_model.h"
using namespace graphics;

wavefront_model::wavefront_model(const std::string& path)
{
	std::string _path = path;

	read_obj_file(path);
	std::string name = _path.replace(path.length() - 4, 4, "");
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

face wavefront_model::get_face(int ind) const
{
	return faces[ind];
}

vec3 wavefront_model::vertex(int ind) const
{
	return vertices[ind];
}

vec3 wavefront_model::texture_vertex(int ind) const
{
	return texture_vertices[ind];
}

vec3 wavefront_model::normal(int ind) const
{
	return normals[ind];
}

TGAColor wavefront_model::texture_map(const vec3& coords) const
{
	int x = (int)(coords[0] * _texture_map->get_width());
	int y = (int)(_texture_map->get_height() - coords[1] * _texture_map->get_height());

	return _texture_map->get(x, y);
}

vec3 wavefront_model::normal_map(const vec3& coords) const
{
	int x = (int)(coords[0] * _normal_map->get_width());
	int y = (int)(_normal_map->get_height() - coords[1] * _normal_map->get_height());

	TGAColor color = _normal_map->get(x, y);
	return vec3({ color.r - 127.0f, color.g - 127.0f, color.b - 127.0f }).normalize();
}

vec3 wavefront_model::normal_tangent_map(const vec3& coords) const
{
	int x = (int)(coords[0] * _normal_tangent_map->get_width());
	int y = (int)(_normal_tangent_map->get_height() - coords[1] * _normal_tangent_map->get_height());

	TGAColor color = _normal_tangent_map->get(x, y);
	return vec3({ color.r - 127.0f, color.g - 127.0f, color.b - 127.0f }).normalize();
}

float wavefront_model::specular_map(const vec3& coords) const
{
	int x = (int)(coords[0] * _specular_map->get_width());
	int y = (int)(_specular_map->get_height() - coords[1] * _specular_map->get_height());
	unsigned char exp = _specular_map->get(x, y).r;

	return (float)(exp > 0 ? exp : exp + 1);
}

int wavefront_model::faces_num() const noexcept
{
	return faces.size();
}

void wavefront_model::read_obj_file(const std::string& path)
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
				texture_vertices.push_back(vec3{ x, y, z });
			}
			else if (s == 'n')
			{
				normals.push_back(vec3{ x, y, z });
			}
			else
			{
				vertices.push_back(vec3{ x, y, z });
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

TGAImage* wavefront_model::load_map(const std::string& path)
{
	TGAImage* map = new TGAImage();

	if (!map->read_tga_file(path))
	{
		delete map;
		map = nullptr;
	}

	return map;
}

void wavefront_model::scale(const vec3& vec) noexcept
{
	matrix4 m = matrix4::identity();
	m[0][0] = vec[0];
	m[1][1] = vec[1];
	m[2][2] = vec[2];

	for (uint i = 0; i < vertices.size(); i++)
	{
		vertices[i] = vec4::project(m * vec3::embed_point(vertices[i]));
	}
}

void wavefront_model::translate(const vec3& vec) noexcept
{
	matrix4 m = matrix4::identity();
	m[0][3] = vec[0];
	m[1][3] = vec[1];
	m[2][3] = vec[2];

	for (uint i = 0; i < vertices.size(); i++)
	{
		vertices[i] = vec4::project(m * vec3::embed_point(vertices[i]));
	}
}