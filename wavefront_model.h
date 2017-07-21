#pragma once

#include "geometry.h"
#include "face.h"
#include "tgaimage.h"
#include <vector>

namespace graphics
{
	class wavefront_model
	{
	public:
		wavefront_model(std::string path);
		~wavefront_model();

		face get_face(int ind);
		vec3 vertex(int ind);
		vec3 texture_vertex(int ind);
		vec3 normal(int ind);

		TGAColor texture_map(vec3& coords);
		vec3 normal_map(vec3& coords);
		vec3 normal_tangent_map(vec3& coords);
		float specular_map(vec3& coords);

		int faces_num();

	private:
		std::vector<face> faces;
		std::vector<vec3> vertices;
		std::vector<vec3> texture_vertices;
		std::vector<vec3> normals;

		TGAImage* _texture_map;
		TGAImage* _normal_map;
		TGAImage* _normal_tangent_map;
		TGAImage* _specular_map;

		//matrix4 model;

		void read_obj_file(std::string path);
		TGAImage* load_map(std::string path);

		/*void rotate(float angle, vec3& vec);
		void scale(vec3& vec);
		void translate(vec3& vec);*/
	};
}