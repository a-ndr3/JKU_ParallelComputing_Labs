#ifndef MATRIX_SOLID_H
#define MATRIX_SOLID_H

#include "globals.h"
#include <cstdint>

class matrix_Solid
{
	int64_t *matrix;
	int64_t rows;
	int64_t columns;

public:
	matrix_Solid(int64_t _rows, int64_t _columns);
	matrix_Solid(int64_t _rows_and_columns);
	~matrix_Solid();

	void fill_matrix(int _seed = globals::seed);

	int64_t get(int64_t i, int64_t j);
	void set(int64_t i, int64_t j, int64_t value);
	int64_t getRows();
	int64_t getColumns();
	void print();

	void make_it_identityMatrix();
};

#endif