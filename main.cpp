#include "renderer.h"

class shader1 : public shader
{
public:
	matrix3 vt;

	virtual matrix3 vertex(wavefront_model& model, int face_ind) override
	{
		face f = model.get_face(face_ind);

		vt = matrix3({ model.texture_vertex(f.vt[0]), model.texture_vertex(f.vt[1]), model.texture_vertex(f.vt[2]) });
		matrix3 texture_viewport = matrix3::identity();
		texture_viewport[0][0] = (float)model.map_width();
		texture_viewport[1][2] = (float)model.map_height();
		texture_viewport[1][1] = -texture_viewport[1][2];
		vt = texture_viewport * vt;

		matrix<4, 3> vert = { vec3::embed_point(model.vertex(f.v[0])), vec3::embed_point(model.vertex(f.v[1])), vec3::embed_point(model.vertex(f.v[2])) };
		vert = renderer::viewport * renderer::projection * renderer::view * vert;
		return matrix3({ vec4::project(vert.get_column(0)), vec4::project(vert.get_column(1)), vec4::project(vert.get_column(2)) });
	}

	virtual TGAColor fragment(wavefront_model& m, vec3& bar) override
	{
		vec3 pos = vt.get_column(0) * bar[0] + vt.get_column(1) * bar[1] + vt.get_column(2) * bar[2];
		TGAColor nc = m.normal_map(pos);
		vec3 n = { (nc.r - 127.0f) / 128.0f, (nc.g - 127.0f) / 128.0f, (nc.b - 127.0f) / 128.0f };
		n = vec4::project(renderer::transform * vec3::embed_vector(n)).normalize();
		vec3 r = (n * (n * renderer::light * 2.0f) - renderer::light).normalize();
		float diffuse = std::max(0.0f, n * renderer::light);
		float exp = m.specular_map(pos).r;
		float specular = pow(std::max(0.0f, r[2]), exp);
		float ambient = 5.0f;
		
		TGAColor color = m.texture_map(pos);
		color.r = (unsigned char)(std::min(ambient + color.r * (diffuse + 0.6f * specular), 255.0f));
		color.g = (unsigned char)(std::min(ambient + color.g * (diffuse + 0.6f * specular), 255.0f));
		color.b = (unsigned char)(std::min(ambient + color.b * (diffuse + 0.6f * specular), 255.0f));

		return color;
	}
} shdr1;

int main()
{
	wavefront_model head("models\\african_head\\african_head.obj");
	wavefront_model eye_inner("models\\african_head\\african_head_eye_inner.obj");

	renderer::set_viewport(800, 800);
	renderer::set_light(vec3({ 10.0f, -20.0f, -20.0f }));
	renderer::set_view(vec3({ 0.0f, 0.0f, 0.0f }), vec3({ 1.0f, 1.0f, 5.0f }), vec3({ 0.0f, 1.0f, 0.0f }));
	
	renderer::render_model(head, &shdr1);
	renderer::render_model(eye_inner, &shdr1);

	TGAImage& frame = renderer::get_frame();
	frame.flip_vertically();
	frame.write_tga_file("frames\\frame.tga");
	renderer::dispose();
	system("frames\\frame.tga");
	return 0;
}