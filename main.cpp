#include "renderer.h"
#include "main_shader.h"
#include "no_shdows_shader.h"
using namespace graphics;

int main()
{
	/*wavefront_model head("models\\african_head\\african_head.obj");
	wavefront_model eye_inner("models\\african_head\\african_head_eye_inner.obj");*/
	wavefront_model floor("models\\floor\\floor.obj");
	wavefront_model diablo("models\\diablo3_pose\\diablo3_pose.obj");

	renderer::set_viewport(800, 800);
	renderer::set_light(vec3({ 3.0f, -1.0f, 1.0f }));
	renderer::set_view(vec3({ 0.0f, 0.0f, 0.0f }), vec3({ 0.0f, 0.0f, 5.0f }), vec3({ 0.0f, 1.0f, 0.0f }));

	/*renderer::render_model(head, &main_shdr);
	renderer::render_model(eye_inner, &main_shdr);*/
	renderer::render_model(diablo, &main_shdr);
	renderer::render_model(floor, &main_shdr);

	TGAImage& frame = renderer::get_frame();
	frame.flip_vertically();
	frame.write_tga_file("frames\\shadows.tga");
	renderer::dispose();
	system("frames\\shadows.tga");
	return 0;
}