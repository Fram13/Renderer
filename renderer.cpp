#include "renderer.h"
#include <numeric>
#include <algorithm>
using namespace graphics;

matrix4 renderer::viewport = matrix4::identity();
matrix4 renderer::projection = matrix4::identity();
matrix4 renderer::view = matrix4::identity();
matrix4 renderer::light_view = matrix4::identity();
vec3 renderer::light = { 0.0f, 0.0f, 1.0f };

const int renderer::DEPTH = 200;
const int renderer::SBUFFER_CORRECTION = 4;
int renderer::width = 0;
int renderer::height = 0;
int** renderer::zbuffer = nullptr;
int** renderer::sbuffer = nullptr;
TGAImage* renderer::frame = nullptr;

void renderer::set_viewport(int width, int height)
{
	dispose();

	renderer::width = width;
	renderer::height = height;
	frame = new TGAImage(width, height, TGAImage::RGB);

	viewport[0][0] = viewport[0][3] = width / 2.0f;
	viewport[1][1] = viewport[1][3] = height / 2.0f;
	viewport[2][2] = viewport[2][3] = DEPTH / 2.0f;
	viewport[3][3] = 1.0f;

	zbuffer = new int*[height];
	sbuffer = new int*[height];

	for (int i = 0; i < height; i++)
	{
		zbuffer[i] = new int[width];
		sbuffer[i] = new int[width];

		for (int j = 0; j < width; j++)
		{
			zbuffer[i][j] = std::numeric_limits<int>::min();
			sbuffer[i][j] = std::numeric_limits<int>::min();
			frame->set(i, j, TGAColor(155, 155, 155, 255));
		}
	}
}

void renderer::set_view(vec3& center, vec3& camera, vec3& up)
{
	vec3 z = (camera - center).normalize();
	vec3 x = geometry::cross_product(up, z).normalize();
	vec3 y = geometry::cross_product(z, x).normalize();

	matrix4 M = { vec3::embed_vector(x), vec3::embed_vector(y), vec3::embed_vector(z), vec4{ 0.0f, 0.0f, 0.0f, 1.0f } };
	matrix4 T = matrix4::identity();
	T.set_column(vec3::embed_point(-center), 3);

	view = M.inverse() * T;

	projection[3][2] = -1.0f / (camera - center).norm();

	set_light(-light);
}

void renderer::set_light(vec3& light_pos)
{
	light = vec4::project((projection * view).transponse().inverse() * vec3::embed_vector(-light_pos)).normalize();
}

void renderer::render_models(std::vector<wavefront_model*>& models, shader* shdr)
{
	set_light_view();
	shdr->prepare();

	for (auto& model : models)
	{
		uint faces = model->faces_num();

		for (uint j = 0; j < faces; j++)
		{
			render_face_to_shadow_buffer(*model, j);
		}
	}

	for (auto& model : models)
	{
		uint faces = model->faces_num();

		for (uint j = 0; j < faces; j++)
		{
			render_face(*model, j, shdr);
		}
	}
}

void renderer::render_model(wavefront_model& model, shader*shdr)
{
	set_light_view();

	int faces = model.faces_num();

	for (int i = 0; i < faces; i++)
	{
		render_face_to_shadow_buffer(model, i);
	}

	for (int i = 0; i < faces; i++)
	{
		render_face(model, i, shdr);
	}
}

TGAImage& renderer::get_frame()
{
	return *frame;
}

void renderer::render_face(wavefront_model& model, int face_ind, shader* shdr)
{
	matrix<4, 3> vert4 = { shdr->vertex(model, face_ind, 0), shdr->vertex(model, face_ind, 1), shdr->vertex(model, face_ind, 2) };

	matrix3 scrn_vert;
	scrn_vert.set_column(vec4::project(viewport * vert4.get_column(0)), 0);
	scrn_vert.set_column(vec4::project(viewport * vert4.get_column(1)), 1);
	scrn_vert.set_column(vec4::project(viewport * vert4.get_column(2)), 2);

	vec3 a = scrn_vert.get_column(1) - scrn_vert.get_column(0);
	vec3 b = scrn_vert.get_column(2) - scrn_vert.get_column(0);

	if (!(geometry::cross_product(a, b).normalize()[2] > 0.0f))
	{
		return;
	}

	int min_x = (int)std::max(0.0f, std::min(scrn_vert[0][0], std::min(scrn_vert[0][1], scrn_vert[0][2])));
	int max_x = (int)std::min((float)(width - 1), std::max(scrn_vert[0][0], std::max(scrn_vert[0][1], scrn_vert[0][2])));
	int min_y = (int)std::max(0.0f, std::min(scrn_vert[1][0], std::min(scrn_vert[1][1], scrn_vert[1][2])));
	int max_y = (int)std::min((float)(height - 1), std::max(scrn_vert[1][0], std::max(scrn_vert[1][1], scrn_vert[1][2])));

	for (int x = min_x; x <= max_x; x++)
	{
		for (int y = min_y; y <= max_y; y++)
		{
			vec3 b_s = geometry::barycentric(scrn_vert, vec3({ (float)x, (float)y, 1.0f }));

			if (b_s[0] < 0.0f || b_s[0] > 1.0f || b_s[1] < 0.0f || b_s[1] > 1.0f || b_s[2] < 0.0f || b_s[2] > 1.0f)
			{
				continue;
			}

			vec3 b_g = { b_s[0] / vert4[3][0], b_s[1] / vert4[3][1], b_s[2] / vert4[3][2] };
			float m = 1.0f / (b_g[0] + b_g[1] + b_g[2]);
			b_g = b_g * m;

			int z = (int)((m - 1.0f) / projection[3][2] * DEPTH / 2.0f + DEPTH / 2.0f);

			if (zbuffer[x][y] < z)
			{
				zbuffer[x][y] = z;
				frame->set(x, y, shdr->fragment(model, b_g));
			}
		}
	}
}

void renderer::render_face_to_shadow_buffer(wavefront_model& model, int face_ind)
{
	face f = model.get_face(face_ind);

	matrix3 scrn_vert;
	scrn_vert.set_column(vec4::project(viewport * light_view * vec3::embed_point(model.vertex(f.v[0]))), 0);
	scrn_vert.set_column(vec4::project(viewport * light_view * vec3::embed_point(model.vertex(f.v[1]))), 1);
	scrn_vert.set_column(vec4::project(viewport * light_view * vec3::embed_point(model.vertex(f.v[2]))), 2);

	vec3 a = scrn_vert.get_column(1) - scrn_vert.get_column(0);
	vec3 b = scrn_vert.get_column(2) - scrn_vert.get_column(0);

	if (!(geometry::cross_product(a, b).normalize()[2] > 0.0f))
	{
		return;
	}

	int min_x = (int)std::max(0.0f, std::min(scrn_vert[0][0], std::min(scrn_vert[0][1], scrn_vert[0][2])));
	int max_x = (int)std::min((float)(width - 1), std::max(scrn_vert[0][0], std::max(scrn_vert[0][1], scrn_vert[0][2])));
	int min_y = (int)std::max(0.0f, std::min(scrn_vert[1][0], std::min(scrn_vert[1][1], scrn_vert[1][2])));
	int max_y = (int)std::min((float)(height - 1), std::max(scrn_vert[1][0], std::max(scrn_vert[1][1], scrn_vert[1][2])));

	for (int x = min_x; x <= max_x; x++)
	{
		for (int y = min_y; y <= max_y; y++)
		{
			vec3 b_s = geometry::barycentric(scrn_vert, vec3({ (float)x, (float)y, 1.0f }));

			if (b_s[0] < 0.0f || b_s[0] > 1.0f || b_s[1] < 0.0f || b_s[1] > 1.0f || b_s[2] < 0.0f || b_s[2] > 1.0f)
			{
				continue;
			}

			int z = (int)(scrn_vert.get_row(2) * b_s) - SBUFFER_CORRECTION;

			if (sbuffer[x][y] < z)
			{
				sbuffer[x][y] = z;
			}
		}
	}
}

void renderer::set_light_view()
{
	vec3 z = light;
	vec3 x = geometry::cross_product(vec3({ 0.0f, 1.0f, 0.0f}), z).normalize();
	vec3 y = geometry::cross_product(z, x).normalize();

	matrix4 M = { vec3::embed_vector(x), vec3::embed_vector(y), vec3::embed_vector(z), vec4({ 0.0f, 0.0f, 0.0f, 1.0f }) };

	light_view = M.inverse();
}

int renderer::get_shadow_buffer_value(int x, int y)
{
	if (x < 0 || x >= width || y < 0 || y >= height)
	{
		return std::numeric_limits<int>::min();
	}

	return sbuffer[x][y];
}

void renderer::dispose()
{
	if (frame != nullptr)
	{
		delete frame;
		frame = nullptr;
	}

	if (zbuffer != nullptr)
	{
		for (int i = 0; i < height; i++)
		{
			delete[] zbuffer[i];
		}

		delete[] zbuffer;
		zbuffer = nullptr;
	}

	if (sbuffer != nullptr)
	{
		for (int i = 0; i < height; i++)
		{
			delete[] sbuffer[i];
		}

		delete[] sbuffer;
		sbuffer = nullptr;
	}
}