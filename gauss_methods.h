#include "support_methods.h"
#include <cstring>
#include "globals.h"

using namespace myMatrixNamespace;

void divideRow(myMatrix& A, long row, long divisor)
{
	for (long i = 0; i < A.getColumns(); i++)
	{
		A.set(row, i, mDiv(A.get(row, i), divisor)); //modular division of each el by divisor
	}
}

void subtractRow(myMatrix& A, long targetRow, long sourceRow, long multiplier)
{
	for (long i = 0; i < A.getColumns(); i++)
	{
		A.set(targetRow, i, mSub(A.get(targetRow, i), mMul(multiplier, A.get(sourceRow, i)))); // substr from target string str multipied 
	}
}

void diagonalize(myMatrix& A, myMatrix& I)
{
	long n = A.getRows();

	for (long i = 0; i < n; i++)
	{
		long pivot = A.get(i, i); // diagonal element
		long inv_pivot = mInv(pivot); // get modular inverse of pivot

		//check if not regular
		if (inv_pivot == -1)
		{
			cout << "Matrix is not regular." << endl;
			return;
		}

		divideRow(A, i, pivot); // divide pivot row by pivot el to get 1
		divideRow(I, i, pivot); // divide following row of identity matrix by pivot el

		for (long j = 0; j < n; j++)
		{
			if (j != i)
			{
				long multiplier = A.get(j, i); // get multiplier to eliminate element below pivot

				subtractRow(A, j, i, multiplier); //substract multiplied pivot row from current to make el below pivot = 0
				subtractRow(I, j, i, multiplier); //substract multiplied pivot row from following in identity
			}
		}
	}
}

myMatrix gaussJordanElimination(myMatrix& A)
{
	long n = A.getRows();

	myMatrix I(n);

	I.make_it_identityMatrix();

	diagonalize(A, I);

	return I;
}

myMatrix multiply_matrices(myMatrix& A, myMatrix& B) 
{
	long n = A.getRows();
	long m = B.getColumns();
	
	myMatrix C(n, m);
	
	for (long i = 0; i < n; i++)
	{
		for (long j = 0; j < m; j++)
		{
			long sum = 0;
			for (long k = 0; k < n; k++)
			{
				sum += (A.get(i, k) * B.get(k, j));
			}
			C.set(i, j, sum);
		}
	}
	return C;
}


bool chekIfInversionIsCorrect(myMatrix& firstMatrix, myMatrix& matrixA)
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