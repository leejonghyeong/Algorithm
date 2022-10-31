/*
RSA 암호화 알고리즘

공개키: n, e
개인키: n, d

평문: M, 암호문: C
암호화: C = M^e mod n
복호문: M = C^d mod n

키 설명:
	n = pq (RSA-2048 에서 n은 2048bit 정수)
	e = phi(n) 과 서로소인 수 (오일러 함수). 보통 65537을 쓴다고 함.
	d: ed = 1 mod phi(n) ( s.t a^ed = a mod n)


구현:
	1. 큰 소수 p, q 를 랜덤 생성
	2. n = pq 연산
	3. e=65537에 대해 조건을 만족하는 d 찾기
	4. 암호화, 복호화에 x^y mod n 연산


- 실제 암호화 과정은 암호키를 암호화시킨 후 AES같은 대칭키 암호화과정을 사용한다.
- 공개키 암호 알고리즘으로 평문을 암호화시킬 수 있지만, 대칭키 암호화 과정보다 많이 느리다.
암호화:
	1. 문자열 평문 숫자열로 변환
	2. RSA 암호화
	3. 암호화 숫자열 문자열로 변환
복호화: 반대로.


Remark:
	e = 65537 = 2^16 + 1 을 사용하는 이유
	1. 65537은 페르마 소수이다. 소수이기 때문에 e 와 phi(n)이 서로소임을 확인하기가 매우 쉽다.
	2. 암호화 과정이 M^e = M (...(M^2)^2 ... )^2 으로 쉽다.

*/

#include <iostream>
#include <vector>
#include <tuple>
#include "BigInt.h"
#include "PrimeGenerator.h"

using namespace std;

class PrivateKey {
private:
	int n_size;
	BigInt p, q;
	BigInt n, e, d;
public:
	PrivateKey(int n_size) : n_size(n_size) {
		p = generateRandomPrime(n_size);
		q = generateRandomPrime(n_size);
		n = p * q;
		e = getPublicKey();
		d = getPrivateKey(e, p, q);
	}
	
	tuple<BigInt, BigInt> get_key() { return make_tuple(n,d); }

};

class PublicKey {
private:
	int n_size;
	BigInt n, e;
public:
	PublicKey(int n_size, BigInt n) : n_size(n_size), n(n) {
		e = getPublicKey();
	}

	tuple<BigInt, BigInt> get_key() { return make_tuple(n, e); }
};



BigInt getPublicKey() {
	/*
	return Fermat Primes, 2^16 + 1
	*/
	return 65537;
}

BigInt getPrivateKey(BigInt e, BigInt p, BigInt q) {
	/*
	e: public key
	p, q: random large primes
	*/
	BigInt euler_phi_n = (p - 1) * (q - 1);
	return getModuloInverse(euler_phi_n, e);
}


string encryptionRSA(string s, PublicKey key) {
	/*
	s: number string
	key: public, private key
	*/

	BigInt M(s);
	BigInt n, e;
	tie(n, e) = key.get_key();

	//if public key is fermat prime, there is more efficient way to encrypt.
	if (e == 65537) {
		BigInt C = M;
		for (int i = 0; i < 16; i++) {
			M = M.mod_power(2, n);
		}
		C = (C * M) % n;
		return C.get_string();
	}

	return M.mod_power(e, n).get_string();
}

string decryptionRSA(string s_crypt, PrivateKey key) {
	/*
	s_crypt: crypted number string
	key: public, private key
	*/

	BigInt C(s_crypt);
	BigInt n, d;
	tie(n, d) = key.get_key();
	C =  C.mod_power(d, n);

	return C.get_string();
}


vector<int> stringToInt(vector<string>& s);
string intToString(vector<int>& nums);


int main() {

}