#include "renderer.h"

matrix4 renderer::viewport = matrix4::identity();
matrix4 renderer::projection = matrix4::identity();
matrix4 renderer::view = matrix4::identity();
matrix4 renderer::transform = matrix4::identity();
vec3 renderer::light = { 0.0f, 0.0f, -1.0f };

const int renderer::DEPTH = 200;
int renderer::width = 0;
int renderer::height = 0;
int** renderer::zbuffer = nullptr;
float renderer::distance = -1.0f;
TGAImage* renderer::frame = nullptr;

void renderer::set_viewport(int w, int h)
{
	width = w;
	height = h;
	frame = new TGAImage(width, height, TGAImage::RGB);

	viewport[0][0] = viewport[0][3] = width / 2.0f;
	viewport[1][1] = viewport[1][3] = height / 2.0f;
	viewport[2][2] = viewport[2][3] = DEPTH / 2.0f;
	viewport[3][3] = 1.0f;

	zbuffer = new int*[height];

	for (int i = 0; i < height; i++)
	{
		zbuffer[i] = new int[width];

		for (int j = 0; j < width; j++)
		{
			zbuffer[i][j] = std::numeric_limits<int>::min();

			//background color
			frame->set(i, j, TGAColor(155, 155, 155, 255));
		}
	}
}

void renderer::set_view(vec3& center, vec3& camera, vec3& up)
{
	vec3 z = (camera - center).normalize();
	vec3 x = vec3::cross_product(up, z).normalize();
	vec3 y = vec3::cross_product(z, x).normalize();

	matrix4 M = { vec3::embed_vector(x), vec3::embed_vector(y), vec3::embed_vector(z), vec4({ 0.0f, 0.0f, 0.0f, 1.0f }) };
	matrix4 T = matrix4::identity();
	T.set_column(vec3::embed_point(center * -1.0f), 3);

	view = M.inverse() * T;
	transform = M.transponse().inverse();
	set_light(light * -1.0f);

	distance = (camera - center).norm();
	projection[3][2] = -1.0f / distance;
}

void renderer::set_light(vec3& l)
{
	light = vec4::project(transform * vec3::embed_vector(l.normalize() * -1.0f));
}

void renderer::render_model(wavefront_model& model, shader*shdr)
{
	int faces = model.faces_num();

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
	matrix3 vert = shdr->vertex(model, face_ind).transponse();

	if (!(vec3::cross_product(vert[1] - vert[0], vert[2] - vert[0]).normalize()[2] > 0.0f))
	{
		return;
	}

	int min_x = (int)std::max(0.0f, std::min(vert[0][0], std::min(vert[1][0], vert[2][0])));
	int max_x = (int)std::min((float)(width - 1), std::max(vert[0][0], std::max(vert[1][0], vert[2][0])));
	int min_y = (int)std::max(0.0f, std::min(vert[0][1], std::min(vert[1][1], vert[2][1])));
	int max_y = (int)std::min((float)(height - 1), std::max(vert[0][1], std::max(vert[1][1], vert[2][1])));

	for (int x = min_x; x <= max_x; x++)
	{
		for (int y = min_y; y <= max_y; y++)
		{
			vec3 b_s = geometry::barycentric(vert, vec3({ (float)x, (float)y, 1.0f }));

			if (b_s[0] < 0.0f || b_s[0] > 1.0f || b_s[1] < 0.0f || b_s[1] > 1.0f || b_s[2] < 0.0f || b_s[2] > 1.0f)
			{
				continue;
			}

			vec3 b_g = geometry::global_barycentric(b_s, vert, projection[3][2]);

			int z = (int)(b_g[0] * vert[0][2] + b_g[1] * vert[1][2] + b_g[2] * vert[2][2]);

			if (zbuffer[x][y] < z)
			{
				zbuffer[x][y] = z;
				frame->set(x, y, shdr->fragment(model, b_g));
			}
		}
	}
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
}