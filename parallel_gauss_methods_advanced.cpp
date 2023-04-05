#include "gauss_methods.cpp"
#include "benchmarking.h"

using namespace GaussMethods;

namespace ParallelGaussAdvanced
{
	class ParallelGaussAdvanced : public Gauss
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
			
		}

	public:
		ParallelGaussAdvanced() : Gauss() {}
		~ParallelGaussAdvanced() {}

		virtual myMatrix Solve(myMatrix& A)
		{
			myBenchmarks bench;

			long n = A.getRows();

			myMatrix I(n);

			I.make_it_identityMatrix();

			Logger::log("Parallel Advanced method started");

			bench.startTimer();
			diagonalize(A, I);
			bench.stopTimer();

			Logger::logFull("Finished with elapsed time: ", bench.getElapsedTime());

			return I;
		}
	};
}