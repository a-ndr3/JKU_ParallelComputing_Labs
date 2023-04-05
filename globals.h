#ifndef globals_h
#define globals_h

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

	extern long primeNumber;
	extern long threadsAmount;
	extern int seed;
	extern int matrixSize;
	extern ThreadMode threadMode;
	extern Algorithm algorithm;
}

#endif