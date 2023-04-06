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
	/*int64_t n = A.getRows();

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

		#pragma omp parallel for schedule(dynamic)
		for (int64_t i = n - 1; i >= 0; i--)
		{
			for (int64_t j = n - 1; j > i; j--)
			{
				int64_t multiplier = A.get(i, j);

				subtractRow(A, i, j, multiplier);
				subtractRow(I, i, j, multiplier);
			}
		}
	}*/

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

		#pragma omp parallel for schedule(dynamic)
		for (int64_t j = 0; j < n; j++)
		{
			if (j != i)
			{
				int64_t multiplier;
				#pragma omp critical //TODO check if really race condition
				{
				multiplier = A.get(j, i); // get multiplier to eliminate element below pivot
				}

				subtractRow(A, j, i, multiplier); //substract multiplied pivot row from current to make el below pivot = 0
				subtractRow(I, j, i, multiplier); //substract multiplied pivot row from following in identity
			}
		}
	}
}


//debug version
/*for (int64_t i = 0; i < n; i++)
	{
		std::cout<< "Start for i = " << i << std::endl;

		int64_t pivot = A.get(i, i); // diagonal element
		int64_t inv_pivot = mInv(pivot); // get modular inverse of pivot

		std::cout << "pivot = " << pivot << std::endl;
		std::cout << "InvPivot = " << inv_pivot << std::endl;

		if (inv_pivot == -1) //check if not regular based on mInv return value cuz modular inverse of 0 is 'null'
		{
			std::cout << "Matrix is not regular." << std::endl;
			return;
		}

		std::cout << "______Divide______" << std::endl;

		divideRow(A, i, pivot); // divide pivot row by pivot el to get 1

		std::cout << "A:_________ " << std::endl;
		A.print();

		divideRow(I, i, pivot); // divide following row of identity matrix by pivot el

		std::cout << "I:_________" << std::endl;
		I.print();

		//#pragma omp parallel for schedule(dynamic)
		for (int64_t j = i + 1; j < n; j++)
		{
			std::cout << "Start j: " << j << std::endl;
			int64_t multiplier = A.get(j, i);

			std::cout<< "multiplier = " << multiplier << std::endl;

			subtractRow(A, j, i, multiplier);

			std::cout << "______Substract______" << std::endl;
			std::cout << "A:_________ " << std::endl;
			A.print();

			subtractRow(I, j, i, multiplier);

			std::cout << "I:_________" << std::endl;
			I.print();
		}
	}*/

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

