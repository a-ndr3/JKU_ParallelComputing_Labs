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
	auto workrow = A.getRow(targetRow);

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
	vector<minimatrix> mini;
	vector<minimatrix> miniI;

	for (int i = 0; i < threads; i++)
	{
		auto mmatr = new minimatrix(threads);
		mmatr->parse(A, i);
		mini.push_back(*mmatr); // не забыть что появляются пустые вектора когда тредов больше чем размер матрицы
	}

	for (int i = 0; i < threads; i++)
	{
		auto mmatr = new minimatrix(threads);
		mmatr->parse(I, i);
		miniI.push_back(*mmatr);
	}


	for (int64_t i = 0; i < arows; i++)
	{
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

		//#pragma omp parallel
		{
			for (int t = 0; t < threads; t++)
			{
				minimatrix& mini_A = mini[t];
				minimatrix& mini_I = miniI[t];
				auto rowInd = mini_A.getRowIndexes(t);

				for (int64_t j = 0; j < rowInd.size(); j++)
				{
					if (rowInd[j] != i)
					{
						int64_t multiplier = mini_A.get(j, i);

						subtractRowThreads(mini_A, j, A.getWholeRow(i), multiplier);
						subtractRowThreads(mini_I, j, A.getWholeRow(i), multiplier);
					}
				}
			}
			
			#pragma omp barrier
			#pragma omp single
			for (int i = 0; i < mini.size(); i++)
			{
				auto ind = mini[i].getRowIndexes(i);

				for (auto& c : ind)
				{
					auto indd = find_if(ind.begin(), ind.end(), [c](int64_t value) {
						return value == c;
						});
					A.setRow(c, mini[i].getRow(std::distance(ind.begin(),indd)));
				}
			}
			#pragma omp single
			for (int i = 0; i < miniI.size(); i++)
			{
				auto ind = miniI[i].getRowIndexes(i);

				for (auto& c : ind)
				{
					auto indd = find_if(ind.begin(), ind.end(), [c](int64_t value) {
						return value == c;
						});
					I.setRow(c, miniI[i].getRow(std::distance(ind.begin(), indd)));
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

	Logger::log("Parallel Advanced method started for Matrix = " + std::to_string(n));

	bench.startTimer();
	diagonalize(A, I);
	bench.stopTimer();

	Logger::logFull("Finished with elapsed time: ", bench.getElapsedTime());

	return I;
}


/*
	for (int i = 0; i < threads; i++)
	{
		minimatrix& mini_A = mini[i];
		minimatrix& mini_I = miniI[i];
		auto rowInd = mini_A.getRowIndexes(i);





		#pragma omp barrier
		#pragma omp single
		for (int64_t i = 0; i < arows; i++)
		{
			int mini_matrix_idx = i % threads;
			A.setRow(i, mini[mini_matrix_idx].getRow(i));
		}
		#pragma omp single
		for (int64_t i = 0; i < arows; i++)
		{
			int mini_matrix_idx = i % threads;
			I.setRow(i, miniI[mini_matrix_idx].getRow(i));
		}
	}
*/

/*
for (int64_t j = 0; j < mini_A.getParsedMatrix().size(); j++)
		{
			int64_t global_row = i + j * threads;
			int64_t maxRowIndex = global_row;

			for (int64_t k = global_row + threads; k < arows; k += threads)
			{
				if (A.get(k, global_row) > A.get(maxRowIndex, global_row))
				{
					maxRowIndex = k;
				}
			}

			if (maxRowIndex != global_row)
			{
				mini_A.swapRows(global_row, maxRowIndex);
				mini_I.swapRows(global_row, maxRowIndex);
			}

			int64_t pivot = mini_A.get(global_row, global_row);

			divideRowThreads(mini_A, global_row, pivot);
			divideRowThreads(mini_I, global_row, pivot);


			auto rowStep = A.getWholeRow(global_row);
			for (int64_t k = 0; k < arows; k++)
			{
				//int64_t local_row = k % x;

				if (k != global_row && std::find(rowInd.begin(), rowInd.end(), k) != rowInd.end()) //&& mini_A.existanceOfRow(local_row))
				{
					auto specific_index_of_row_that_stored_in_a_thread = std::distance(rowInd.begin(), std::find_if(rowInd.begin(), rowInd.end(), [k](int64_t value) {
						return value == k;
						}));

					int64_t multiplier = A.get(j, global_row); //mini[k % x].get(local_row, global_row);

					subtractRowThreads(mini_A, specific_index_of_row_that_stored_in_a_thread, rowStep, multiplier);
					subtractRowThreads(mini_I, specific_index_of_row_that_stored_in_a_thread, rowStep, multiplier);
				}
			}
		}
*/