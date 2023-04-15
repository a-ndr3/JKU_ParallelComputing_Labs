#ifndef BENCHMARKING_H
#define BENCHMARKING_H

#include <omp.h>

class myBenchmarks
{
	double start;
	double end;
	double elapsed;

public:
	myBenchmarks();
	~myBenchmarks();

	void startTimer();

	void stopTimer();

	double getElapsedTime();
	double getStartMinusGetTime();
};

#endif