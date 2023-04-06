#include "globals.h"
#include "math.h"
#include <stdlib.h>
#include <iostream>
#include <random>
#include "matrix_solid.h"

int64_t* matrix;
int64_t rows;
int64_t columns;

matrix_Solid::matrix_Solid(int64_t _rows, int64_t _columns)
{
	rows = _rows;
	columns = _columns;
	matrix = new int64_t[rows * columns];
}
matrix_Solid::matrix_Solid(int64_t _rows_and_columns)
{
	rows = _rows_and_columns;
	columns = _rows_and_columns;
	matrix = new int64_t[rows * columns];
}
matrix_Solid::~matrix_Solid()
{
	delete[] matrix;
}

void matrix_Solid::fill_matrix(int _seed)
{
	std::default_random_engine el(_seed);
	std::uniform_int_distribution<int64_t> distribution(LONG_MIN, LONG_MAX);

	for (int64_t i = 0; i < rows; i++)
	{
		for (int64_t j = 0; j < columns; j++)
		{
			matrix[i * columns + j] = abs(distribution(el) % globals::primeNumber);
		}
	}
}

int64_t matrix_Solid::get(int64_t i, int64_t j)
{
	return matrix[i * columns + j];
}
void matrix_Solid::set(int64_t i, int64_t j, int64_t value)
{
	matrix[i * columns + j] = value;
}
int64_t matrix_Solid::getRows()
{
	return rows;
}
int64_t matrix_Solid::getColumns()
{
	return columns;
}
void matrix_Solid::print()
{
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			std::cout << matrix[i * columns + j] << " ";
		}
		std::cout << std::endl;
	}
}

void matrix_Solid::make_it_identityMatrix()
{
	for (int64_t i = 0; i < rows; i++)
	{
		for (int64_t j = 0; j < columns; j++)
		{
			matrix[i * columns + j] = (i == j) ? 1 : 0;
		}
	}
}