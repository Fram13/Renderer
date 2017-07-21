#pragma once

#include <initializer_list>
#include <algorithm>
#include <stdexcept>
#include <math.h>

namespace graphics
{
	template <int SIZE>
	class vector
	{
	public:
		static_assert(SIZE > 1, "Vector size must be greater than one (at vector<SIZE>)");

		vector()
		{
			for (int i = 0; i < SIZE; i++)
			{
				raw[i] = 0.0f;
			}
		}

		vector(std::initializer_list<float> init_list)
		{
			auto iter = init_list.begin();
			int i = 0;

			while (i < SIZE && iter != init_list.end())
			{
				raw[i] = *iter;
				i++;
				iter++;
			}

			while (i < SIZE)
			{
				raw[i] = 0.0f;
				i++;
			}
		}

		vector<SIZE> add(vector<SIZE>& other)
		{
			vector<SIZE> res;

			for (int i = 0; i < SIZE; i++)
			{
				res[i] = raw[i] + other[i];
			}

			return res;
		}

		vector<SIZE> subtract(vector<SIZE>& other)
		{
			vector<SIZE> res;

			for (int i = 0; i < SIZE; i++)
			{
				res[i] = raw[i] - other[i];
			}

			return res;
		}

		vector<SIZE> multiply(float scalar)
		{
			vector<SIZE> res;

			for (int i = 0; i < SIZE; i++)
			{
				res[i] = raw[i] * scalar;
			}

			return res;
		}

		float dot_product(vector<SIZE>& other)
		{
			float res = 0.0f;

			for (int i = 0; i < SIZE; i++)
			{
				res += raw[i] * other[i];
			}

			return res;
		}

		float norm()
		{
			float res = 0.0f;

			for (int i = 0; i < SIZE; i++)
			{
				res += raw[i] * raw[i];
			}

			return sqrt(res);
		}

		vector<SIZE> normalize()
		{
			float length = norm();

			if (length > E)
			{
				return multiply(1.0f / length);
			}
			else
			{
				return *this;
			}
		}

		vector<SIZE> operator+(vector<SIZE>& other)
		{
			return add(other);
		}

		vector<SIZE> operator-()
		{
			return multiply(-1.0f);
		}

		vector<SIZE> operator-(vector<SIZE>& other)
		{
			return subtract(other);
		}

		vector<SIZE> operator*(float scalar)
		{
			return multiply(scalar);
		}

		float operator*(vector<SIZE>& other)
		{
			return dot_product(other);
		}

		float& operator[](int ind)
		{
			if (ind < 0 || ind >= SIZE)
			{
				throw std::out_of_range("Index is out of range.");
			}

			return raw[ind];
		}

		vector<SIZE>& operator=(vector<SIZE>& other)
		{
			for (int i = 0; i < SIZE; i++)
			{
				raw[i] = other[i];
			}

			return *this;
		}

		static vector<SIZE + 1> embed_point(vector<SIZE>& vec);

		static vector<SIZE + 1> embed_vector(vector<SIZE>& vec);

		static vector<SIZE - 1> project(vector<SIZE>& vec);

		static vector<SIZE> cross_product(vector<SIZE>& vec1, vector<SIZE>& vec2);

	private:
		float raw[SIZE];
		static const float E;
	};

	template <int SIZE>
	const float vector<SIZE>::E = 1e-6f;

	template <int SIZE>
	vector<SIZE + 1> vector<SIZE>::embed_point(vector<SIZE>& vec)
	{
		vector<SIZE + 1> res = vector<SIZE>::embed_vector(vec);

		res[SIZE] = 1.0f;

		return res;
	}

	template <int SIZE>
	vector<SIZE + 1> vector<SIZE>::embed_vector(vector<SIZE>& vec)
	{
		vector<SIZE + 1> res;

		for (int i = 0; i < SIZE; i++)
		{
			res[i] = vec[i];
		}

		return res;
	}

	template <int SIZE>
	vector<SIZE - 1> vector<SIZE>::project(vector<SIZE>& vec)
	{
		vector<SIZE - 1> res;
		float m = abs(vec[SIZE - 1]) > vector<SIZE>::E ? vec[SIZE - 1] : 1.0f;

		for (int i = 0; i < SIZE - 1; i++)
		{
			res[i] = vec[i] / m;
		}

		return res;
	}

	template <int SIZE>
	vector<SIZE> vector<SIZE>::cross_product(vector<SIZE>& vec1, vector<SIZE>& vec2)
	{
		static_assert(SIZE == 3, "Cross product can only be computed in a three-dimensional space (at vector<SIZE>::cross_product(vector<SIZE>& vec1, vector<SIZE>& vec2))");

		return vector<SIZE>({ vec1[1] * vec2[2] - vec2[1] * vec1[2], -(vec1[0] * vec2[2] - vec2[0] * vec1[2]), vec1[0] * vec2[1] - vec2[0] * vec1[1] });
	}
}