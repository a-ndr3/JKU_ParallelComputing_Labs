#include "benchmarking.h"

double start;
double end;
double elapsed;

myBenchmarks::myBenchmarks()
{
	start = 0.0000000000000;
}
myBenchmarks::~myBenchmarks()
{

}

void myBenchmarks::startTimer()
{
	start = MPI_Wtime();
}

double myBenchmarks::getTime() {
    return MPI_Wtime() - start;
}