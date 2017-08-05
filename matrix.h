#pragma once

#include "vector.h"

namespace graphics
{
	template <uint ROWS, uint COLUMNS>
	class matrix
	{
		static_assert(ROWS > 1, "Matrix rows number must be greater than one (at matrix<ROWS, COLUMNS>)");
		static_assert(COLUMNS > 1, "Matrix columns number must be greater than one (at matrix<ROWS, COLUMNS>)");

	private:
		vector<COLUMNS> raw[ROWS];
		static const float E;

		void assign(const matrix<ROWS, COLUMNS>& other) noexcept
		{
			for (uint i = 0; i < ROWS; i++)
			{
				raw[i] = other.raw[i];
			}
		}

	public:
		explicit matrix() noexcept
		{

		}

		matrix(std::initializer_list<std::initializer_list<float>> init_list) noexcept
		{
			auto rows_iter = init_list.begin();
			uint i = 0;

			while (i < ROWS && rows_iter != init_list.end())
			{
				auto columns_iter = rows_iter->begin();
				uint j = 0;

				while (j < COLUMNS && columns_iter != rows_iter->end())
				{
					raw[i][j] = *columns_iter;
					columns_iter++;
					j++;
				}

				rows_iter++;
				i++;
			}
		}

		matrix(std::initializer_list<vector<ROWS>> init_list) noexcept
		{
			auto iter = init_list.begin();
			uint j = 0;

			while (j < COLUMNS && iter != init_list.end())
			{
				vector<ROWS> vec = *iter;
				set_column(vec, j);
				iter++;
				j++;
			}
		}

		matrix(const matrix<ROWS, COLUMNS>& other) noexcept
		{
			assign(other);
		}

		matrix(const matrix<ROWS, COLUMNS>&& other) noexcept
		{
			assign(other);
		}

		~matrix() noexcept
		{

		}

		matrix<ROWS, COLUMNS>& operator=(const matrix<ROWS, COLUMNS>& other) noexcept
		{
			assign(other);
			return *this;
		}

		matrix<ROWS, COLUMNS>& operator=(const matrix<ROWS, COLUMNS>&& other) noexcept
		{
			assign(other);
			return *this;
		}

		friend vector<ROWS> operator*(const matrix<ROWS, COLUMNS>& left, const vector<COLUMNS>& right) noexcept
		{
			vector<ROWS> res;

			for (uint i = 0; i < ROWS; i++)
			{
				res[i] = left.raw[i] * right;
			}

			return res;
		}

		template<uint OTHER_COLUMNS>
		friend matrix<ROWS, OTHER_COLUMNS> operator*(const matrix<ROWS, COLUMNS>& left, const matrix<COLUMNS, OTHER_COLUMNS>& right) noexcept
		{
			matrix<ROWS, OTHER_COLUMNS> res;
			matrix<OTHER_COLUMNS, COLUMNS> right_t = right.transponse();

			for (uint i = 0; i < ROWS; i++)
			{
				for (uint j = 0; j < OTHER_COLUMNS; j++)
				{
					res[i][j] = left.raw[i] * right_t.raw[j];
				}
			}

			return res;
		}

		vector<COLUMNS>& operator[](uint ind)
		{
			if (ind >= ROWS)
			{
				throw std::out_of_range("Index is out of range.");
			}

			return raw[ind];
		}

		const vector<COLUMNS>& operator[](uint ind) const
		{
			if (ind >= ROWS)
			{
				throw std::out_of_range("Index is out of range.");
			}

			return raw[ind];
		}

		vector<COLUMNS> get_row(uint ind) const
		{
			if (ind >= ROWS)
			{
				throw std::out_of_range("Index is out of range.");
			}

			return raw[ind];
		}

		void set_row(vector<COLUMNS>& row, uint ind)
		{
			if (ind >= ROWS)
			{
				throw std::out_of_range("Index is out of range.");
			}

			raw[ind] = row;
		}

		vector<ROWS> get_column(uint ind) const
		{
			if (ind >= COLUMNS)
			{
				throw std::out_of_range("Index is out of range.");
			}

			vector<ROWS> res;

			for (uint i = 0; i < ROWS; i++)
			{
				res.raw[i] = raw[i].raw[ind];
			}

			return res;
		}

		void set_column(vector<ROWS>& column, uint ind)
		{
			if (ind >= COLUMNS)
			{
				throw std::out_of_range("Index is out of range.");
			}

			for (uint i = 0; i < ROWS; i++)
			{
				raw[i][ind] = column.raw[i];
			}
		}

		matrix<COLUMNS, ROWS> transponse() const noexcept
		{
			matrix<COLUMNS, ROWS> res;

			for (uint i = 0; i < ROWS; i++)
			{
				for (uint j = 0; j < COLUMNS; j++)
				{
					res.raw[j][i] = raw[i][j];
				}
			}

			return res;
		}

		matrix<ROWS, COLUMNS> inverse() const
		{
			static_assert(ROWS == COLUMNS, "Inverse matrix must be square (at matrix<ROWS, COLUMNS>.inverse())");

			matrix<ROWS, COLUMNS> matr = *this;
			matrix<ROWS, COLUMNS> res = identity();

			for (uint i = 0; i < ROWS; i++)
			{
				if (abs(matr[i][i]) < E)
				{
					bool inverse_exists = false;

					for (uint j = i + 1; j < ROWS; j++)
					{
						if (abs(matr[j][i]) > E)
						{
							vector<COLUMNS> row = matr[i];
							matr[i] = matr[j];
							matr[j] = row;

							row = res[i];
							res[i] = res[j];
							res[j] = row;
							
							inverse_exists = true;
							break;
						}
					}

					if (!inverse_exists)
					{
						throw std::invalid_argument("Inverse matrix does not exist.");
					}
				}

				float factor = 1.0f / matr[i][i];
				matr[i] = matr[i] * factor;
				res[i] = res[i] * factor;

				for (uint j = 0; j < COLUMNS; j++)
				{
					if (j != i)
					{
						factor = matr[j][i];
						matr[j] = matr[j] - matr[i] * factor;
						res[j] = res[j] - res[i] * factor;
					}
				}
			}

			return res;
		}

		static matrix<ROWS, COLUMNS> identity() noexcept;
	};

	template <uint ROWS, uint COLUMNS>
	const float matrix<ROWS, COLUMNS>::E = 1e-6f;

	template <uint ROWS, uint COLUMNS>
	matrix<ROWS, COLUMNS> matrix<ROWS, COLUMNS>::identity() noexcept
	{
		static_assert(ROWS == COLUMNS, "Identity matrix must be square (at matrix<ROWS, COLUMNS>::identity())");

		matrix<ROWS, COLUMNS> res;

		for (uint i = 0; i < ROWS; i++)
		{
			res[i][i] = 1.0f;
		}

		return res;
	}
}