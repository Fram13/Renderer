#pragma once

#include "vector.h"

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
				rows[i][j] = *columns_iter;
				columns_iter++;
				j++;
			}

			rows_iter++;
			i++;
		}
	}

	matrix(std::initializer_list<vector<ROWS>> init_list)
	{

	}

	vector<ROWS> multiply(vector<COLUMNS>& other)
	{
		vector<ROWS> res;

		for (int i = 0; i < ROWS; i++)
		{
			res[i] = rows[i] * other;
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
				res[i][j] = rows[i] * other_tr[j];
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
				res[j][i] = rows[i][j];
			}
		}

		return res;
	}

	matrix<ROWS, COLUMNS> inverse()
	{
		static_assert(ROWS == COLUMNS, "Inverse matrix must be square (at matrix<ROWS, COLUMNS>.inverse())");

		
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

		return rows[ind];
	}

	matrix<ROWS, COLUMNS>& operator=(matrix<ROWS, COLUMNS>& other)
	{
		for (int i = 0; i < ROWS; i++)
		{
			rows[i] = other[i];
		}

		return *this;
	}

	static matrix<ROWS, COLUMNS> identity();

private:
	vector<COLUMNS> rows[ROWS];
};

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