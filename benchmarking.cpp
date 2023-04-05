#include <omp.h>

namespace Benchmarking 
{
	class myBenchmarks
	{
		double start;
		double end;
		double elapsed;

	public:
		myBenchmarks()
		{
			start = 0.0000000000000;
			end = 0.0000000000000;
			elapsed = 0.0000000000000;
		}
		~myBenchmarks()
		{

		}

		void startTimer()
		{
			start = omp_get_wtime();
		}

		void stopTimer()
		{
			end = omp_get_wtime();
		}

		double getElapsedTime()
		{
			elapsed = end - start;
			return elapsed;
		}
	};
}