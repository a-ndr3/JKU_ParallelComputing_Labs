#include <omp.h>
#include "benchmarking.h"

double start;
double end;
double elapsed;

myBenchmarks::myBenchmarks()
{
	start = 0.0000000000000;
	end = 0.0000000000000;
	elapsed = 0.0000000000000;
}
myBenchmarks::~myBenchmarks()
{

}

void myBenchmarks::startTimer()
{
	start = omp_get_wtime();
}

void myBenchmarks::stopTimer()
{
	end = omp_get_wtime();
}

double myBenchmarks::getElapsedTime()
{
	elapsed = end - start;
	return elapsed;
}

double myBenchmarks::getStartMinusGetTime()
{
	return omp_get_wtime() - start;
}