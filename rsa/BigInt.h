#pragma once
#include <string>
using namespace std;

//BigInt Class
class BigInt {
private:
	string big_int;
	long long big_int_ll;

public:
	BigInt(){}
	BigInt(const BigInt& x) {
		big_int = x.big_int;
		if (x.big_int_ll) big_int_ll = x.big_int_ll;
	}
	BigInt(int num) {
		big_int_ll = num;

		while (num > 0) {
			big_int = (char)(num % 10 + 48) + big_int;
			num = num / 10;
		}
	}
	BigInt(string s) : big_int(s) {};
	~BigInt(){}

	//Operators
	bool operator==(const BigInt& y);
	bool operator>(const BigInt& y);
	bool operator<(const BigInt& y);
	BigInt operator+(const BigInt& y);
	BigInt operator-(const BigInt& y);
	BigInt operator/(const BigInt& y);
	BigInt operator*(const BigInt& y);
	BigInt operator%(const BigInt& y);
	BigInt mod_power(BigInt m, BigInt n); //compute modular power for Big Integers
 
	//access big_int data
	string get_string() const { return big_int; }
	long long get_number() const { return big_int_ll; }
	//determine if big_int can be expressed in terms of long long data type
	bool is_big() const { if (big_int.size() > 18) return true; return false; }
	//print big_int data
	void print_big_int() const { std::cout << big_int << endl; }
};

//Test BigInt class
int main() {
	BigInt a(1000000), b(22200000);

	(a + b).print_big_int();
	(a - b).print_big_int();
	(b / a).print_big_int();
	(a * b).print_big_int();

	return 0;
}