#include "globals.h"
#include "math.h"
#include <stdlib.h>
#include <iostream>
#include <random>
#include "matrix.h"
#include "globals.h"
#include <cstdint>

class myMatrix
{
private:
    int64_t* matrix;
    int64_t rows;
    int64_t columns;

public:
    myMatrix(int64_t _rows, int64_t _columns);
    myMatrix(int64_t _rows_and_columns);
    ~myMatrix();

    void fill_matrix(int _seed);
    int64_t get(int64_t i, int64_t j);
    void set(int64_t i, int64_t j, int64_t value);
    int64_t getRows();
    int64_t getColumns();
    void print();
    void make_it_identityMatrix();
};

myMatrix::myMatrix(int64_t _rows, int64_t _columns)
{
    rows = _rows;
    columns = _columns;
    matrix = new int64_t[rows * columns];
}

myMatrix::myMatrix(int64_t _rows_and_columns)
{
    rows = _rows_and_columns;
    columns = _rows_and_columns;
    matrix = new int64_t[rows * columns];
}

myMatrix::~myMatrix()
{
    delete[] matrix;
}

void myMatrix::fill_matrix(int _seed)
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

int64_t myMatrix::get(int64_t i, int64_t j)
{
    return matrix[i * columns + j];
}

void myMatrix::set(int64_t i, int64_t j, int64_t value)
{
    matrix[i * columns + j] = value;
}

int64_t myMatrix::getRows()
{
    return rows;
}

int64_t myMatrix::getColumns()
{
    return columns;
}

void myMatrix::print()
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

void myMatrix::make_it_identityMatrix()
{
    for (int64_t i = 0; i < rows; i++)
    {
        for (int64_t j = 0; j < columns; j++)
        {
            matrix[i * columns + j] = (i == j) ? 1 : 0;
        }
    }
}