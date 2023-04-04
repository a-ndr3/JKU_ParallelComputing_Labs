#include "globals.h"
#include <math.h>

//Methods use global:primeNumber to create a modular arithmetic

static long mInv(long a)
{
	long r = globals::primeNumber; long old_r = a;
	long s = 0; long old_s = 1;
	while (r != 0)
	{
		long q = old_r / r;
		long r0 = r; r = old_r - q * r; old_r = r0;
		long s0 = s; s = old_s - q * s; old_s = s0;
	}
	return old_s >= 0 ? old_s : old_s + globals::primeNumber;
}

static long mAdd(long a, long b) { return (a + b) % globals::primeNumber; }
static long mSub(long a, long b) { return (a + globals::primeNumber - b) % globals::primeNumber; }
static long mMul(long a, long b) { return (a * b) % globals::primeNumber; }
static long mDiv(long a, long b) { return mMul(a, mInv(b)); }



