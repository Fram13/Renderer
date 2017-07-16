#include "renderer.h"

float min(float a, float b)
{
	return a < b ? a : b;
}

float max(float a, float b)
{
	return a > b ? a : b;
}

renderer::renderer(int width, int height, fragment_shader* shader)
{
	//Инициализация кадра
	frame = new TGAImage(width, height, TGAImage::RGB);
	this->width = width;
	this->height = height;

	viewport[0][0] = viewport[0][3] = width / 2.0f;
	viewport[1][1] = viewport[1][3] = height / 2.0f;
	viewport[2][2] = viewport[2][3] = DEPTH / 2.0f;
	viewport[3][3] = 1.0f;

	//Инициализация буфера глубины
	zbuffer = new int*[height];

	for (int i = 0; i < height; i++)
	{
		zbuffer[i] = new int[width];

		for (int j = 0; j < width; j++)
		{
			zbuffer[i][j] = numeric_limits<int>::min();
			frame->set(i, j, TGAColor(255, 255, 255, 255));
		}
	}

	//Инициализация матрицы проекции (ортогональная проекция по умолчанию)
	projection = matrix4::identity();
	distance = -1.0f;

	//Инициализация видовой матрицы
	view = matrix4::identity();
	M = matrix4::identity();

	//Инициализация фрагментного шейдера
	this->shader = shader;
}

renderer::~renderer()
{
	if (frame != nullptr)
	{
		delete frame;
	}

	if (zbuffer != nullptr)
	{
		for (int i = 0; i < height; i++)
		{
			delete[] zbuffer[i];
		}

		delete[] zbuffer;
	}
}

void renderer::render_model(model& m)
{
	vector<face> faces = m.get_faces();
	vector<vec3> vertices = m.get_vertices();
	vector<vec3> texture_vertices = m.get_texture_vertices();
	vector<vec3> normals = m.get_normals();
	TGAImage texture = m.get_texture();

	texture_viewport[0][0] = texture.get_width();
	texture_viewport[1][1] = texture.get_height();
	texture_viewport[1][3] = -texture_viewport[1][1];

	for (auto i = faces.begin(); i != faces.end(); i++)
	{
		render_face(vertices[i->v[0]], vertices[i->v[1]], vertices[i->v[2]],
					texture_vertices[i->vt[0]], texture_vertices[i->vt[1]], texture_vertices[i->vt[2]],
					normals[i->vn[0]], normals[i->vn[1]], normals[i->vn[2]],
					texture);
	}
}

void renderer::set_view(vec3& center, vec3& camera, vec3& up)
{
	vec3 z = (camera - center).normalize();
	vec3 x = (up ^ z).normalize();
	vec3 y = (z ^ x).normalize();

	matrix4 M_inv = matrix4::identity();
	M_inv[0] = vec4(x[0], x[1], x[2], 0.0f);
	M_inv[1] = vec4(y[0], y[1], y[2], 0.0f);
	M_inv[2] = vec4(z[0], z[1], z[2], 0.0f);

	matrix4 transponse = matrix4::identity();
	transponse[0][3] = -center[0];
	transponse[1][3] = -center[1];
	transponse[2][3] = -center[2];

	view = M_inv * transponse;

	distance = (camera - center).norm();
	projection[3][2] = -1.0f / distance;

	M = M_inv.transponse();
}

void renderer::set_light(vec3& light)
{
	this->light = geometry::to_3d(M * vec4(light.x, light.y, light.z, 0.0f));
}

TGAImage& renderer::get_frame()
{
	return *frame;
}

void renderer::render_face(vec3& v1, vec3& v2, vec3& v3, vec3& vt1, vec3& vt2, vec3& vt3, vec3& vn1, vec3& vn2, vec3& vn3, TGAImage& texture)
{
	vec3 vs1 = vertex_shader(v1);
	vec3 vs2 = vertex_shader(v2);
	vec3 vs3 = vertex_shader(v3);

	if (!(((vs2 - vs1) ^ (vs3 - vs1)) * vec3(0.0f, 0.0f, -1.0f) < 0.0f))
	{
		return;
	}

	int x1 = (int)min(vs1.x, min(vs2.x, vs3.x));
	int x2 = (int)max(vs1.x, max(vs2.x, vs3.x));

	int y1 = (int)min(vs1.y, min(vs2.y, vs3.y));
	int y2 = (int)max(vs1.y, max(vs2.y, vs3.y));

	for (int x = x1; x <= x2; x++)
	{
		for (int y = y1; y <= y2; y++)
		{
			if (x < 0 || x >= width || y < 0 || y >= height)
			{
				continue;
			}

			vec3 b_s = geometry::barycentric_coords(vs1, vs2, vs3, vec3(x, y, 1.0f));

			if (b_s.x < 0.0f || b_s.x > 1.0f || b_s.y < 0.0f || b_s.y > 1.0f || b_s.z < 0.0f || b_s.z > 1.0f)
			{
				continue;
			}

			vec3 vz(v1.z, v2.z, v3.z);
			vec3 b_g = geometry::global_barycentric_coords(b_s, vz, projection[3][2]);
			int z = (int)(vec3(vs1.z, vs2.z, vs3.z) * b_g);

			if (z > zbuffer[x][y])
			{
				zbuffer[x][y] = z;
				frame->set(x, y, shader->fragment(b_g, vt1, vt2, vt3, vn1, vn2, vn3, texture, light));
			}
		}
	}
}

vec3 renderer::vertex_shader(vec3& vec)
{
	return geometry::to_3d(viewport * projection * view * geometry::to_4d(vec));
}