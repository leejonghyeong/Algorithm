/*
RSA ��ȣȭ �˰���

����Ű: n, e
����Ű: n, d

��: M, ��ȣ��: C
��ȣȭ: C = M^e mod n
��ȣ��: M = C^d mod n

Ű ����:
	n = pq (RSA-2048 ���� n�� 2048bit ����)
	e = phi(n) �� ���μ��� �� (���Ϸ� �Լ�). ���� 65537�� ���ٰ� ��.
	d: ed = 1 mod phi(n) ( s.t a^ed = a mod n)


����:
	1. ū �Ҽ� p, q �� ���� ����
	2. n = pq ����
	3. e=65537�� ���� ������ �����ϴ� d ã��
	4. ��ȣȭ, ��ȣȭ�� x^y mod n ����


- ���� ��ȣȭ ������ ��ȣŰ�� ��ȣȭ��Ų �� AES���� ��ĪŰ ��ȣȭ������ ����Ѵ�.
- ����Ű ��ȣ �˰������� ���� ��ȣȭ��ų �� ������, ��ĪŰ ��ȣȭ �������� ���� ������.
��ȣȭ:
	1. ���ڿ� �� ���ڿ��� ��ȯ
	2. RSA ��ȣȭ
	3. ��ȣȭ ���ڿ� ���ڿ��� ��ȯ
��ȣȭ: �ݴ��.


Remark:
	e = 65537 = 2^16 + 1 �� ����ϴ� ����
	1. 65537�� �丣�� �Ҽ��̴�. �Ҽ��̱� ������ e �� phi(n)�� ���μ����� Ȯ���ϱⰡ �ſ� ����.
	2. ��ȣȭ ������ M^e = M (...(M^2)^2 ... )^2 ���� ����.

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