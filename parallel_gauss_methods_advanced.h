#ifndef PARALLEL_GAUSS_METHODS_ADVANCED_H
#define PARALLEL_GAUSS_METHODS_ADVANCED_H

#include "gauss_methods.cpp"
#include "minimatrix.h"

class ParallelGaussAdvanced : public Gauss
{
	 void divideRow(myMatrix& A, int64_t row, int64_t divisor) override;

	 void subtractRow(myMatrix& A, int64_t targetRow, int64_t sourceRow, int64_t multiplier) override;

	 void diagonalize(myMatrix& A, myMatrix& I) override;

	 void subtractRowThreads(minimatrix& A, int64_t targetRow, vector<int64_t> sourceRow, int64_t multiplier);
	 void divideRowThreads(minimatrix& A, int64_t row, int64_t divisor);
public:
	ParallelGaussAdvanced() : Gauss() {};
	~ParallelGaussAdvanced() {};

	 myMatrix Solve(myMatrix& A) override;	 
};

#endif