#include "globals.h"
#include "math.h"
#include <stdlib.h>
#include <iostream>
#include <random>

using namespace std;

namespace myMatrixNamespace 
{
	class myMatrix
	{
		long** matrix;
		long rows;
		long columns;

	public:
		myMatrix(long _rows, long _columns)
		{
			rows = _rows;
			columns = _columns;
			matrix = new long* [rows];
			for (long i = 0; i < rows; i++)
			{
				matrix[i] = new long[columns];
			}
		}
		myMatrix(long _rows_and_columns)
		{
			rows = _rows_and_columns;
			columns = _rows_and_columns;
			matrix = new long* [rows];
			for (long i = 0; i < rows; i++)
			{
				matrix[i] = new long[columns];
			}
		}
		~myMatrix()
		{
			for (long i = 0; i < rows; i++)
			{
				delete[] matrix[i];
			}
			delete[] matrix;
		}

		void fill_matrix(int _seed = globals::seed)
		{
			//srand(_seed);

			default_random_engine el(_seed);

			uniform_int_distribution<long> distribution(LONG_MIN, LONG_MAX);

			for (long i = 0; i < rows; i++)
			{
				for (long j = 0; j < columns; j++)
				{
					matrix[i][j] = abs(distribution(el) % globals::primeNumber);
				}
			}
		}

		long get(long i, long j)
		{
			return matrix[i][j];
		}
		void set(long i, long j, long value)
		{
			matrix[i][j] = value;
		}
		long getRows()
		{
			return rows;
		}
		long getColumns()
		{
			return columns;
		}
		void print()
		{
			for (int i = 0; i < rows; i++)
			{
				for (int j = 0; j < columns; j++)
				{
					cout << matrix[i][j] << " ";
				}
				cout << endl;
			}
		}

		void make_it_identityMatrix()
		{
			for (long i = 0; i < rows; i++)
			{
				matrix[i] = new long[rows];
				memset(matrix[i], 0, rows * sizeof(long)); // initialize identity matrix to all zeros
				matrix[i][i] = 1;
			}
		}
	};
}