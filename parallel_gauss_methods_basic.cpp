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

	if (globals::threadMode == globals::ThreadMode::LOCAL)
	{
		omp_set_num_threads(globals::threadsAmount);
	}
	else
	{
		omp_set_num_threads(omp_get_max_threads());
	}

	for (int64_t i = 0; i < n; i++)
	{
		int64_t pivot = A.get(i, i); // diagonal element
		int64_t inv_pivot = mInv(pivot); // get modular inverse of pivot

		if (inv_pivot == -1) //check if not regular based on mInv return value cuz modular inverse of 0 is 'null'
		{
			std::cout << "Matrix is not regular." << std::endl;
			return;
		}

		divideRow(A, i, pivot); // divide pivot row by pivot el to get 1
		divideRow(I, i, pivot); // divide following row of identity matrix by pivot el

		#pragma omp parallel for schedule(dynamic)
		for (int64_t j = i + 1; j < n; j++)
		{
			int64_t multiplier = A.get(j, i);

			subtractRow(A, j, i, multiplier);
			subtractRow(I, j, i, multiplier);
		}
	}
}

myMatrix ParallelGauss::Solve(myMatrix& A)
{
	myBenchmarks bench;

	int64_t n = A.getRows();

	myMatrix I(n);

	I.make_it_identityMatrix();

	Logger::log("Parallel Basic Gauss method started");

	bench.startTimer();
	diagonalize(A, I);
	bench.stopTimer();

	Logger::logFull("Finished with elapsed time: ", bench.getElapsedTime());

	return I;
}

