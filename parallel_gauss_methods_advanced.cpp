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

static void printDebug(vector<minimatrix>& mini, myMatrix& A) 
{
	cout << "A: " << endl;
	A.print();
	cout << "mini: " << endl;
	for (auto& x : mini)
		x.print();
}

void ParallelGaussAdvanced::diagonalize(myMatrix& A, myMatrix& I)
{
	auto threads = omp_get_max_threads();
	auto arows = A.getRows();

	if (threads > arows)
	{
		threads = arows;
	}

	myBenchmarks be;

	vector<minimatrix> mini;
	vector<minimatrix> miniI;

	mini.reserve(threads);
	miniI.reserve(threads);

	be.startTimer();
	for (int i = 0; i < threads; i++)
	{
		auto mmatr = std::make_unique<minimatrix>(threads);
		mmatr->parse(A, i);
		mini.push_back(*mmatr);
	}

	for (int i = 0; i < threads; i++)
	{
		auto mmatr = std::make_unique<minimatrix>(threads);
		mmatr->parse(I, i);
		miniI.push_back(*mmatr);
	}
	be.stopTimer();
	cout <<"push :" << be.getElapsedTime() << endl;
	be.startTimer();
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

			int64_t iswap = i % threads; 
			int64_t maxswap = maxRowIndex % threads;			

			mini[iswap].updateRow(i, A.getWholeRow(i));
			miniI[iswap].updateRow(i, I.getWholeRow(i));

			mini[maxswap].updateRow(maxRowIndex, A.getWholeRow(maxRowIndex));
			miniI[maxswap].updateRow(maxRowIndex, I.getWholeRow(maxRowIndex));
		}

		int64_t pivot = A.get(i, i);

		divideRow(A, i, pivot);
		divideRow(I, i, pivot);
		
		int64_t iswap = i % threads;
		mini[iswap].updateRow(i, A.getWholeRow(i));
		miniI[iswap].updateRow(i, I.getWholeRow(i));		
		
//		cout << "after swap : " << be.getStartMinusGetTime() << endl;

		//#pragma omp parallel for schedule(static) //schedule(dynamic)
		for (int t = 0; t < threads; t++)
		{

			minimatrix& mini_A = mini[t];
			minimatrix& mini_I = miniI[t];
			auto rowInd = mini_A.getRowIndexes(t);
//			cout << "inside for after get indices = " << "  time = " << be.getStartMinusGetTime() << endl;

			for (int64_t j = 0; j < rowInd.size(); j++)
			{
//				cout << "rowInd.size()  " << "  time = " << be.getStartMinusGetTime() << endl;
				if (rowInd[j] != i)
				{
					int64_t multiplier = A.get(rowInd[j], i);
					
					//cout << "get multiplier time = " << be.getStartMinusGetTime() << endl;
					subtractRowThreads(mini_A, j, A.getWholeRow(i), multiplier);
					subtractRowThreads(mini_I, j, I.getWholeRow(i), multiplier);
					//cout << "after substraction time = " << be.getStartMinusGetTime() << endl;
				}
			}
		}
//		cout << "after i= " << i  << "  time = " << be.getStartMinusGetTime() << endl;
		#pragma omp parallel for
		for (int i = 0; i < mini.size(); i++)
		{
			auto ind = mini[i].getRowIndexes(i);

			for (int k = 0; k < ind.size(); k++)
			{
				auto c = ind[k];

				A.setRow(c, mini[i].getRow(k));
				I.setRow(c, miniI[i].getRow(k));
			}
		}

//		cout << "after push into A = " << be.getStartMinusGetTime() << endl;
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