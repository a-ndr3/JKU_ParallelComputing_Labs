// PC_assignment1.cpp : Defines the entry point for the application.
//

#include "PC_assignment1.h"
#include "globals.h"
#include "matrix.h"
#include "gauss_methods.cpp"
#include "parallel_gauss_methods_basic.cpp"
#include "parallel_gauss_methods_advanced.cpp"
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

	/*
	const int N = 100;
	std::vector<int> data(N);

	//omp_set_num_threads(4);

	#pragma omp parallel for schedule(dynamic)
		for (int i = 0; i < N; ++i) {

		data[i] = i * i;

		std::cout << "Thread " << omp_get_thread_num() << " processed element " << i << std::endl;
		}
	*/

	/*
	// myMatrix* A = new myMatrix(5, 5); heap alloc

	// myMatrix B(5, 5); stack alloc

	//myMatrix ** C = new myMatrix*[5];
	//C[0] = new myMatrix(5, 5);

	//A->fill_matrix();
	//B.fill_matrix();


	//A->print();
	//B.print();
	*/

	//ZUSIE switch
	/* 
	#pragma region Switch_for_ZUSIE
		switch (globals::algorithm)
		{
			myMatrix* result;

			myMatrix A(globals::matrixSize);

			A.fill_matrix(globals::seed);

		case globals::Algorithm::SEQ:
			GaussMethods::Gauss gauss;
			result = &gauss.Solve(A);
			break;
		case globals::Algorithm::BASIC:
			ParallelGaussBasic::ParallelGauss parallelGaussBasic;
			result = &parallelGaussBasic.Solve(A);
			break;
		case globals::Algorithm::ADVANCED:
			ParallelGaussAdvanced::ParallelGaussAdvanced parallelGaussAdvanced;
			result = &parallelGaussAdvanced.Solve(A);
			break;
		default:
			std::cout << "Invalid algorithm" << std::endl;
			return -1;
			break;
		}
	#pragma endregion
	*/

	//debug
	GaussMethods::Gauss gauss;
	ParallelGaussBasic::ParallelGauss parallelGaussBasic;
	ParallelGaussAdvanced::ParallelGaussAdvanced parallelGaussAdvanced;

	//myBenchmarks bench;
	//myMatrix A(4); //test data
	//A.fill_matrix(15); //test data
	//A.print();

	myMatrix A(50);

	A.fill_matrix(15);

	//myMatrix A(1200);  // solo ~ 62 sec parallbasic with 4 ~ 12sec				
	//A.fill_matrix(15);

	//myMatrix A(2000); //solo ~ 281 sec parallbasic with 4 ~ 56sec
	//A.fill_matrix(15);

	//bench.startTimer();

	myMatrix result = parallelGaussBasic.Solve(A);
	
	//myMatrix result = gauss.Solve(A);
	//myMatrix result = parallelGaussAdvanced.Solve(A);	

	  //bench.stopTimer();

	  //std::cout << "Time elapsed: " << std::fixed << std::setprecision(13) << bench.getElapsedTime() << std::endl;
}
