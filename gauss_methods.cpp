#include "support_methods.h"
#include <cstring>
#include "globals.h"
#include "matrix.h"
#include <iostream>
#include "benchmarking.h"
#include "logger.cpp"

namespace GaussMethods
{
	class Gauss
	{
		myMatrix multiply_matrices(myMatrix& A, myMatrix& B)
		{
			int64_t n = A.getRows();
			int64_t m = B.getColumns();

			myMatrix C(n, m);

			for (int64_t i = 0; i < n; i++)
			{
				for (int64_t j = 0; j < m; j++)
				{
					int64_t sum = 0;
					for (int64_t k = 0; k < n; k++)
					{
						sum += (A.get(i, k) * B.get(k, j));
					}
					C.set(i, j, sum);
				}
			}
			return C;
		}

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
			
			Logger::log("Gauss method started");

			bench.startTimer();
			diagonalize(A, I);
			bench.stopTimer();

			Logger::logFull("Finished with elapsed time: ", bench.getElapsedTime());

			return I;
		}

		bool checkIfInversionIsCorrect(myMatrix& firstMatrix, myMatrix& matrixA)
		{
			auto tempMatr = multiply_matrices(firstMatrix, matrixA);

			for (int i = 0; i < tempMatr.getRows(); i++)
			{
				for (int j = 0; j < tempMatr.getColumns(); j++)
				{
					if (i != j)
					{
						if (abs(tempMatr.get(i, j) % globals::primeNumber) != 0)
							return false;
					}
					else
					{
						if (abs(tempMatr.get(i, j) % globals::primeNumber) != 1)
							return false;
					}
				}
			}
			return true;
		}
	};
}