#pragma once

#include <initializer_list>
#include <stdexcept>
#include <math.h>

namespace graphics
{
	using uint = unsigned int;

	template <uint SIZE>
	struct vector
	{
		static_assert(SIZE > 1, "Vector size must be greater than one (at vector<SIZE>)");

	private:
		static const float E;

	public:
		float raw[SIZE];

		explicit vector() = default;
		vector(const vector<SIZE>& other) = default;
		vector<SIZE>& operator=(const vector<SIZE>& other) = default;
		~vector() = default;

		friend vector<SIZE> operator+(const vector<SIZE>& left, const vector<SIZE>& right) noexcept
		{
			vector<SIZE> res;

			for (uint i = 0; i < SIZE; i++)
			{
				res.raw[i] = left.raw[i] + right.raw[i];
			}

			return res;
		}

		friend vector<SIZE> operator-(const vector<SIZE>& left) noexcept
		{
			return left * (-1.0f);
		}

		friend vector<SIZE> operator-(const vector<SIZE>& left, const vector<SIZE>& right) noexcept
		{
			vector<SIZE> res;

			for (uint i = 0; i < SIZE; i++)
			{
				res.raw[i] = left.raw[i] - right.raw[i];
			}

			return res;
		}

		friend float operator*(const vector<SIZE>& left, const vector<SIZE>& right) noexcept
		{
			float res = 0.0f;

			for (uint i = 0; i < SIZE; i++)
			{
				res += left.raw[i] * right.raw[i];
			}

			return res;
		}

		friend vector<SIZE> operator*(const vector<SIZE>& left, float factor) noexcept
		{
			vector<SIZE> res;

			for (uint i = 0; i < SIZE; i++)
			{
				res.raw[i] = left.raw[i] * factor;
			}

			return res;
		}

		friend vector<SIZE> operator*(float factor, const vector<SIZE>& left) noexcept
		{
			return left * factor;
		}

		friend vector<SIZE> operator/(const vector<SIZE>& left, float factor)
		{
			if (abs(factor) < vector<SIZE>::E)
			{
				throw std::invalid_argument("Dividing by zero.");
			}

			vector<SIZE> res;

			for (uint i = 0; i < SIZE; i++)
			{
				res.raw[i] = left.raw[i] / factor;
			}

			return res;
		}

		float& operator[](uint ind)
		{
			if (ind >= SIZE)
			{
				throw std::out_of_range("Index is out of range.");
			}

			return raw[ind];
		}

		const float& operator[](uint ind) const
		{
			if (ind >= SIZE)
			{
				throw std::out_of_range("Index is out of range.");
			}

			return raw[ind];
		}

		float norm() const noexcept
		{
			float res = 0.0f;

			for (uint i = 0; i < SIZE; i++)
			{
				res += raw[i] * raw[i];
			}

			return sqrt(res);
		}

		vector<SIZE> normalize() const noexcept
		{
			float length = norm();

			if (length > E)
			{
				return (*this) / length;
			}
			else
			{
				return *this;
			}
		}

		static vector<SIZE + 1> embed_point(vector<SIZE>& vec) noexcept;

		static vector<SIZE + 1> embed_vector(vector<SIZE>& vec) noexcept;

		static vector<SIZE - 1> project(vector<SIZE>& vec) noexcept;
	};

	template <uint SIZE>
	const float vector<SIZE>::E = 1e-6f;

	template <uint SIZE>
	vector<SIZE + 1> vector<SIZE>::embed_point(vector<SIZE>& vec) noexcept
	{
		vector<SIZE + 1> res = vector<SIZE>::embed_vector(vec);

		res[SIZE] = 1.0f;

		return res;
	}

	template <uint SIZE>
	vector<SIZE + 1> vector<SIZE>::embed_vector(vector<SIZE>& vec) noexcept
	{
		vector<SIZE + 1> res;

		for (uint i = 0; i < SIZE; i++)
		{
			res[i] = vec.raw[i];
		}

		return res;
	}

	template <uint SIZE>
	vector<SIZE - 1> vector<SIZE>::project(vector<SIZE>& vec) noexcept
	{
		vector<SIZE - 1> res;
		float factor = abs(vec[SIZE - 1]) > vector<SIZE>::E ? vec[SIZE - 1] : 1.0f;

		for (uint i = 0; i < SIZE - 1; i++)
		{
			res[i] = vec.raw[i] / factor;
		}

		return res;
	}
}
