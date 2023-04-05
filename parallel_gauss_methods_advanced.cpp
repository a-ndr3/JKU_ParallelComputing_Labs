#include "gauss_methods.cpp"
#include "benchmarking.h"
#include <cstdint>

using namespace GaussMethods;

namespace ParallelGaussAdvanced
{
	class ParallelGaussAdvanced : public Gauss
	{
		void divideRow(myMatrix& A, int64_t row, int64_t divisor) override
		{
			for (int64_t i = 0; i < A.getColumns(); i++)
			{
				A.set(row, i, mDiv(A.get(row, i), divisor)); //modular division of each el by divisor
			}
		}

		void subtractRow(myMatrix& A, int64_t targetRow, int64_t sourceRow, int64_t multiplier) override
		{
			for (int64_t i = 0; i < A.getColumns(); i++)
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

			int64_t n = A.getRows();

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