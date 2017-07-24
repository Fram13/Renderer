#pragma once

#include "vector.h"

namespace graphics
{
	template <int ROWS, int COLUMNS>
	class matrix
	{
	public:
		static_assert(ROWS > 1, "Matrix rows number must be greater than one (at matrix<ROWS, COLUMNS>)");
		static_assert(COLUMNS > 1, "Matrix columns number must be greater than one (at matrix<ROWS, COLUMNS>)");

		matrix()
		{

		}

		matrix(std::initializer_list<std::initializer_list<float>> init_list)
		{
			auto rows_iter = init_list.begin();
			int i = 0;

			while (i < ROWS && rows_iter != init_list.end())
			{
				auto columns_iter = rows_iter->begin();
				int j = 0;

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

		matrix(std::initializer_list<vector<ROWS>> init_list)
		{
			auto iter = init_list.begin();
			int j = 0;

			while (j < COLUMNS && iter != init_list.end())
			{
				vector<ROWS> vec = *iter;
				set_column(vec, j);
				iter++;
				j++;
			}
		}

		vector<COLUMNS> get_row(int ind)
		{
			if (ind < 0 || ind >= ROWS)
			{
				throw std::out_of_range("Index is out of range.");
			}

			return raw[ind];
		}

		void set_row(vector<COLUMNS>& row, int ind)
		{
			if (ind < 0 || ind >= ROWS)
			{
				throw std::out_of_range("Index is out of range.");
			}

			raw[ind] = row;
		}

		vector<ROWS> get_column(int ind)
		{
			if (ind < 0 || ind >= COLUMNS)
			{
				throw std::out_of_range("Index is out of range.");
			}

			vector<ROWS> res;

			for (int i = 0; i < ROWS; i++)
			{
				res[i] = raw[i][ind];
			}

			return res;
		}

		void set_column(vector<ROWS>& column, int ind)
		{
			if (ind < 0 || ind >= COLUMNS)
			{
				throw std::out_of_range("Index is out of range.");
			}

			for (int i = 0; i < ROWS; i++)
			{
				raw[i][ind] = column[i];
			}
		}

		vector<ROWS> multiply(vector<COLUMNS>& other)
		{
			vector<ROWS> res;

			for (int i = 0; i < ROWS; i++)
			{
				res[i] = raw[i] * other;
			}

			return res;
		}

		template<int OTHER_COLUMNS>
		matrix<ROWS, OTHER_COLUMNS> multiply(matrix<COLUMNS, OTHER_COLUMNS>& other)
		{
			matrix<ROWS, OTHER_COLUMNS> res;
			matrix<OTHER_COLUMNS, COLUMNS> other_tr = other.transponse();

			for (int i = 0; i < ROWS; i++)
			{
				for (int j = 0; j < OTHER_COLUMNS; j++)
				{
					res[i][j] = raw[i] * other_tr[j];
				}
			}

			return res;
		}

		matrix<COLUMNS, ROWS> transponse()
		{
			matrix<COLUMNS, ROWS> res;

			for (int i = 0; i < ROWS; i++)
			{
				for (int j = 0; j < COLUMNS; j++)
				{
					res[j][i] = raw[i][j];
				}
			}

			return res;
		}

		matrix<ROWS, COLUMNS> inverse()
		{
			static_assert(ROWS == COLUMNS, "Inverse matrix must be square (at matrix<ROWS, COLUMNS>.inverse())");

			matrix<ROWS, COLUMNS> matr = *this;
			matrix<ROWS, COLUMNS> res = identity();

			for (int i = 0; i < ROWS; i++)
			{
				if (abs(matr[i][i]) < E)
				{
					bool inverse_exists = false;

					for (int j = i + 1; j < ROWS; j++)
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

				float m = 1.0f / matr[i][i];
				matr[i] = matr[i] * m;
				res[i] = res[i] * m;

				for (int j = 0; j < COLUMNS; j++)
				{
					if (j != i)
					{
						m = matr[j][i];
						matr[j] = matr[j] - matr[i] * m;
						res[j] = res[j] - res[i] * m;
					}
				}
			}

			return res;
		}

		vector<ROWS> operator*(vector<COLUMNS>& other)
		{
			return multiply(other);
		}

		template<int OTHER_COLUMNS>
		matrix<ROWS, OTHER_COLUMNS> operator*(matrix<COLUMNS, OTHER_COLUMNS>& other)
		{
			return multiply(other);
		}

		vector<COLUMNS>& operator[](int ind)
		{
			if (ind < 0 || ind >= ROWS)
			{
				throw std::out_of_range("Index is out of range.");
			}

			return raw[ind];
		}

		matrix<ROWS, COLUMNS>& operator=(matrix<ROWS, COLUMNS>& other)
		{
			for (int i = 0; i < ROWS; i++)
			{
				raw[i] = other[i];
			}

			return *this;
		}

		static matrix<ROWS, COLUMNS> identity();

	private:
		vector<COLUMNS> raw[ROWS];
		static const float E;
	};

	template <int ROWS, int COLUMNS>
	const float matrix<ROWS, COLUMNS>::E = 1e-6f;

	template <int ROWS, int COLUMNS>
	matrix<ROWS, COLUMNS> matrix<ROWS, COLUMNS>::identity()
	{
		static_assert(ROWS == COLUMNS, "Identity matrix must be square (at matrix<ROWS, COLUMNS>::identity())");

		matrix<ROWS, COLUMNS> res;

		for (int i = 0; i < ROWS; i++)
		{
			res[i][i] = 1.0f;
		}

		return res;
	}
}