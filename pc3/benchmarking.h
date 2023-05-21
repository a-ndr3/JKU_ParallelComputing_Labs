#ifndef BENCHMARKING_H
#define BENCHMARKING_H

#include <mpi.h>

class myBenchmarks
{
	double start;

public:
	myBenchmarks();
	~myBenchmarks();

	void startTimer();
	double getTime();
};

#endif