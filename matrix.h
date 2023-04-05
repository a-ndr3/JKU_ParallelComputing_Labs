#ifndef MATRIX_H
#define MATRIX_H

#include "globals.h"

class myMatrix
{
	long** matrix;
	long rows;
	long columns;

	public:
		myMatrix(long _rows, long _columns);
		myMatrix(long _rows_and_columns);
		~myMatrix();

		void fill_matrix(int _seed = globals::seed);

		long get(long i, long j);
		void set(long i, long j, long value);
		long getRows();
		long getColumns();
		void print();

		void make_it_identityMatrix();
};

#endif