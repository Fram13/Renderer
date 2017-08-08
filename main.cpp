#include "renderer.h"
#include "main_shader.h"
using namespace graphics;

int main()
{
	wavefront_model head("models\\african_head\\african_head.obj");
	wavefront_model eye_inner("models\\african_head\\african_head_eye_inner.obj");
	wavefront_model floor("models\\floor\\floor.obj");

	renderer::set_viewport(800, 800);
	renderer::set_light(vec3({ -3.0f, -1.0f, -2.0f }));
	renderer::set_view(vec3({ 0.0f, 0.0f, 0.0f }), vec3({ 1.0f, 1.0f, 5.0f }), vec3({ 0.0f, 1.0f, 0.0f }));

	std::vector<std::reference_wrapper<wavefront_model>> models;
	models.push_back(std::ref(eye_inner));
	models.push_back(std::ref(head));
	models.push_back(std::ref(floor));

	renderer::render_models(models, &main_shdr);
	
	TGAImage& frame = renderer::get_frame();
	frame.flip_vertically();
	frame.write_tga_file("frames\\african_head.tga");
	renderer::dispose();
	system("frames\\african_head.tga");
	return 0;
}