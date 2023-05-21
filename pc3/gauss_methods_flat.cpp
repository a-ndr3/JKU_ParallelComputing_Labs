#ifndef gauss_methods_flat_cpp
#define gauss_methods_flat_cpp

#include "support_methods.h"
#include "flatmatrix.h"
#include <iostream>
#include "benchmarking.h"
#include "logger.cpp"

class GaussFlat
{
    virtual void divideRow(flatmatrix& A, int64_t row, int64_t divisor)
    {
        auto columns = A.getColumns();
        auto data = A.getData();

        for (int64_t i = 0; i < columns; i++)
        {
            data[row * columns + i] = mDiv(data[row * columns + i], divisor);
        }
    }

    virtual void subtractRow(flatmatrix& A, int64_t targetRow, int64_t sourceRow, int64_t multiplier)
    {
        auto columns = A.getColumns();
        auto data = A.getData();

        for (int64_t i = 0; i < columns; i++)
        {
            data[targetRow * columns + i] = mSub(data[targetRow * columns + i], mMul(multiplier, data[sourceRow * columns + i]));
        }
    }

    virtual void diagonalize(flatmatrix& A, flatmatrix& I)
    {
        int64_t n = A.getRows();

        for (int64_t i = 0; i < n; i++)
        {
            int64_t maxRowIndex = i;
            for (int64_t k = i + 1; k < n; k++) {
                if (A.getData()[k * A.getColumns() + i] > A.getData()[maxRowIndex * A.getColumns() + i]) {
                    maxRowIndex = k;
                }
            }

            if (maxRowIndex != i) {
                std::swap_ranges(&A.getData()[i * A.getColumns()], &A.getData()[(i+1) * A.getColumns()], &A.getData()[maxRowIndex * A.getColumns()]);
                std::swap_ranges(&I.getData()[i * I.getColumns()], &I.getData()[(i+1) * I.getColumns()], &I.getData()[maxRowIndex * I.getColumns()]);
            }

            int64_t pivot = A.getData()[i * A.getColumns() + i];

            divideRow(A, i, pivot);

            divideRow(I, i, pivot);

            for (int64_t j = 0; j < n; j++)
            {
                if (j != i)
                {
                    int64_t multiplier = A.getData()[j * A.getColumns() + i];

                    subtractRow(A, j, i, multiplier);
                    subtractRow(I, j, i, multiplier);
                }
            }
        }
    }

public:

    virtual flatmatrix Solve(flatmatrix& A)
    {
        myBenchmarks bench;

        int64_t n = A.getRows();
        int64_t m = A.getColumns();

        flatmatrix I(n,m);

        I.make_flatmatrix_identityMatrix();

        Logger::log("Gauss method started for Matrix: " + std::to_string(n) + "x" + std::to_string(m));

        bench.startTimer();
        diagonalize(A, I);
        double elapsed = bench.getTime();

        Logger::logFull("Finished with elapsed time: ", elapsed);

        return I;
    }
};
#endif



