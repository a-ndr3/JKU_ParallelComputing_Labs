#include "parallel_gauss_methods_advanced.h"
#include <cstdint>

void ParallelGaussAdvanced::divideRow(myMatrix& A, int64_t row, int64_t divisor)
{
	for (int64_t i = 0; i < A.getColumns(); i++)
	{
		A.set(row, i, mDiv(A.get(row, i), divisor)); //modular division of each el by divisor
	}
}

void ParallelGaussAdvanced::subtractRow(myMatrix& A, int64_t targetRow, int64_t sourceRow, int64_t multiplier)
{
	for (int64_t i = 0; i < A.getColumns(); i++)
	{
		A.set(targetRow, i, mSub(A.get(targetRow, i), mMul(multiplier, A.get(sourceRow, i)))); // substr from target string str multipied 
	}
}

void ParallelGaussAdvanced::diagonalize(myMatrix& A, myMatrix& I)
{
	int64_t n = A.getRows();

	for (int64_t i = 0; i < n; i++)
	{
		int64_t pivot = A.get(i, i); // diagonal element
		int64_t inv_pivot = mInv(pivot); // get modular inverse of pivot

		//check if not regular
		if (inv_pivot == -1)
		{
			std::cout << "Matrix is not regular." << std::endl;
			return;
		}

		divideRow(A, i, pivot); // divide pivot row by pivot el to get 1
		divideRow(I, i, pivot); // divide following row of identity matrix by pivot el

		#pragma omp parallel
		{
			int64_t tId = omp_get_thread_num();
			int64_t num_threads = omp_get_num_threads();

			for (int64_t j = tId; j < n; j += num_threads)
			{
				if (j != i)
				{
					int64_t multiplier;
					#pragma omp critical
					{
						multiplier = A.get(j, i); // get multiplier to eliminate element below pivot
					}

					subtractRow(A, j, i, multiplier); //substract multiplied pivot row from current to make el below pivot = 0
					subtractRow(I, j, i, multiplier); //substract multiplied pivot row from following in identity
				}
			}
		}
	}
}

myMatrix ParallelGaussAdvanced::Solve(myMatrix& A)
{
	myBenchmarks bench;

	int64_t n = A.getRows();

	myMatrix I(n);

	I.make_it_identityMatrix();

	Logger::log("Parallel Advanced method started");

	bench.startTimer();
	diagonalize(A, I);
	bench.stopTimer();

	Logger::logFull("Finished with elapsed time: ", bench.getElapsedTime());

	return I;
}
