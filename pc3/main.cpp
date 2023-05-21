#include <iostream>
#include <mpi.h>
#include <vector>
#include "flatmatrix.h"
#include "parallel_gauss_methods_basic.h"
#include "logger.cpp"
#include "gauss_methods_flat.cpp"

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

    int rank = 0; int size = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    globals::mpi_rank = rank;
    globals::mpi_size = size;

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

    if (argc > 1)
    {
        std::vector<std::string> argsVector(argv, argv + argc);
        parseArgs(argsVector);

        std::string str;

        for (auto x : argsVector)
        {
            str.append(x);
        }

        Logger::log(str);
    }

    int N = globals::matrixSize;
    int M = globals::matrixSize;

    switch (globals::algorithm)
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
            A.print(); std::cout << "_________________" << std::endl;

            result = new flatmatrix(pg.solveParallel(A, N, M, globals::mpi_size, globals::mpi_rank));

            result->print();

            MPI_Finalize();

            break;
        }
    }

    MPI_Finalize();
    return 0;
}