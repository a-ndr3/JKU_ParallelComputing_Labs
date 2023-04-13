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

void ParallelGaussAdvanced::divideRowThreads(minimatrix& A, int64_t row, int64_t divisor)
{
	auto rows = A.getParsedMatrix();
	for (int64_t i = 0; i < rows.size(); i++)
	{
		for (int j = 0; j < rows[i].size(); j++)
		{
			rows[row][j] = mDiv(rows[row][j], divisor);
		}
	}
}

void ParallelGaussAdvanced::subtractRowThreads(minimatrix& A, int64_t targetRow, vector<int64_t> sourceRow, int64_t multiplier)
{
	auto& workrow = A.getRow(targetRow);

	for (int64_t i = 0; i < workrow.size(); i++)
	{
		workrow[i] = mSub(workrow[i], mMul(multiplier, sourceRow[i]));
	}
}

void ParallelGaussAdvanced::subtractRow(myMatrix& A, int64_t targetRow, int64_t sourceRow, int64_t multiplier)
{
	for (int64_t i = 0; i < A.getColumns(); i++)
	{
		A.set(targetRow, i, mSub(A.get(targetRow, i), mMul(multiplier, A.get(sourceRow, i))));
	}
}


void ParallelGaussAdvanced::diagonalize(myMatrix& A, myMatrix& I)
{
	auto threads = omp_get_max_threads();
	auto arows = A.getRows();

	if (threads > arows)
	{
		threads = arows;
	}

	vector<minimatrix> mini;
	vector<minimatrix> miniI;

	for (int64_t i = 0; i < arows; i++)
	{
		mini.clear();
		miniI.clear();

		int64_t maxRowIndex = i;
		for (int64_t k = i + 1; k < arows; k++)
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

		for (int i = 0; i < threads; i++)
		{
			auto mmatr = std::make_unique<minimatrix>(threads);
			mmatr->parse(A, i);
			mini.push_back(*mmatr); // не забыть что появляются пустые вектора когда тредов больше чем размер матрицы
		}

		for (int i = 0; i < threads; i++)
		{
			auto mmatr = std::make_unique<minimatrix>(threads);
			mmatr->parse(I, i);
			miniI.push_back(*mmatr);
		}

			#pragma omp parallel for num_threads(threads)
			for (int t = 0; t < threads; t++)
			{
				minimatrix& mini_A = mini[t];
				minimatrix& mini_I = miniI[t];
				auto rowInd = mini_A.getRowIndexes(t);

				for (int64_t j = 0; j < rowInd.size(); j++)
				{
					if (rowInd[j] != i)
					{
						int64_t multiplier = A.get(rowInd[j], i);

						subtractRowThreads(mini_A, j, A.getWholeRow(i), multiplier);
						subtractRowThreads(mini_I, j, I.getWholeRow(i), multiplier);
					}
				}
			}
			
			#pragma omp parallel for
			for (int i = 0; i < mini.size(); i++)
			{
				auto ind = mini[i].getRowIndexes(i);

				for (int k = 0; k < ind.size(); k++)
				{
					auto c = ind[k];

					A.setRow(c, mini[i].getRow(k));
				}				
			}
			
			#pragma omp parallel for
			for (int i = 0; i < miniI.size(); i++)
			{
				auto ind = miniI[i].getRowIndexes(i);

				for (int k = 0; k < ind.size(); k++)
				{
					auto c = ind[k];

					I.setRow(c, miniI[i].getRow(k));
				}
			}
	//#pragma omp barrier
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