// PC_assignment1.cpp : Defines the entry point for the application.
//

#include "PC_assignment1.h"
#include "globals.h"
#include "matrix.h"
//#include "gauss_methods.cpp"
#include "parallel_gauss_methods_basic.cpp"
//#include "benchmarking.cpp"
#include <iomanip>
#include <vector>
#include <string>

using namespace std;
using namespace Benchmarking;
//using namespace GaussMethods;
using namespace ParallelGaussBasic;

void parseArgs(const std::vector<std::string>& args)
{
	for (auto i = 0; i < args.size(); i++) {
		if (args[i] == "-p" && i + 1 < args.size()) {
			globals::primeNumber = std::stol(args[i + 1]);
		}
		else if (args[i] == "-t" && i + 1 < args.size()) {
			globals::threadsAmount = std::stol(args[i + 1]);
		}
		else if (args[i] == "-s" && i + 1 < args.size()) {
			globals::seed = std::stoi(args[i + 1]);
		}
	}
}

int main(int argc, char* args[])
{
	if (argc > 0) 
	{
		std::vector<std::string> argsVector(args, args + argc);
		parseArgs(argsVector);
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

	myBenchmarks bench;

	Gauss gauss;
	ParallelGauss parallelGaussBasic;// = new ParallelGauss(true);

	myMatrix A(4); //test data

	A.fill_matrix(15); //test data

	//A.print();

	
	//myMatrix A(1200);  // solo ~ 62 sec parallbasic with 4 ~ 12sec				
	//A.fill_matrix(15);

	//myMatrix A(2000); //solo ~ 281 sec parallbasic with 4 ~ 56sec
	//A.fill_matrix(15);

	bench.startTimer();
	myMatrix result = parallelGaussBasic.Solve(A);
	//myMatrix result = gauss.Solve(A);
	bench.stopTimer();

	cout << "Time elapsed: " << fixed << setprecision(13) << bench.getElapsedTime() << endl;
}
