// PC_assignment1.cpp : Defines the entry point for the application.
//

#include "PC_assignment1.h"
#include "globals.h"
#include "matrix.h"
#include "gauss_methods.cpp"
#include "parallel_gauss_methods_basic.h"
#include "parallel_gauss_methods_advanced.h"
#include <iomanip>
#include <vector>
#include <string>
#include "benchmarking.h"

void parseArgs(const std::vector<std::string>& args)
{
	for (auto i = 0; i < args.size(); i++)
	{
		if (args[i] == "-a" && i + 1 < args.size())
		{
			if (args[i + 1] == "seq")
			{
				globals::algorithm = globals::Algorithm::SEQ;
			}
			else if (args[i + 1] == "bas")
			{
				globals::algorithm = globals::Algorithm::BASIC;
			}
			else if (args[i + 1] == "adv")
			{
				globals::algorithm = globals::Algorithm::ADVANCED;
			}
		}

		if (args[i] == "-m" && i + 1 < args.size())
		{
			if (args[i + 1] == "max")
			{
				globals::threadMode = globals::ThreadMode::MAX;
			}
			else if (args[i + 1] == "loc")
			{
				globals::threadMode = globals::ThreadMode::LOCAL;
			}
		}
		if (args[i] == "-n" && i + 1 < args.size())
		{
			globals::matrixSize = std::stol(args[i + 1]);
		}
		if (args[i] == "-p" && i + 1 < args.size())
		{
			globals::primeNumber = std::stol(args[i + 1]);
		}
		if (args[i] == "-t" && i + 1 < args.size())
		{
			globals::threadsAmount = std::stol(args[i + 1]);
		}
		if (args[i] == "-s" && i + 1 < args.size())
		{
			globals::seed = std::stoi(args[i + 1]);
		}
	}
}

int main(int argc, char* args[])
{
	if (argc > 1)
	{
		std::vector<std::string> argsVector(args, args + argc);
		parseArgs(argsVector);

		std::string str;

		for (auto x : argsVector)
		{
			str.append(x);
		}

		Logger::log(str);
	}


	/*myMatrix A(globals::matrixSize);
	myMatrix* result; bool x = true;

	A.fill_matrix(globals::seed);
	A.print();

	Gauss gauss;
	ParallelGauss parallelGaussBasic;
	ParallelGaussAdvanced parallelGaussAdvanced;*/

	/*
	//ZUSIE switch
#pragma region Switch_for_ZUSIE

	switch (globals::algorithm)
	{
	case globals::Algorithm::SEQ:
		result = new myMatrix(gauss.Solve(A));

		A.fill_matrix(globals::seed);
		x = gauss.checkIfInversionIsCorrect(A, *result);
		A.print();
		std::cout << "__________" << std::endl;
		result->print();

		break;
	case globals::Algorithm::BASIC:
		result = new myMatrix(parallelGaussBasic.Solve(A));

		A.fill_matrix(globals::seed);
		x = gauss.checkIfInversionIsCorrect(A, *result);
		A.print();
		std::cout << "__________" << std::endl;
		result->print();

		break;
	case globals::Algorithm::ADVANCED:
		result = new myMatrix(parallelGaussAdvanced.Solve(A));

		A.fill_matrix(globals::seed);
		x = gauss.checkIfInversionIsCorrect(A, *result);
		A.print();
		std::cout << "__________" << std::endl;
		result->print();

		break;
	default:
		std::cout << "Invalid algorithm" << std::endl;
		return -1;
		break;
	}
#pragma endregion
*/
	
	ParallelGaussAdvanced gauss;
	//ParallelGauss gauss;
	for (int i = 5; i < 6; i++)
	{
		myMatrix A(i);
		bool x = true;

		A.fill_matrix(globals::seed);
		//A.print();
		myMatrix result = gauss.Solve(A);

		//std::cout << "______________" << std::endl;
		//result.print();

		A.fill_matrix(globals::seed);
		x = gauss.checkIfInversionIsCorrect(A, result);

		if (!x)
		{
			std::cout << "Error with i = " << i << std::endl;
		}
		
	}
}

