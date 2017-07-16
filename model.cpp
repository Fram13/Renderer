#include "model.h"

model::model(const char* modelPath)
{
	input.open(modelPath);

	if (!input.is_open())
		throw std::invalid_argument("Can not open the file.");

	texture = nullptr;

	load();
}

model::model(const char* modelPath, const char* texturePath)
{
	input.open(modelPath);

	if (!input.is_open())
		throw std::invalid_argument("Can not open the file.");

	texture = new TGAImage();

	if (!texture->read_tga_file(texturePath))
	{
		delete texture;
		texture = nullptr;
		throw std::invalid_argument("Can not open the file.");
	}

	load();
}

model::~model()
{
	if (texture != nullptr)
	{
		delete texture;
		texture = nullptr;
	}
}

void model::load()
{
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
				texture_vertices.push_back(vec3({ x, y, z }));
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
}

std::vector<face>& model::get_faces()
{
	return faces;
}

std::vector<vec3>& model::get_vertices()
{
	return vertices;
}

std::vector<vec3>& model::get_texture_vertices()
{
	return texture_vertices;
}

std::vector<vec3>& model::get_normals()
{
	return normals;
}

TGAImage& model::get_texture()
{
	return *texture;
}