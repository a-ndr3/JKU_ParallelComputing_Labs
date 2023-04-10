#include "parallel_gauss_methods_advanced.h"
#include <cstdint>

void ParallelGaussAdvanced::divideRow(myMatrix& A, int64_t row, int64_t divisor)
{
	for (int64_t i = 0; i < A.getColumns(); i++)
	{
		A.set(row, i, mDiv(A.get(row, i), divisor));
	}
}

void ParallelGaussAdvanced::subtractRowThreads(int64_t* row, int64_t* pivotRow, int64_t multiplier, int64_t columns)
{
	for (int64_t k = 0; k < columns; k++)
	{
		row[k] = mSub(row[k], mMul(multiplier, pivotRow[k]));
	}
}

void ParallelGaussAdvanced::subtractRow(myMatrix& A, int64_t targetRow, int64_t sourceRow, int64_t multiplier)
{

}

void ParallelGaussAdvanced::diagonalize(myMatrix& A, myMatrix& I)
{
	int64_t n = A.getRows();

	int threadsAmount = omp_get_max_threads();

	A.splitMatrix(threadsAmount); // Split the matrix A into vectors per thread
	I.splitMatrix(threadsAmount); // Split the identity matrix I into vectors per thread

	std::vector<Vec> aVecs = A.getVec(); // Get the vectors for matrix A
	std::vector<Vec> iVecs = I.getVec(); // Get the vectors for identity matrix I

	for (int64_t i = 0; i < n; i++)
	{
		// Partial pivoting
		int64_t maxRowIndex = i;
		for (int64_t k = i + 1; k < n; k++)
		{
			if (A.get(k, i) > A.get(maxRowIndex, i))
			{
				maxRowIndex = k;
			}
		}

		if (maxRowIndex != i)
		{
			A.swapRows(i, maxRowIndex);
			I.swapRows(i, maxRowIndex);
		}

		int64_t pivot = A.get(i, i);

		divideRow(A, i, pivot);
		divideRow(I, i, pivot);

		//#pragma omp parallel
		//{
		for (size_t threadNum = 0; threadNum < 8; threadNum++)
		{
			if (threadNum == 3) return;

			//int64_t threadNum = omp_get_thread_num();
			Vec& aVec = aVecs[threadNum];
			Vec& iVec = iVecs[threadNum];

			for (int64_t j = 0; j < aVec.startOfArrays.size(); j++)
			{
				int64_t rowIdx = threadNum * aVec.startOfArrays.size() + j;
				
				if (rowIdx != i)
				{
					int64_t multiplier = A.get(rowIdx, i); // get multiplier to eliminate element below pivot

					int64_t* aRow = aVec.startOfArrays[j];
					int64_t* iRow = iVec.startOfArrays[j];

					int64_t* aPivotRow = aVecs[i % threadsAmount].startOfArrays[i / threadsAmount];
					int64_t* iPivotRow = iVecs[i % threadsAmount].startOfArrays[i / threadsAmount];

					int64_t columns = A.getColumns();

					subtractRowThreads(aRow, aPivotRow, multiplier, columns); // subtract multiplied pivot row from current to make el below pivot = 0
					subtractRowThreads(iRow, iPivotRow, multiplier, columns); // subtract multiplied pivot row from following in identity
				}
			}
		}
		//}
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


