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

void ParallelGaussAdvanced::subtractRowThreads(minimatrix& A, int64_t targetRow, int64_t sourceRow, int64_t multiplier)
{
	auto rows = A.getParsedMatrix();
	for (int64_t i = 0; i < rows.size(); i++)
	{
		for (int j = 0; j < rows[i].size(); j++)
		{
			rows[targetRow][j] = mSub(rows[targetRow][j], mMul(multiplier, rows[sourceRow][j]));
		}

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
	auto x = omp_get_max_threads();
	auto arows = A.getRows();
	vector<minimatrix> mini;
	vector<minimatrix> miniI;

	for (int i = 0; i < x; i++)
	{
		auto mmatr = new minimatrix(x);
		mmatr->parse(A, i);
		mini.push_back(*mmatr); // не забыть что появляются пустые вектора когда тредов больше чем размер матрицы
	}

	for (int i = 0; i < x; i++)
	{
		auto mmatr = new minimatrix(x);
		mmatr->parse(I, i);
		miniI.push_back(*mmatr);
	}

	//#pragma omp parallel //for num_threads(x)
	for (int i = 0; i < x; i++)
	{
		minimatrix& mini_A = mini[i];
		minimatrix& mini_I = miniI[i];

		for (int64_t j = 0; j < mini_A.getParsedMatrix().size(); j++)
		{
			int64_t global_row = i + j * x; // Calculate global row index
			int64_t maxRowIndex = global_row;
			for (int64_t k = global_row + x; k < arows; k += x)
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

			for (int64_t j = 0; j < arows; j++)
			{
				if (j != global_row)
				{
					int64_t multiplier = A.get(j, global_row);

					subtractRowThreads(mini_A, j, global_row, multiplier);
					subtractRowThreads(mini_I, j, global_row, multiplier);
				}
			}

			#pragma omp barrier
			#pragma omp single
			for (int64_t i = 0; i < arows; i++)
			{
				int mini_matrix_idx = i % x;
				A.setRow(i, mini[mini_matrix_idx].getRow(i));
			}
			#pragma omp single
			for (int64_t i = 0; i < arows; i++)
			{
				int mini_matrix_idx = i % x;
				I.setRow(i, miniI[mini_matrix_idx].getRow(i));
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



/*	//auto miniA = miniParser(A.getMatrix(), A.getRows(), A.getColumns(), x);
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


	/*
	#pragma omp parallel for num_threads(x)
		for (int i = 0; i < x; i++)
		{
			minimatrix& mini_A = mini[i];

			for (int64_t j = 0; j < mini_A.getParsedMatrix().size(); j++)
			{
				int64_t global_row = i + j * x;
				int64_t maxRowIndex = global_row;
				for (int64_t k = global_row + x; k < arows; k += x)
				{
					if (A.get(k, global_row) > A.get(maxRowIndex, global_row))
					{
						maxRowIndex = k;
					}
				}

				if (maxRowIndex != global_row)
				{
					A.swapRows(global_row, maxRowIndex);
					I.swapRows(global_row, maxRowIndex);
				}

				int64_t pivot = A.get(global_row, global_row);

				divideRow(A, global_row, pivot);
				divideRow(I, global_row, pivot);

				for (int64_t j = 0; j < arows; j++)
				{
					if (j != global_row)
					{
						int64_t multiplier = A.get(j, global_row);

						subtractRow(A, j, global_row, multiplier);
						subtractRow(I, j, global_row, multiplier);
					}
				}
			}
		}*/