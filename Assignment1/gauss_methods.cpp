#ifndef gauss_methods_cpp
#define gauss_methods_cpp

#include "support_methods.h"
#include <cstring>
#include "globals.h"
#include "matrix.h"
#include <iostream>
#include "benchmarking.h"
#include "logger.cpp"
#include <vector>

class Gauss
{	
	virtual void divideRow(myMatrix& A, int64_t row, int64_t divisor)
	{
		for (int64_t i = 0; i < A.getColumns(); i++)
		{
			A.set(row, i, mDiv(A.get(row, i), divisor)); //modular division of each el by divisor
		}
	}

	virtual void subtractRow(myMatrix& A, int64_t targetRow, int64_t sourceRow, int64_t multiplier)
	{
		for (int64_t i = 0; i < A.getColumns(); i++)
		{
			A.set(targetRow, i, mSub(A.get(targetRow, i), mMul(multiplier, A.get(sourceRow, i)))); // substr from target string str multipied 
		}
	}

	virtual void diagonalize(myMatrix& A, myMatrix& I)
	{
		int64_t n = A.getRows();

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

			int64_t pivot = A.get(i, i); // diagonal element

			divideRow(A, i, pivot); // divide pivot row by pivot el to get 1

			divideRow(I, i, pivot); // divide following row of identity matrix by pivot el

			for (int64_t j = 0; j < n; j++)
			{
				if (j != i)
				{
					int64_t multiplier = A.get(j, i); // get multiplier to eliminate element below pivot

					subtractRow(A, j, i, multiplier); //substract multiplied pivot row from current to make el below pivot = 0
					subtractRow(I, j, i, multiplier); //substract multiplied pivot row from following in identity

				}
			}
		}
	}

public:

	virtual myMatrix Solve(myMatrix& A)
	{
		myBenchmarks bench;

		int64_t n = A.getRows();

		myMatrix I(n);

		I.make_it_identityMatrix();

		Logger::log("Gauss method started for Matrix = " + std::to_string(n));

		bench.startTimer();
		diagonalize(A, I);
		bench.stopTimer();

		Logger::logFull("Finished with elapsed time: ", bench.getElapsedTime());

		return I;
	}

	bool checkIfInversionIsCorrect(myMatrix& matrix, myMatrix& inverseMatrix)
	{	
		typedef std::vector<std::vector<int64_t>> Matrix;
		int64_t n = matrix.getRows();
		
		Matrix result(n, std::vector<int64_t>(n, 0));

		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < n; ++j) {
				for (int k = 0; k < n; ++k) {
					int64_t product = static_cast<int64_t>(matrix.get(i,k)) * inverseMatrix.get(k,j);
					result[i][j] = (result[i][j] + product) % globals::primeNumber;
				}
			}
		}
		
		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < n; ++j) {
				if (i == j && result[i][j] != 1) {
					return false;
				}
				if (i != j && result[i][j] != 0) {
					return false;
				}
			}
		}
		return true;
	}
};
#endif // !gauss_methods_cpp
