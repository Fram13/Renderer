#include "renderer.h"
#include "phong_nm_shader.h"
#include "phong_tangent_nm_shader.h"
using namespace graphics;

int main()
{
	wavefront_model head("models\\african_head\\african_head.obj");
	wavefront_model eye_inner("models\\african_head\\african_head_eye_inner.obj");

	renderer::set_viewport(800, 800);
	renderer::set_light(vec3({ 0.5f, -1.0f, -1.0f }));
	renderer::set_view(vec3({ 0.0f, 0.0f, 0.0f }), vec3({ 5.0f, 1.0f, 3.0f }), vec3({ 0.0f, 1.0f, 0.0f }));

	renderer::render_model(head, &phong_tangent_nm_shdr);
	renderer::render_model(eye_inner, &phong_tangent_nm_shdr);

	TGAImage& frame = renderer::get_frame();
	frame.flip_vertically();
	frame.write_tga_file("frames\\african_head.tga");
	renderer::dispose();
	system("frames\\african_head.tga");
	return 0;
}