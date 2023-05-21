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

            MPI_Comm_rank(MPI_COMM_WORLD, &globals::mpi_rank);
            MPI_Comm_size(MPI_COMM_WORLD, &globals::mpi_size);

            ParallelGauss pg = *new ParallelGauss();

            flatmatrix A(N,M);
            flatmatrix* result;

            A.fillflatmatrix(globals::seed);
            //A.print(); std::cout << "_________________" << std::endl;

            result = new flatmatrix(pg.solveParallel(A, N, M, globals::mpi_size, globals::mpi_rank));

            //result->print();

            MPI_Finalize();

            break;
        }
    }
    return 0;
}