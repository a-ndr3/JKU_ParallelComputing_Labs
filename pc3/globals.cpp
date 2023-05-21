#include "globals.h"

namespace globals
{
	extern int64_t primeNumber{ 109 }; //109

	extern int seed{ 32456 }; //54

	extern int matrixSize{ 5 };

	extern Algorithm algorithm{ Algorithm::MPI_PARALLEL };

    extern int mpi_rank{ 0 };

    extern int mpi_size{ 0 };
}