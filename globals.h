#ifndef GLOBALS_H
#define GLOBALS_H
#include <cstdint>

namespace globals 
{
	enum ThreadMode
	{
		LOCAL = 1,
		MAX = 2
	};

	enum Algorithm
	{
		SEQ,
		BASIC = 1,
		ADVANCED = 2
	};

	extern int64_t primeNumber;
	extern int threadsAmount;
	extern int seed;
	extern int matrixSize;
	extern ThreadMode threadMode;
	extern Algorithm algorithm;
}

#endif