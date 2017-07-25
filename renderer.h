#pragma once

#include <numeric>
#include "wavefront_model.h"
#include "shader.h"

namespace graphics
{
	class renderer
	{
	public:
		static matrix4 viewport, projection, view;
		static vec3 light;
		static const int DEPTH;

		static void set_viewport(int width, int height);
		static void set_view(vec3& center, vec3& camera, vec3& up);
		static void set_light(vec3& light_pos);

		static void render_model(wavefront_model& model, shader* shdr);

		static TGAImage& get_frame();

		static void dispose();
		static matrix4 light_view_matrix();
		static int get_shadow_buffer_value(int x, int y);

	private:
		static int width, height;
		static int** zbuffer;
		static int** sbuffer;

		static TGAImage* frame;

		static void render_face(wavefront_model& model, int face_ind, shader* shdr);
		static void render_face_to_shadow_buffer(wavefront_model& model, int face_ind);
	};
}