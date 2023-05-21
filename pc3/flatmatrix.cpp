//
// Created by abloh on 5/21/2023.
//

#include <random>
#include <iostream>
#include "flatmatrix.h"

flatmatrix::flatmatrix(int64_t _rows, int64_t _columns)
{
    rows = _rows;
    columns = _columns;
    data = new int64_t[rows * columns];
}

void flatmatrix::fillflatmatrix(int _seed)
{
    std::default_random_engine el(_seed);

    std::uniform_int_distribution<int64_t> distribution(0, globals::primeNumber - 1);

    for (int64_t i = 0; i < rows; i++)
    {
        for (int64_t j = 0; j < columns; j++)
        {
            data[i * columns + j] = distribution(el) % globals::primeNumber;
        }
    }

}

void flatmatrix::make_flatmatrix_identityMatrix()
{
       for (int64_t i = 0; i < rows; i++)
        {
            for (int64_t j = 0; j < columns; j++)
            {
                if (i == j)
                {
                    data[i * columns + j] = 1;
                }
                else
                {
                    data[i * columns + j] = 0;
                }
            }
        }
}

void flatmatrix::print()
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            std::cout << data[i * columns + j] << " ";
        }
        std::cout << std::endl;
    }
}

int64_t * flatmatrix::getData()
{
    return data;
}

flatmatrix::~flatmatrix() {
    delete[] data;
}
