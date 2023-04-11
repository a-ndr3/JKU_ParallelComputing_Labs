#include "parallel_gauss_methods_advanced.h"
#include "minimatrix.h"
#include <cstdint>
#include <omp.h>
#include <algorithm>

void ParallelGaussAdvanced::divideRow(myMatrix& A, int64_t row, int64_t divisor)
{
	for (int64_t i = 0; i < A.getColumns(); i++)
	{
		A.set(row, i, mDiv(A.get(row, i), divisor));
	}
}

void ParallelGaussAdvanced::subtractRowThreads(std::vector<std::vector<int64_t>>& A, int64_t targetRow, int64_t sourceRow, int64_t multiplier)
{
	for (int64_t i = 0; i < A[0].size(); i++)
	{
		A[targetRow][i] = mSub(A[targetRow][i], mMul(multiplier, A[sourceRow][i]));
	}
}

void ParallelGaussAdvanced::subtractRow(myMatrix& A, int64_t targetRow, int64_t sourceRow, int64_t multiplier)
{
	for (int64_t i = 0; i < A.getColumns(); i++)
	{
		A.set(targetRow, i, mSub(A.get(targetRow, i), mMul(multiplier, A.get(sourceRow, i))));
	}
}

static bool findPivotRow(myMatrix& matrix, int i) {
	if (matrix.get(i, i) != 0) {
		return true;
	}

	for (int pivot_search_row = i + 1; pivot_search_row < matrix.getRows(); pivot_search_row++) {
		if (matrix.get(pivot_search_row, i) != 0) {
			matrix.swapRows(pivot_search_row, i);
			return true;
		}
	}
	return false;
}

void ParallelGaussAdvanced::diagonalize(myMatrix& A, myMatrix& I)
{
	auto x = omp_get_max_threads();
	//auto miniA = miniParser(A.getMatrix(), A.getRows(), A.getColumns(), x);
	//auto miniI = miniParser(I.getMatrix(), A.getRows(), A.getColumns(), x);
	//auto parsedA = miniA.getParsedMatrix();
	//auto parsedI = miniParser(I.getMatrix(), I.getRows(), I.getColumns(), omp_get_max_threads()).getParsedMatrix();

	/*int i = 0;
	for (const auto& row : parsedA)
	{
		for (const auto& col : row)
		{
			std::cout << col << " ";
		}
		std::cout << "thread: " << miniA.getThreadForSpecificRow(i) << std::endl;
		i++;
	}*/

	int64_t n = A.getRows();
	int64_t num_threads = omp_get_max_threads();

	for (int64_t i = 0; i < n; i++)
	{
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

		int64_t pivot = A.get(i, i);

		divideRow(A, i, pivot);
		divideRow(I, i, pivot);

		auto miniA = miniParser(A.getMatrix(), A.getRows(), A.getColumns(), x);
		auto miniI = miniParser(I.getMatrix(), A.getRows(), A.getColumns(), x);
		auto parsedA = miniA.getParsedMatrix();
		auto parsedI = miniParser(I.getMatrix(), I.getRows(), I.getColumns(), omp_get_max_threads()).getParsedMatrix();

		#pragma omp parallel num_threads(num_threads)
		{
			int64_t tid = omp_get_thread_num();
			for (int64_t j = 0; j < miniA.getNumOfThreadUsed(); j++)
			{
				if (miniA.getThreadForSpecificRow(j) == tid && j != i)
				{
					int64_t multiplier = A.get(j, i);

					subtractRowThreads(parsedA, j, i, multiplier);
					subtractRowThreads(parsedI, j, i, multiplier);
				}
			}

			#pragma omp barrier
			if (tid == 0) 
			{
				for (int64_t j = 0; j < miniA.getNumOfThreadUsed(); j++) 
				{
					for (int64_t k = 0; k < A.getColumns(); k++) 
					{
						A.set(j, k, parsedA[j][k]);
					}
				}
			}
			if (tid == 1) 
			{
				for (int64_t j = 0; j < miniI.getNumOfThreadUsed(); j++)
				{
					for (int64_t k = 0; k < I.getColumns(); k++)
					{
						I.set(j, k, parsedI[j][k]);
					}
				}
			}
		#pragma omp barrier
		}
	}
}


myMatrix ParallelGaussAdvanced::Solve(myMatrix& A)
{
	myBenchmarks bench;

	int64_t n = A.getRows();

	myMatrix I(n);

	I.make_it_identityMatrix();

	Logger::log("Parallel Advanced method started for Matrix = " + std::to_string(n));

	bench.startTimer();
	diagonalize(A, I);
	bench.stopTimer();

	Logger::logFull("Finished with elapsed time: ", bench.getElapsedTime());

	return I;
}


