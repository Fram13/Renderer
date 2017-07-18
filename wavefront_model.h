#pragma once

#include "geometry.h"
#include "face.h"
#include "tgaimage.h"
#include <vector>

class wavefront_model
{
public:
	wavefront_model(std::string path);
	~wavefront_model();

	face get_face(int ind);
	vec3 vertex(int ind);
	vec3 texture_vertex(int ind);
	vec3 normal(int ind);

	TGAColor get_texture(vec3& coords);
	TGAColor get_normal(vec3& coords);

	int map_width();
	int map_height();

	int faces_num();

private:
	std::vector<face> faces;
	std::vector<vec3> vertices;
	std::vector<vec3> texture_vertices;
	std::vector<vec3> normals;

	TGAImage* texture_map;
	TGAImage* normal_map;

	matrix4 model;

	void read_obj_file(std::string path);
	TGAImage* load_map(std::string path);

	/*void rotate(float angle, vec3& vec);
	void scale(vec3& vec);
	void translate(vec3& vec);*/
};