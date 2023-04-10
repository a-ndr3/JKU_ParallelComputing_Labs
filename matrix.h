#ifndef MATRIX_H
#define MATRIX_H

#include "globals.h"
#include <cstdint>
#include <omp.h>
#include <vector>

struct Vec{
    std::vector<int64_t*> startOfArrays;
    int64_t size;
    int threadNumber;
};

class myMatrix
{
    int64_t** matrix;
    int64_t rows;
    int64_t columns;

    std::vector<Vec> vectors;

public:
    myMatrix(int64_t _rows, int64_t _columns);
    myMatrix(int64_t _rows_and_columns);
    ~myMatrix();

    void fill_matrix(int _seed = globals::seed);

    int64_t get(int64_t i, int64_t j);
    void set(int64_t i, int64_t j, int64_t value);
    int64_t getRows();
    int64_t getColumns();
    void swapRows(int64_t row1, int64_t row2);
    void print();

    void make_it_identityMatrix();


    //Advanced planned?
    void splitMatrix(int64_t threadsAmount);
    std::vector<Vec> getVec();
    int64_t getVecSize();
};

#endif