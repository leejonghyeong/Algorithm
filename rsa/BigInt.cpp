#include "BigInt.h"
#include "karatsuba.h"

bool BigInt::operator==(const BigInt& y) {
	if (big_int.compare(y.get_string()) == 0)
		return true;
	return false;
}

bool BigInt::operator>(const BigInt& y) {
	if (big_int.compare(y.get_string()) == 1) {
		return true;
	}
	return false;
}
bool BigInt::operator<(const BigInt& y) {
	if (big_int.compare(y.get_string()) == -1) {
		return true;
	}
	return false;
}


BigInt BigInt::operator+(const BigInt& y) {
	if (max(big_int.size(), y.get_string().size()) + 1 <= 18) {
		return BigInt(big_int_ll + y.get_number());
	}
	string temp = big_int;
	karatsubaAdd(temp, y.get_string());
	return BigInt(temp);
}
BigInt BigInt::operator-(const BigInt& y) {
	if (max(big_int.size(), y.get_string().size()) <= 18) {
		return BigInt(big_int_ll - y.get_number());
	}
	string temp = big_int;
	karatsubaSub(temp, y.get_string());
	return BigInt(temp);
}
BigInt BigInt::operator/(const BigInt& y) {
	return y;

}
BigInt BigInt::operator*(const BigInt& y) {
	if (big_int.size() + y.get_string().size() + 1 <= 18) {
		return BigInt(big_int_ll * y.get_number());
	}
	string product = karatsubaProduct(big_int, y.get_string());
	return BigInt(product);
}
BigInt BigInt::operator%(const BigInt& y) {
	return y;

}
//perform modular power for Big Integers
BigInt BigInt::mod_power(BigInt m, BigInt n) {
	return m;

}
