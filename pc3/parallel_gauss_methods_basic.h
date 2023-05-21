#ifndef PARALLEL_GAUSS_METHODS_BASIC_H
#define PARALLEL_GAUSS_METHODS_BASIC_H

#include "flatmatrix.h"

#include <mpi.h>

class ParallelGauss
{
	 void divideRow(int64_t* A, int64_t row, int64_t divisor, int64_t M);

	 void subtractRow(int64_t* A, int64_t targetRow, int64_t sourceRow, int64_t multiplier, int64_t M);



public:
    flatmatrix solveParallel(flatmatrix& A, int N, int M,int size, int rank);
    void diagonalize(flatmatrix& aMatr, flatmatrix& iMatr, int M, int N, int size, int rank);

    ParallelGauss();

};

#endif