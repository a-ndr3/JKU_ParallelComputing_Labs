#ifndef PARALLEL_GAUSS_METHODS_BASIC_H
#define PARALLEL_GAUSS_METHODS_BASIC_H

#include "gauss_methods.cpp"

class ParallelGauss : public Gauss
{
	 void divideRow(myMatrix& A, int64_t row, int64_t divisor) override;

	 void subtractRow(myMatrix& A, int64_t targetRow, int64_t sourceRow, int64_t multiplier) override;

	 void diagonalize(myMatrix& A, myMatrix& I) override;

public:
	ParallelGauss() : Gauss() {}
	~ParallelGauss() {}

	 myMatrix Solve(myMatrix& A) override;
};

#endif