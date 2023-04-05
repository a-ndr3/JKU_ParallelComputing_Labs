#ifndef support_methods_h
#define support_methods_h

#include "globals.h"
#include <math.h>
#include <cstdint>

//Methods use global:primeNumber to create a modular arithmetic

static int64_t mInv(int64_t a)
{
	int64_t r = globals::primeNumber; int64_t old_r = a;
	int64_t s = 0; int64_t old_s = 1;
	while (r != 0)
	{
		int64_t q = old_r / r;
		int64_t r0 = r; r = old_r - q * r; old_r = r0;
		int64_t s0 = s; s = old_s - q * s; old_s = s0;
	}
	return old_s >= 0 ? old_s : old_s + globals::primeNumber;
}

static int64_t mAdd(int64_t a, int64_t b) { return (a + b) % globals::primeNumber; }
static int64_t mSub(int64_t a, int64_t b) { return (a + globals::primeNumber - b) % globals::primeNumber; }
static int64_t mMul(int64_t a, int64_t b) { return (a * b) % globals::primeNumber; }
static int64_t mDiv(int64_t a, int64_t b) { return mMul(a, mInv(b)); }

#endif // !support_methods_h