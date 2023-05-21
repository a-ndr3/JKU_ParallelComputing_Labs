#ifndef GLOBALS_H
#define GLOBALS_H
#include <cstdint>

namespace globals 
{
	enum Algorithm
	{
		SEQ = 0,
		MPI_PARALLEL = 1
	};

	extern int64_t primeNumber;
	extern int seed;
	extern int matrixSize;
	extern Algorithm algorithm;
    extern int mpi_rank;
    extern int mpi_size;
}

#endif