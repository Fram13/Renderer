#pragma once

#include "wavefront_model.h"
#include "shader.h"

namespace graphics
{
	class renderer
	{
	public:
		renderer() = delete;

		static matrix4 viewport, projection, view, light_view;
		static vec3 light;

		static void set_viewport(int width, int height);
		static void set_view(const vec3& center, const vec3& camera, const vec3& up) noexcept;
		static void set_light(const vec3& light_pos) noexcept;

		static void render_models(const std::vector<std::reference_wrapper<wavefront_model>>& models, shader* shdr) noexcept;

		static TGAImage& get_frame();

		static void dispose();
		
		static int get_shadow_buffer_value(int x, int y) noexcept;

	private:
		static int width, height;
		static int** zbuffer;
		static int** sbuffer;
		static const int DEPTH;
		static const int SBUFFER_CORRECTION;

		static TGAImage* frame;

		static void set_light_view() noexcept;
		static void render_face(const wavefront_model& model, int face_ind, shader* shdr) noexcept;
		static void render_face_to_shadow_buffer(const wavefront_model& model, int face_ind) noexcept;
	};
}