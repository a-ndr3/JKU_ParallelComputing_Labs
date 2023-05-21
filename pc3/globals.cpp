#include "globals.h"

namespace globals
{
	extern int64_t primeNumber{ 109 }; //109

	extern int seed{ 32456 }; //54

	extern int matrixSize{ 7 };

	extern Algorithm algorithm{ Algorithm::MPI_PARALLEL };

    extern int mpi_rank{ 1 };

    extern int mpi_size{ 2 };
}