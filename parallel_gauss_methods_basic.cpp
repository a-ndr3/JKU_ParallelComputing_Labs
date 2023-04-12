#include "parallel_gauss_methods_basic.h"
#include <cstdint>

void ParallelGauss::divideRow(myMatrix& A, int64_t row, int64_t divisor)
{
	for (int64_t i = 0; i < A.getColumns(); i++)
	{
		A.set(row, i, mDiv(A.get(row, i), divisor)); //modular division of each el by divisor
	}
}

void ParallelGauss::subtractRow(myMatrix& A, int64_t targetRow, int64_t sourceRow, int64_t multiplier)
{
	for (int64_t i = 0; i < A.getColumns(); i++)
	{
		A.set(targetRow, i, mSub(A.get(targetRow, i), mMul(multiplier, A.get(sourceRow, i)))); // substr from target string str multipied 
	}
}

void ParallelGauss::diagonalize(myMatrix& A, myMatrix& I)
{
	int64_t n = A.getRows();

	for (int64_t i = 0; i < n; i++)
	{
		//partially move
		int64_t maxRowIndex = i;
		for (int64_t k = i + 1; k < n; k++) {
			if (A.get(k, i) > A.get(maxRowIndex, i)) {
				maxRowIndex = k;
			}
		}

		if (maxRowIndex != i) {
			A.swapRows(i, maxRowIndex);
			I.swapRows(i, maxRowIndex);
		}

		int64_t pivot = A.get(i, i); // diagonal element

		divideRow(A, i, pivot); // divide pivot row by pivot el to get 1

		divideRow(I, i, pivot); // divide following row of identity matrix by pivot el

	//	  #pragma omp parallel for schedule(runtime)
		//#pragma omp parallel for schedule(dynamic)
		for (int64_t j = 0; j < n; j++)
		{
			if (j != i)
			{
				int64_t multiplier = A.get(j, i); // get multiplier to eliminate element below pivot

				subtractRow(A, j, i, multiplier); //substract multiplied pivot row from current to make el below pivot = 0
				subtractRow(I, j, i, multiplier); //substract multiplied pivot row from following in identity
				I.print();
			}
		}
	}
}


myMatrix ParallelGauss::Solve(myMatrix& A)
{
	myBenchmarks bench;

	int64_t n = A.getRows();

	myMatrix I(n);

	I.make_it_identityMatrix();

	Logger::log("Parallel Basic Gauss method started for Matrix = " + std::to_string(n));

	bench.startTimer();
	diagonalize(A, I);
	bench.stopTimer();

	Logger::logFull("Finished with elapsed time: ", bench.getElapsedTime());

	return I;
}

