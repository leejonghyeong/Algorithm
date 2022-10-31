#pragma once
#include "BigInt.h"

bool primalityTest(BigInt n, int k);

BigInt generateRandomPrime(int n_size);

BigInt getModuloInverse(BigInt a, BigInt m);