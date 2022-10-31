#include "BigInt.h"
#include <vector>
#include <tuple>
#include <cassert>

using namespace std;

//to generate random 50bits prime
//randomly generate 50bits number and perform primality tests

vector<int> small_primes = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97 };

bool primalityTest(BigInt n, int k) {
	/*
	Miller-Rabin Pramality Test
	Pomerance, Selfridge, Wagstaff and Jaeschke verified that 
	if n < 341,550,071,728,321, enough to test a = 2, 3, 5, 7, 11, 13, 17.

	Algorithm:
		Fermat's little theorem says that for (a, n) = 1,  a^(n-1) = 1 mod n
		let n = 2^s d + 1 where d is odd. 
		Then, (a^d - 1) (a^d + 1) (a^2d + 1) ... (a^(2^(s-1) d) + 1) = 0 mod n.
		If n is a prime, then n divides one of these factors.
		
		The idea of algorithm is that if n divides one of them , n is probably a prime.
	*/

	//test for small primes
	for (const auto& prime : small_primes) {
		if (n % prime == 0 && n > prime)
			return false;
	}

	vector<int> base_list = { 2,3,5,7,11,13,17 };
	int s = 0;
	BigInt d = n - 1;

	while (d % 2 == 0) {
		s += 1;
		d = d / 2;
	}

	while (k-- > 0) {
		BigInt a = rand() % base_list.size();
		BigInt x = a.mod_power(d, n);
		if (x == 1) {
			continue;
		}
		for (int i = 0; i < s; i++) {
			x = x.mod_power(2, n);
			if (x == n - 1) {
				break;
			}
		}
		if (x == n - 1) continue;

		//
		return false;
	}

	return true;
}


BigInt generateRandomPrime(int n_size) {
	string number_string;

	for (int i = 0; i < n_size; i++) {
		if (i == 0)
			number_string.push_back((char)(1 + rand() % 9 + 48));
		else
			number_string.push_back((char)(rand() % 10 + 48));
	}

	BigInt rand_number(number_string);

	if (primalityTest(rand_number, 5))
		return rand_number;
	else
		return generateRandomPrime(n_size);
}


tuple<BigInt, BigInt, BigInt> extendedEuclidean(BigInt a, BigInt b) {
	/*
	Let a = b*q1 + r1 and bx' + r1y' = gcd(b, r1)
	Then, (a-b*q1)y' + b x' = a*y' + b(x'-q1*y') = gcd(b, r1) = gcd(a, b).
	So, if we let x = y', y = x' - (a / b)*y', then ax + by = gcd(a, b) holds.

	return tuple (gcd, x, y)
	*/

	//when the remainder is zero
	if (b == 0) {
		//return x'= 0, y' = 1 so that x = 1, y = - (a / b) -> a + b (- (a/b)) = 0
		return make_tuple(b, 0, 1);
	}

	BigInt gcd, x, y;
	tie(gcd, x, y) = extendedEuclidean(b, a % b);

	return make_tuple(gcd, y, x - (a / b) * y);


}



BigInt getModuloInverse(BigInt a, BigInt b) {
	/*
	a, m: a pair of two large integers. Suppose they are coprime
	 
	Extended Euclidean algorithm gives x, y such that ax+by = gcd(a, b)
	If gcd(a, b) = 1, then we get x with ax = 1 mod b. i.e. inverse of a modulo n.
	*/
	BigInt gcd, x, y;
	tie(gcd, x, y) = extendedEuclidean(a, b);

	//make sure input integers a, b are coprime.
	assert(gcd == 1);

	return x;
}