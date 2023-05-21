#include <string>
#include "parallel_gauss_methods_basic.h"
#include "flatmatrix.h"
#include "support_methods.h"
#include "benchmarking.h"

void ParallelGauss::divideRow(int64_t* A, int64_t row, int64_t divisor, int64_t M)
{
    for (int64_t i = 0; i < M; i++)
    {
        A[row * M + i] = mDiv(A[row * M + i], divisor);
    }
}

void ParallelGauss::subtractRow(int64_t* A, int64_t targetRow, int64_t sourceRow, int64_t multiplier, int64_t M)
{
    for (int64_t i = 0; i < M; i++)
    {
        A[targetRow * M + i] = mSub(A[targetRow * M + i], mMul(multiplier, A[sourceRow * M + i]));
    }
}

/*void ParallelGauss::diagonalize(flatmatrix& aMatr, flatmatrix& iMatr, int M, int N, int size, int rank)
{
    int64_t* A = aMatr.getData();
    int64_t* I = iMatr.getData();

    int64_t rows_per_process = N / size; // Rows of matrix handled by each process

    // rows among processes
    if (rank == 0) {
        for (int i = 0; i < N; i++) {
            int target_process = i % size; // get process for the row
            if (target_process != 0) {
                // send the row to the target process
                MPI_Send(&A[i * M], M, MPI_INT64_T, target_process, 0, MPI_COMM_WORLD);
            }
        }
    } else {
        for (int i = 0; i < rows_per_process; i++) {
            // receive the rows from the master process
            MPI_Recv(&A[i * M], M, MPI_INT64_T, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
    }

    for (int64_t i = 0; i < N; i++)
    {
        int64_t pivot;
        if (i % size == rank) {
            pivot = A[(i / size) * M + i]; // diagonal element
            if (pivot == 0) {
                MPI_Abort(MPI_COMM_WORLD, -1); // abort if pivot is zero
            }
            divideRow(A, i / size, pivot, M); // divide pivot row by pivot el to get 1
            divideRow(I, i / size, pivot, M); // divide following row of identity matrix by pivot el
        }

        // broadcast pivot row to all processes
        MPI_Bcast(&pivot, 1, MPI_INT64_T, i % size, MPI_COMM_WORLD);

        for (int j = 0; j < rows_per_process; j++)
        {
            if (rank * rows_per_process + j != i) // exclude pivot row
            {
                int64_t multiplier = A[j * M + i]; // get multiplier to eliminate element below pivot

                subtractRow(A, j, i / size, multiplier, M); // subtract multiplied pivot row from current to make el below pivot = 0
                subtractRow(I, j, i / size, multiplier, M); // subtract multiplied pivot row from following in identity
            }
        }
    }

    // gather the permuted inverse matrix B from all processes
    if (rank == 0) {
        for (int i = 1; i < size; i++) {
            for (int j = 0; j < rows_per_process; j++) {
                // receive the rows from the other processes
                MPI_Recv(&I[(i * rows_per_process + j) * M], M, MPI_INT64_T, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }
        }
    } else {
        for (int i = 0; i < rows_per_process; i++) {
            // send the rows to the master process
            MPI_Send(&I[i * M], M, MPI_INT64_T, 0, 0, MPI_COMM_WORLD);
        }
    }
}*/

void ParallelGauss::diagonalize(flatmatrix& aMatr, flatmatrix& iMatr, int M, int N, int size, int rank)
{
    int64_t* A = aMatr.getData();
    int64_t* I = iMatr.getData();

    int rows_per_process = N / size; // Rows of matrix handled by each process

    int64_t* local_A = new int64_t[rows_per_process * M];
    int64_t* local_I = new int64_t[rows_per_process * M];

    MPI_Scatter(A, rows_per_process * M, MPI_INT64_T, local_A,
                    rows_per_process * M, MPI_INT64_T, 0,
                    MPI_COMM_WORLD);

    MPI_Scatter(I, rows_per_process * M, MPI_INT64_T, local_I,
                    rows_per_process * M, MPI_INT64_T, 0,
                    MPI_COMM_WORLD);


    for (int64_t i = 0; i < N; i++)
    {
        int64_t pivot;
        if (i % size == rank) {
            pivot = A[(i / size) * M + i]; // diagonal element
            if (pivot == 0) {
                MPI_Abort(MPI_COMM_WORLD, -1); // abort if pivot is zero
            }
            divideRow(local_A, i / size, pivot, M); // divide pivot row by pivot el to get 1
            divideRow(local_I, i / size, pivot, M); // divide following row of identity matrix by pivot el
        }

        // broadcast pivot row to all processes
        MPI_Bcast(&pivot, 1, MPI_INT64_T, i % size, MPI_COMM_WORLD);

        for (int j = 0; j < rows_per_process; j++)
        {
            if (rank * rows_per_process + j != i) // exclude pivot row
            {
                int64_t multiplier = local_A[j * M + i]; // get multiplier to eliminate element below pivot

                subtractRow(local_A, j, i / size, multiplier, M); // subtract multiplied pivot row from current to make el below pivot = 0
                subtractRow(local_I, j, i / size, multiplier, M); // subtract multiplied pivot row from following in identity
            }
        }
    }

    // gather the permuted inverse matrix B from all processes
        MPI_Gather(local_A, rows_per_process * M, MPI_INT64_T, A, rows_per_process * M, MPI_INT64_T, 0, MPI_COMM_WORLD);
        MPI_Gather(local_I, rows_per_process * M, MPI_INT64_T, I,rows_per_process * M, MPI_INT64_T, 0, MPI_COMM_WORLD);

    delete[] local_A;
    delete[] local_I;
}

flatmatrix ParallelGauss::solveParallel(flatmatrix &A, int N, int M, int size, int rank)
{
    myBenchmarks bench;

	flatmatrix I(N,M);

	I.make_flatmatrix_identityMatrix();

	//Logger::log("Parallel MPI method started for Matrix: " + std::to_string(N) + "x" + std::to_string(M));

	bench.startTimer();
	diagonalize(A, I, M, N, size, rank);
    double elapsed = bench.getTime();

	//Logger::logFull("Finished with elapsed time: ", elapsed);

	return I;
}

ParallelGauss::ParallelGauss() = default;

