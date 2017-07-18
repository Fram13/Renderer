#pragma once

#include <numeric>
#include "wavefront_model.h"
#include "shader.h"

class renderer
{
public:
	static matrix4 viewport, projection, view, transform;
	static vec3 light;

	static void set_viewport(int w, int h);
	static void set_view(vec3& center, vec3& camera, vec3& up);
	static void set_light(vec3& l);

	static void render_model(wavefront_model& model, shader* shdr);

	static TGAImage& get_frame();

	static void dispose();

private:
	static const int DEPTH;
	static int width, height;
	static int** zbuffer;
	static float distance;

	static TGAImage* frame;
	
	static void render_face(wavefront_model& model, int face_ind, shader* shdr);
};