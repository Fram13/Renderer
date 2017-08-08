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
		explicit wavefront_model(const std::string& path);
		wavefront_model(const wavefront_model& other) = default;
		~wavefront_model();

		wavefront_model& operator=(const wavefront_model& other) = delete;

		face get_face(int ind) const;
		vec3 vertex(int ind) const;
		vec3 texture_vertex(int ind) const;
		vec3 normal(int ind) const;

		TGAColor texture_map(const vec3& coords) const;
		vec3 normal_map(const vec3& coords) const;
		vec3 normal_tangent_map(const vec3& coords) const;
		float specular_map(const vec3& coords) const;

		int faces_num() const noexcept;

		//void rotate(float angle, const vec3& vec) noexcept;
		void scale(const vec3& vec) noexcept;
		void translate(const vec3& vec) noexcept;

	private:
		std::vector<face> faces;
		std::vector<vec3> vertices;
		std::vector<vec3> texture_vertices;
		std::vector<vec3> normals;

		TGAImage* _texture_map;
		TGAImage* _normal_map;
		TGAImage* _normal_tangent_map;
		TGAImage* _specular_map;

		void read_obj_file(const std::string& path);
		TGAImage* load_map(const std::string& path);
	};
}