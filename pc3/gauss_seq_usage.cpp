#include <iostream>
#include "flatmatrix.h"
#include "gauss_methods_flat.cpp"
#include <vector>

/*
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

int main(int argc, char *argv[])
{
    if (argc > 1)
    {
        std::vector<std::string> argsVector(argv, argv + argc);
        parseArgs(argsVector);

        std::string str;

        for (auto x : argsVector)
        {
            str.append(x);
        }
    }

    flatmatrix An(globals::matrixSize);
    flatmatrix* resultM;
    An.fillflatmatrix(globals::seed);

    GaussFlat gauss;

    resultM = new flatmatrix(gauss.Solve(An));

    return 0;
}

*/