#include "globals.h"

namespace globals
{
	extern int64_t primeNumber{ 109 }; //primeNumber = 11 for demo

	extern int threadsAmount{ 1 };

	extern int seed{ 14 };

	extern int matrixSize{ 4 };

	extern ThreadMode threadMode{ ThreadMode::MAX };

	extern Algorithm algorithm{ Algorithm::SEQ };
}