#include "globals.h"

namespace globals
{
	extern int64_t primeNumber{ 109 }; //109

	extern int threadsAmount{ 8 }; //the idea was to use with thread mode // added for future use

	extern int seed{ 32456 }; //54

	extern int matrixSize{ 2000 };

	extern ThreadMode threadMode{ ThreadMode::MAX }; //added for future use

	extern Algorithm algorithm{ Algorithm::ADVANCED };
}