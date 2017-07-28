#pragma once

#include "renderer.h"
#include <algorithm>

namespace graphics
{
	class main_shader : public shader
	{
	private:
		matrix3 v;
		matrix3 vl;
		matrix3 vt;
		matrix3 vn;

	public:
		virtual vec4 vertex(wavefront_model& model, int face_ind, int vert_ind)
		{
			face f = model.get_face(face_ind);

			vec4 v4 = vec3::embed_point(model.vertex(f.v[vert_ind]));

			vl.set_column(vec4::project(renderer::viewport * renderer::light_view * v4), vert_ind);

			v4 = renderer::projection * renderer::view * v4;
			v.set_column(vec4::project(v4), vert_ind);

			vt.set_column(model.texture_vertex(f.vt[vert_ind]), vert_ind);

			vec4 vn4 = vec3::embed_vector(model.normal(f.vn[vert_ind]));
			vn4 = (renderer::projection * renderer::view).transponse().inverse() * vn4;
			vn.set_column(vec4::project(vn4), vert_ind);

			return v4;
		}

		virtual TGAColor fragment(wavefront_model& model, vec3& barycentric)
		{
			vec3 uv = vt * barycentric;
			vec3 k = (vn * barycentric).normalize();

			matrix3 M;
			M.set_row(v.get_column(1) - v.get_column(0), 0);
			M.set_row(v.get_column(2) - v.get_column(0), 1);
			M.set_row(k, 2);

			matrix3 M_inv = M.inverse();

			vec3 i = M_inv * vec3({ vt[0][1] - vt[0][0], vt[0][2] - vt[0][0], 0.0f });
			vec3 j = M_inv * vec3({ vt[1][1] - vt[1][0], vt[1][2] - vt[1][0], 0.0f });

			matrix3 B = { i.normalize(), j.normalize(), k };

			vec3 n = (B * model.normal_tangent_map(uv)).normalize();
			vec3 r = (n * (n * renderer::light * 2.0f) - renderer::light).normalize();
			vec3 vert_light = vl * barycentric;

			float specular = pow(std::max(0.0f, r[2]), model.specular_map(uv));
			float diffuse = std::max(n * renderer::light, 0.0f);
			float ambient = 5.0f;
			float shadow = 1.0f;

			if ((int)vert_light[2] < renderer::get_shadow_buffer_value((int)vert_light[0], (int)vert_light[1]))
			{
				shadow = 0.3f;
			}

			TGAColor color = model.texture_map(uv);

			color.r = (unsigned char)std::min((ambient + color.r * shadow * (diffuse + 0.6f * specular)), 255.0f);
			color.g = (unsigned char)std::min((ambient + color.g * shadow * (diffuse + 0.6f * specular)), 255.0f);
			color.b = (unsigned char)std::min((ambient + color.b * shadow * (diffuse + 0.6f * specular)), 255.0f);

			return color;
		}
	} main_shdr;
}