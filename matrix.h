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

	public:
		explicit matrix() = default;

		matrix(const std::initializer_list<vector<ROWS>> init_list) noexcept
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

		matrix(const matrix<ROWS, COLUMNS>& other) = default;

		~matrix() = default;

		matrix<ROWS, COLUMNS>& operator=(const matrix<ROWS, COLUMNS>& other) = default;

		vector<ROWS> operator*(const vector<COLUMNS>& right) const noexcept
		{
			vector<ROWS> res;

			for (uint i = 0; i < ROWS; i++)
			{
				res.raw[i] = this->raw[i] * right;
			}

			return res;
		}

		template<uint OTHER_COLUMNS>
		matrix<ROWS, OTHER_COLUMNS> operator*(const matrix<COLUMNS, OTHER_COLUMNS>& right) const noexcept
		{
			matrix<ROWS, OTHER_COLUMNS> res;
			matrix<OTHER_COLUMNS, COLUMNS> right_tr = right.transponse();

			for (uint i = 0; i < ROWS; i++)
			{
				for (uint j = 0; j < OTHER_COLUMNS; j++)
				{
					res.raw[i].raw[j] = this->raw[i] * right_tr.raw[j];
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

		void set_row(const vector<COLUMNS>& row, uint ind)
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

		void set_column(const vector<ROWS>& column, uint ind)
		{
			if (ind >= COLUMNS)
			{
				throw std::out_of_range("Index is out of range.");
			}

			for (uint i = 0; i < ROWS; i++)
			{
				raw[i].raw[ind] = column.raw[i];
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