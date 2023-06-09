//
// Created by abloh on 5/21/2023.
//

#ifndef PC3_FLATMATRIX_H
#define PC3_FLATMATRIX_H


#include <cstdint>
#include "globals.h"

class flatmatrix {
    int64_t rows;
    int64_t columns;
    int64_t* data;

public:
    flatmatrix(int64_t _rows, int64_t _columns);
    flatmatrix(int64_t _rows_cols);
    ~flatmatrix();
    void fillflatmatrix(int _seed = globals::seed);
    void print();
    void make_flatmatrix_identityMatrix();
    int64_t * getData();

    int64_t getRows() const;
    int64_t getColumns() const;
};


#endif //PC3_FLATMATRIX_H
