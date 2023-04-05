#include "globals.h"
#include "math.h"
#include <stdlib.h>
#include <iostream>
#include <random>
#include "matrix.h"

long** matrix;
long rows;
long columns;

myMatrix::myMatrix(long _rows, long _columns)
{
	rows = _rows;
	columns = _columns;
	matrix = new long* [rows];
	for (long i = 0; i < rows; i++)
	{
		matrix[i] = new long[columns];
	}
}
myMatrix::myMatrix(long _rows_and_columns)
{
	rows = _rows_and_columns;
	columns = _rows_and_columns;
	matrix = new long* [rows];
	for (long i = 0; i < rows; i++)
	{
		matrix[i] = new long[columns];
	}
}
myMatrix::~myMatrix()
{
	for (long i = 0; i < rows; i++)
	{
		delete[] matrix[i];
	}
	delete[] matrix;
}

void myMatrix::fill_matrix(int _seed)
{
	//srand(_seed);

	std::default_random_engine el(_seed);

	std::uniform_int_distribution<long> distribution(LONG_MIN, LONG_MAX);

	for (long i = 0; i < rows; i++)
	{
		for (long j = 0; j < columns; j++)
		{
			matrix[i][j] = abs(distribution(el) % globals::primeNumber);
		}
	}
}

long myMatrix::get(long i, long j)
{
	return matrix[i][j];
}
void myMatrix::set(long i, long j, long value)
{
	matrix[i][j] = value;
}
long myMatrix::getRows()
{
	return rows;
}
long myMatrix::getColumns()
{
	return columns;
}
void myMatrix::print()
{
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			std::cout << matrix[i][j] << " ";
		}
		std::cout << std::endl;
	}
}

void myMatrix::make_it_identityMatrix()
{
	for (long i = 0; i < rows; i++)
	{
		matrix[i] = new long[rows];
		memset(matrix[i], 0, rows * sizeof(long)); // initialize identity matrix to all zeros
		matrix[i][i] = 1;
	}
}
