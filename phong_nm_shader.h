#pragma once

#include "renderer.h"
using namespace graphics;

class phong_nm_shader : public shader
{
public:
	matrix3 vt;

	virtual vec3 vertex(wavefront_model& model, int face_ind, int vert_ind) override
	{
		face f = model.get_face(face_ind);
		vt.set_column(model.texture_vertex(f.vt[vert_ind]), vert_ind);

		vec4 v = vec3::embed_point(model.vertex(f.v[vert_ind]));
		return vec4::project(renderer::viewport * renderer::projection * renderer::view * v);
	}

	virtual TGAColor fragment(wavefront_model& m, vec3& bar) override
	{
		vec3 uv = vt * bar;
		vec3 n = m.normal_map(uv);
		n = vec4::project((renderer::projection * renderer::view).transponse().inverse() * vec3::embed_vector(n)).normalize();

		vec3 r = (n * (n * renderer::light * 2.0f) - renderer::light).normalize();
		float diffuse = std::max(0.0f, n * renderer::light);
		float specular = pow(std::max(0.0f, r[2]), m.specular_map(uv));
		float ambient = 5.0f;

		TGAColor color = m.texture_map(uv);
		color.r = (unsigned char)(std::min(ambient + color.r * (diffuse + 0.6f * specular), 255.0f));
		color.g = (unsigned char)(std::min(ambient + color.g * (diffuse + 0.6f * specular), 255.0f));
		color.b = (unsigned char)(std::min(ambient + color.b * (diffuse + 0.6f * specular), 255.0f));

		return color;
	}
} phong_nm_shdr;