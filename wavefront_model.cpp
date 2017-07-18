#include "wavefront_model.h"

wavefront_model::wavefront_model(std::string path)
{
	read_obj_file(path + "\\model.obj");
	texture_map = load_map(path + "\\texture_map.tga");
	normal_map = load_map(path + "\\normal_map.tga");
}

wavefront_model::~wavefront_model()
{
	if (texture_map != nullptr)
	{
		delete texture_map;
		texture_map = nullptr;
	}

	if (normal_map != nullptr)
	{
		delete normal_map;
		normal_map = nullptr;
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

TGAColor wavefront_model::get_texture(vec3& coords)
{
	return texture_map->get((int)coords[0], (int)coords[1]);
}

TGAColor wavefront_model::get_normal(vec3& coords)
{
	return normal_map->get((int)coords[0], (int)coords[1]);
}

int wavefront_model::map_width()
{
	return texture_map->get_width();
}

int wavefront_model::map_height()
{
	return texture_map->get_height();
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