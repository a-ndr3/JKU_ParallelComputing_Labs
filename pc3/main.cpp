#include <iostream>
#include <mpi.h>
#include <vector>
#include "flatmatrix.h"
#include "parallel_gauss_methods_basic.h"
#include "logger.cpp"
#include "gauss_methods_flat.cpp"
#include "logger2.cpp"

void divideRow(flatmatrix& A, int64_t row, int64_t divisor, int M);

void subtractRow(int64_t *A, int targetRow, int64_t sourceRow, int64_t multiplier, int M);

static void writeInFile(const std::string& filename, const std::string& content)
{
    std::ofstream file;
    file.open(filename, std::ios::app);
    file << content << std::endl;
    file.close();
}

void parseArgs(const std::vector<std::string>& args)
{
    for (auto i = 0; i < args.size(); i++)
    {
        if (args[i] == "-alg" && i + 1 < args.size())
        {
            if (args[i + 1] == "seq")
            {
                globals::algorithm = globals::Algorithm::SEQ;
            }
            else if (args[i + 1] == "par")
            {
                globals::algorithm = globals::Algorithm::MPI_PARALLEL;
            }
        }
        if (args[i] == "-mpi_rank" && i + 1 < args.size())
        {
            globals::mpi_rank = std::stoi(args[i + 1]);
        }
        if (args[i] == "-mpi_size" && i + 1 < args.size())
        {
            globals::mpi_size = std::stoi(args[i + 1]);
        }
        if (args[i] == "-n" && i + 1 < args.size())
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

    int rank; int size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0 && argc > 1)
    {
        std::vector<std::string> argsVector(argv, argv + argc);
        parseArgs(argsVector);

        std::string str;

        for (auto x : argsVector)
        {
            str.append(x);
        }

        writeInFile("log.txt",str);
        //Logger::log(str);
    }

    MPI_Bcast(&globals::algorithm, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&globals::matrixSize, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&globals::primeNumber, 1, MPI_INT64_T, 0, MPI_COMM_WORLD);
    MPI_Bcast(&globals::seed, 1, MPI_INT, 0, MPI_COMM_WORLD);

    flatmatrix A(globals::matrixSize, globals::matrixSize);
    flatmatrix I(globals::matrixSize, globals::matrixSize);

    auto pg = *new ParallelGauss();

    if (rank == 0)
    {
        A.fillflatmatrix(globals::seed);
        I.make_flatmatrix_identityMatrix();
    }

    int N = globals::matrixSize;
    int M = globals::matrixSize;

    int rows_per_process = globals::matrixSize / size;

    int64_t* local_A = new int64_t[rows_per_process * N];
    int64_t* local_I = new int64_t[rows_per_process * N];

    if (rank == 0)
    {
        MPI_Scatter(A.getData(), rows_per_process * N, MPI_INT64_T, local_A,
                    rows_per_process * N, MPI_INT64_T, 0,
                    MPI_COMM_WORLD);

        MPI_Scatter(I.getData(), rows_per_process * N, MPI_INT64_T, local_I,
                    rows_per_process * N, MPI_INT64_T, 0,
                    MPI_COMM_WORLD);
    }

    if (rank == 0)
    {
        A.print();
        std::cout<<"___________"<<std::endl;
    }

    for (int64_t i = 0; i < N; i++)
    {
        if (rank == 0)
        {
            int64_t pivot;
            if (i % size == rank) {
                pivot = A.getData()[(i / size) * M + i]; // diagonal element
                if (pivot == 0) {
                    MPI_Abort(MPI_COMM_WORLD, -1); // abort if pivot is zero
                }
                divideRow(A, i / size, pivot, M); // divide pivot row by pivot el to get 1
                divideRow(I, i / size, pivot, M); // divide following row of identity matrix by pivot el
            }
        }

        // broadcast pivot row to all processes
        //cast ili ne cast?
        if (rank == 0)
        {
            MPI_Scatter(A.getData(), rows_per_process * N, MPI_INT64_T, local_A,
                    rows_per_process * N, MPI_INT64_T, 0,
                    MPI_COMM_WORLD);

            MPI_Scatter(I.getData(), rows_per_process * N, MPI_INT64_T, local_I,
                    rows_per_process * N, MPI_INT64_T, 0,
                    MPI_COMM_WORLD);
        }

       // MPI_Bcast(&pivot, 1, MPI_INT64_T, i % size, MPI_COMM_WORLD);
       // if (i % size != rank) {
        //    MPI_Bcast(A.getData()  + (i / size) * M, M, MPI_INT64_T, i % size, MPI_COMM_WORLD);
       //     MPI_Bcast(I.getData()   + (i / size) * M, M, MPI_INT64_T, i % size, MPI_COMM_WORLD);
       // }

        for (int j = 0; j < rows_per_process; j++)
        {
            if (rank * rows_per_process + j != i) // exclude pivot row
            {                        //local_A ?
                int64_t multiplier = A.getData()[j * M + i]; // get multiplier to eliminate element below pivot

                subtractRow(local_A, j, i / size, multiplier, M); // subtract multiplied pivot row from current to make el below pivot = 0
                subtractRow(local_I, j, i / size, multiplier, M); // subtract multiplied pivot row from following in identity
            }
        }
        MPI_Gather(local_A, rows_per_process * M, MPI_INT64_T, A.getData(), rows_per_process * M, MPI_INT64_T, 0, MPI_COMM_WORLD);
        MPI_Gather(local_I, rows_per_process * M, MPI_INT64_T, I.getData(),rows_per_process * M, MPI_INT64_T, 0, MPI_COMM_WORLD);
    }

    //MPI_Gather(local_A, rows_per_process * M, MPI_INT64_T, A.getData(), rows_per_process * M, MPI_INT64_T, 0, MPI_COMM_WORLD);
   // MPI_Gather(local_I, rows_per_process * M, MPI_INT64_T, I.getData(),rows_per_process * M, MPI_INT64_T, 0, MPI_COMM_WORLD);

    delete[] local_A;
    delete[] local_I;

    MPI_Finalize();

    I.print();

    return 0;
}

void divideRow(flatmatrix& A, int64_t row, int64_t divisor, int M) {
    for (int64_t i = 0; i < M; i++)
    {
        A.getData()[row * M + i] = mDiv(A.getData()[row * M + i], divisor);
    }
}

void subtractRow(int64_t *A, int targetRow, int64_t sourceRow, int64_t multiplier, int M) {
    for (int64_t i = 0; i < M; i++)
    {
        A[targetRow * M + i] = mSub(A[targetRow * M + i], mMul(multiplier, A[sourceRow * M + i]));
    }
}


/*
    int* matrix = new int[16];
    int* res = new int[4];
    for (int i = 0; i < 16; i++)
    {
        matrix[i] = i*(rank+1);
    }

    MPI_Scatter(matrix, 4, MPI_INT, res, 4, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        std::ofstream ofs("log_rank0.txt", std::ofstream::app);
        for (int64_t i = 0; i < 4; i++)
        {
            ofs << res[i] << " ";
        }
        ofs.close();
    }
    if (rank == 1)
    {
        std::ofstream ofs("log_rank1.txt", std::ofstream::app);
        for (int64_t i = 0; i < 4; i++)
        {
            ofs << res[i] << " ";
        }
        ofs.close();
    }
    if (rank == 2)
    {
        std::ofstream ofs("log_rank2.txt", std::ofstream::app);
        for (int64_t i = 0; i < 4; i++)
        {
            ofs << res[i] << " ";
        }
        ofs.close();
    }
    if (rank == 3)
    {
        std::ofstream ofs("log_rank3.txt", std::ofstream::app);
        for (int64_t i = 0; i < 4; i++)
        {
            ofs << res[i] << " ";
        }
        ofs.close();
    }
*/


/*switch (globals::algorithm)
{
    case globals::SEQ:
    {
        Logger::log("Algorithm: SEQ");

        flatmatrix An(globals::matrixSize);
        flatmatrix* resultM;

        An.fillflatmatrix(globals::seed);
        GaussFlat gauss;
        An.print(); std::cout << "_________________" << std::endl;
        resultM = new flatmatrix(gauss.Solve(An));
        resultM->print();
        break;
    }
    case globals::MPI_PARALLEL:
    {
        Logger::log("Algorithm: MPI_PARALLEL");

        ParallelGauss pg = *new ParallelGauss();

        flatmatrix A(N,M);
        flatmatrix* result;

        A.fillflatmatrix(globals::seed);
        //A.print(); std::cout << "_________________" << std::endl;

        globals::mpi_rank = rank;
        globals::mpi_size = size;

        result = new flatmatrix(pg.solveParallel(A, N, M, size, rank));

        //result->print();
        MPI_Finalize();
        break;
    }
}*/