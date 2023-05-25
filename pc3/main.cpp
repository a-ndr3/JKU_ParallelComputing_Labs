#include <iostream>
#include <mpi.h>
#include <vector>
#include <cstdint>
#include "flatmatrix.h"
#include "logger.cpp"
#include "gauss_methods_flat.cpp"
#include "benchmarking.h"


void divideRow(int64_t* A, int64_t row, int64_t divisor, int M);

void subtractRow(int64_t *A, int64_t targetRow, int64_t* sourceRow, int64_t multiplier, int M);

void swapRows(int64_t *A, int64_t row1, int64_t row2, int M);

void parseArgs(const std::vector<std::string>& args)
{
    for (auto i = 0; i < args.size(); i++)
    {
        if (args[i] == "-alg" && i + 1 < args.size())
        {
            if (args[i + 1] == "par")
            {
                globals::algorithm = globals::Algorithm::MPI_PARALLEL;
            }
        }
        if (args[i] == "-ms" && i + 1 < args.size())
        {
            globals::matrixSize = std::stoi(args[i + 1]);
        }
        if (args[i] == "-prime" && i + 1 < args.size())
        {
            globals::primeNumber = std::stoi(args[i + 1]);
        }
        if (args[i] == "-seed" && i + 1 < args.size())
        {
            globals::seed = std::stoi(args[i + 1]);
        }
    }
}


int main(int argc, char *argv[]) {

    MPI_Init(&argc, &argv);

    int rank_global; int size_global;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank_global);
    MPI_Comm_size(MPI_COMM_WORLD, &size_global);

    if (rank_global == 0 && argc > 1)
    {
        std::vector<std::string> argsVector(argv, argv + argc);
        parseArgs(argsVector);

        std::string str;

        for (auto x : argsVector)
        {
            str.append(x);
        }
    }

    MPI_Bcast(&globals::algorithm, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&globals::matrixSize, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&globals::primeNumber, 1, MPI_INT64_T, 0, MPI_COMM_WORLD);
    MPI_Bcast(&globals::seed, 1, MPI_INT, 0, MPI_COMM_WORLD);

    flatmatrix* aMatrix_global;
    flatmatrix* iMatrix_global;

    if (rank_global == 0)
    {
        aMatrix_global = new flatmatrix(globals::matrixSize, globals::matrixSize);
        iMatrix_global = new flatmatrix(globals::matrixSize, globals::matrixSize);
        aMatrix_global->fillflatmatrix(globals::seed);
        iMatrix_global->make_flatmatrix_identityMatrix();
    }

    int N = globals::matrixSize;
    int M = globals::matrixSize;

    int rows_per_process_local = globals::matrixSize / size_global;

    auto* aMatrix_local = new int64_t[rows_per_process_local * N];
    auto* iMatrix_local = new int64_t[rows_per_process_local * N];

   int64_t* pivotRow = new int64_t[N];
   int64_t* inversed_pivotRow = new int64_t[N];

    myBenchmarks bench;

    bench.startTimer();

    MPI_Scatter(aMatrix_global->getData(), rows_per_process_local * N, MPI_INT64_T, aMatrix_local,
                rows_per_process_local * N, MPI_INT64_T, 0,
                MPI_COMM_WORLD);

    MPI_Scatter(iMatrix_global->getData(), rows_per_process_local * N, MPI_INT64_T, iMatrix_local,
                rows_per_process_local * N, MPI_INT64_T, 0,
                MPI_COMM_WORLD);

    for (int64_t i = 0; i < N; i++)
    {
        if (i / rows_per_process_local == rank_global)
        {
            int64_t row = i % rows_per_process_local;
            int64_t pivot = aMatrix_local[row * N + i];

            divideRow(aMatrix_local, row, pivot, N);
            divideRow(iMatrix_local, row, pivot, N);

            for (int64_t j = 0; j < N; j++)
            {
                pivotRow[j] = aMatrix_local[row * N + j];
                inversed_pivotRow[j] = iMatrix_local[row * N + j];
            }
        }

        MPI_Bcast(pivotRow, N, MPI_INT64_T, i / rows_per_process_local, MPI_COMM_WORLD);
        MPI_Bcast(inversed_pivotRow, N, MPI_INT64_T, i / rows_per_process_local, MPI_COMM_WORLD);


        for (int64_t j = 0; j < rows_per_process_local; j++)
        {
            if (j + rank_global * rows_per_process_local == i)
            {
                continue;
            }

            int64_t multiplier = aMatrix_local[j * N + i];

            subtractRow(aMatrix_local, j, pivotRow, multiplier, N);
            subtractRow(iMatrix_local, j, inversed_pivotRow, multiplier, N);
        }
        MPI_Barrier(MPI_COMM_WORLD);

    }
    MPI_Gather(aMatrix_local, rows_per_process_local * N, MPI_INT64_T, aMatrix_global->getData(), rows_per_process_local * N, MPI_INT64_T, 0, MPI_COMM_WORLD);
    MPI_Gather(iMatrix_local, rows_per_process_local * N, MPI_INT64_T, iMatrix_global->getData(),rows_per_process_local * N, MPI_INT64_T, 0, MPI_COMM_WORLD);

   auto timerResult = bench.getTime();

  if (rank_global == 0)
  {
      std::cout<<"Time: "<<timerResult<<std::endl;

      writeInFile("log.txt",  "[PARALLEL] Matrix_size: " + std::to_string(globals::matrixSize) + "\n" + std::to_string(timerResult));
  }

    MPI_Finalize();

    return 0;
}

void divideRow(int64_t* A, int64_t row, int64_t divisor, int M) {
    for (int64_t i = 0; i < M; i++)
    {
        A[row * M + i] = mDiv(A[row * M + i], divisor);
    }
}

void subtractRow(int64_t *A, int64_t targetRow, int64_t* sourceRow, int64_t multiplier, int M) {
    for (int64_t i = 0; i < M; i++)
    {
        A[targetRow * M + i] = mSub(A[targetRow * M + i], mMul(multiplier, sourceRow[i]));
    }
}

void swapRows(int64_t *A, int64_t row1, int64_t row2, int N) {
    for (int64_t i = 0; i < N; i++)
    {
        int64_t temp = A[row1 * N + i];
        A[row1 * N + i] = A[row2 * N + i];
        A[row2 * N + i] = temp;
    }
}

