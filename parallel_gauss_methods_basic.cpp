#include "gauss_methods.cpp"
#include "benchmarking.cpp"

using namespace GaussMethods;

namespace ParallelGaussBasic
{
	class ParallelGauss : public Gauss
	{
		bool useThreads = false;

		void divideRow(myMatrix& A, long row, long divisor) override
		{
			for (long i = 0; i < A.getColumns(); i++)
			{
				A.set(row, i, mDiv(A.get(row, i), divisor)); //modular division of each el by divisor
			}
		}

		void subtractRow(myMatrix& A, long targetRow, long sourceRow, long multiplier) override
		{
			for (long i = 0; i < A.getColumns(); i++)
			{
				A.set(targetRow, i, mSub(A.get(targetRow, i), mMul(multiplier, A.get(sourceRow, i)))); // substr from target string str multipied 
			}
		}

		void diagonalize(myMatrix& A, myMatrix& I) override
		{
			long n = A.getRows();
			
			if (useThreads)
			{
				omp_set_num_threads(globals::threadsAmount);
			}
			else 
			{
				omp_set_num_threads(omp_get_max_threads());
			}

			for (long i = 0; i < n; i++)
			{
				long pivot = A.get(i, i); // diagonal element
				long inv_pivot = mInv(pivot); // get modular inverse of pivot

				if (inv_pivot == -1) //check if not regular based on mInv return value cuz modular inverse of 0 is 'null'
				{
					std::cout << "Matrix is not regular." << std::endl;
					return;
				}

				divideRow(A, i, pivot); // divide pivot row by pivot el to get 1
				divideRow(I, i, pivot); // divide following row of identity matrix by pivot el

				#pragma omp parallel for schedule(dynamic)
				for (long j = i + 1; j < n; j++)
				{
					long multiplier = A.get(j, i);

					subtractRow(A, j, i, multiplier);
					subtractRow(I, j, i, multiplier);
				}
			}
		}

	public:
		ParallelGauss() : Gauss() {}
		ParallelGauss(bool useThreadsGlobal) : Gauss()
		{
			useThreads = useThreadsGlobal;
		}
		~ParallelGauss() {}
	};
}