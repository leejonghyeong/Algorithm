#include <iostream>
#include <vector>
#include <string>

using namespace std;

//int to vector<int>
vector<int> intToVec(int num) {
	vector<int> vec;
	while (num > 0) {
		vec.push_back(num % 10);
		num = num / 10;
	}

	return vec;
}

vector<int> llToVec(long long num) {
	vector<int> vec;
	while (num > 0) {
		vec.push_back(num % 10);
		num = num / 10;
	}

	return vec;
}


vector<int> stringToVec(string s) {
	vector<int> vec;
	int s_size = s.size();
	for (int i = 0; i < s_size; i++) {
		vec.push_back(s[s_size - i - 1] - '0');
	}
	return vec;
}

string intToString(int num) {
	string s;
	if (num < 0)
		return "-" + intToString(-num);
	else if (num == 0)
		return "0";

	while (num > 0) {
		s = (char)(num % 10 + 48) + s ;
		num = num / 10;
	}

	return s;
}

string vecToString(vector<int> vec) {
	string s;
	int size = vec.size();
	for (int i = 0; i < size; i++) {
		s.push_back((char)(vec[size - i - 1] + 48));
	}
	return s;
}


// 덧셈/뺄셈에 대한 num 자릿수 올림/내림을 수행
// 앞자리 0 제거
void regroup(vector<int>& num) {
	num.push_back(0);
	int size = num.size();

	for (int i = 0; i < size - 1; i++) {
		if (num[i] >= 0) {
			num[i + 1] += num[i] / 10;
			num[i] = num[i] % 10;
		}
		else {
			int borrow = (abs(num[i]) + 9) / 10;
			num[i + 1] -= borrow;
			num[i] += borrow * 10;
		}
	}

	while (num.size() > 1 && num.back() == 0)
		num.pop_back();
}

vector<int> product(const vector<int>& a, const vector<int>& b) {
	int size1 = a.size();
	int size2 = b.size();
	vector<int> c(size1 + size2, 0);

	for (int i = 0; i < size1; i++) {
		for (int j = 0; j < size2; j++) {
			c[i + j] += a[i] * b[j];
		}
	}

	regroup(c);
	return c;
}

// a += b * 10^k
void karatsubaAdd(vector<int>& a, const vector<int>& b, int k = 0) {
	int max_size = max(a.size(), b.size() + k);
	if (a.size() < max_size)
		a.resize(max_size, 0);

	for (int i = 0; i < b.size(); i++) {
		a[i + k] += b[i];
	}

	regroup(a);
}

// a -= b
// a >= b 가정
void karatsubaSub(vector<int>& a, const vector<int>& b) {
	for (int i = 0; i < b.size(); i++) {
		a[i] -= b[i];
	}

	regroup(a);
}

// karatsuba product
// a = a1 + a2 * 10^n
// b = b1 + b2 * 10^n
// c = a1 * b1 + (a2 * b1 + a1 * b2) * 10^(n/2) + a2 * b2 * 10^n
vector<int> karatsubaProduct(const vector<int>& a, const vector<int>& b) {
	int size1 = a.size();
	int size2 = b.size();

	if (size1 < size2)
		return karatsubaProduct(b, a);
	if (size1 == 0 || size2 == 0)
		return vector<int>();
	if (size1 <= 8) {
		string a_str = vecToString(a);
		string b_str = vecToString(b);
		return llToVec(stoll(a_str) * stoll(b_str));
	}
	int n = size1;

	vector<int> a1(a.begin(), a.begin() + n/2);
	vector<int> a2(a.begin() + n/2, a.end());
	vector<int> b1(b.begin(), b.begin() + min<int>(b.size(), n / 2));
	vector<int> b2(b.begin() + min<int>(b.size(), n / 2), b.end());

	//c1 = a1 * b1
	vector<int> c1 = karatsubaProduct(a1, b1);
	//c2 = a2 * b2
	vector<int> c2 = karatsubaProduct(a2, b2);
	//c3 = (a1+a2)*(b1+b2) - c1 - c2
	karatsubaAdd(a1, a2);
	karatsubaAdd(b1, b2);
	vector<int> c3 = karatsubaProduct(a1, b1);
	karatsubaSub(c3, c1);
	karatsubaSub(c3, c2);

	// c = c1 + c3 * 10^(n/2) + c2 * 10^(n/2 * 2)
	vector<int> c(c2.size() + n, 0);
	karatsubaAdd(c, c1, 0);
	karatsubaAdd(c, c3, n / 2);
	karatsubaAdd(c, c2, (n / 2) * 2);

	return c;
	
}

//string 혹은 integer를 인수로 받는 경우에도 정의

//compare two strings which indicate two positive integers
//return true if a >= b return false else
void karatsubaAdd(string& a, const string& b);

bool compareString(const string& a, const string& b) {
	if (a.size() > b.size()) return true;
	else if (b.size() > a.size()) return false;
	int n = a.size();
	for (int i = 0; i < n; i++) {
		if ((a[i] - '0') > (b[i] - '0'))
			return true;
		else if ((a[i] - '0') < (b[i] - '0'))
			return false;
	}

	return true;
}

void karatsubaSub(string& a, const string& b) {

	if (a[0] == '-' && b[0] != '-') {
		a = a.substr(1);
		karatsubaAdd(a, b);
		a = "-" + a;
		return;
	}
	else if (a[0] != '-' && b[0] == '-') {
		karatsubaAdd(a, b.substr(1));
		return;
	}
	else if (a[0] == '-' && b[0] == '-') {
		a = a.substr(1);
		karatsubaSub(a, b.substr(1));
		if (a[0] == '-')
			a = a.substr(1);
		else
			a = "-" + a;
		return;
	}
	

	//Case(a < b)
	if (compareString(a, b) == false) {
		string temp = b;
		karatsubaSub(temp, a);
		a = "-" + temp;
		return;
	}


	//Case(a > b)
	vector<int> a_vec = stringToVec(a);
	vector<int> b_vec = stringToVec(b);
	
	karatsubaSub(a_vec, b_vec);

	a = vecToString(a_vec);

}

void karatsubaAdd(string& a, const string& b) {
	//the case when a or b is negative
	if (a[0] == '-' && b[0] != '-') {
		a = a.substr(1);
		karatsubaSub(a, b);
		if (a[0] == '-')
			a = a.substr(1);
		else
			a = "-" + a;
		return;
	}
	else if (a[0] != '-' && b[0] == '-') {
		karatsubaSub(a, b.substr(1));
		return;
	}
	else if (a[0] == '-' && b[0] == '-') {
		a = a.substr(1);
		string temp = b.substr(1);
		karatsubaAdd(a, temp);
		a = "-" + a;
		return;
	}

	vector<int> a_vec = stringToVec(a);
	vector<int> b_vec = stringToVec(b);

	karatsubaAdd(a_vec, b_vec);

	a = vecToString(a_vec);
}

string karatsubaProduct(const string& a, const string& b) {

	if (a[0] == '-') {
		return "-" + karatsubaProduct(a.substr(1), b);
	}
	else if (b[0] == '-') {
		return "-" + karatsubaProduct(a, b.substr(1));
	}

	vector<int> a_vec = stringToVec(a);
	vector<int> b_vec = stringToVec(b);

	return vecToString(karatsubaProduct(a_vec, b_vec));
}

/*
int main() {
	string string_a = "3141592653589793238462643383279502884197169399375105820974944592";
	string string_b = "2718281828459045235360287471352662497757247093699959574966967627";

	vector<char> a_char(string_a.begin(), string_a.end());
	vector<char> b_char(string_b.begin(), string_b.end());
	int size_a = a_char.size();
	int size_b = b_char.size();

	vector<int> a(size_a);
	vector<int> b(size_b);

	for (int i = 0; i < size_a ; i++) {
		a[i] = a_char[size_a - 1 - i] - '0';
	}
	for (int i = 0; i < size_b ; i++) {
		b[i] = b_char[size_b - 1 - i] - '0';
	}


	vector<int> c = karatsuba_product(a, b);
	int size_c = c.size();
	// karatsuba product
	for (int i = size_c-1; i >=0; i--) {
		std::cout << c[i];
	}
	std::cout << endl;

	// normal product
	vector<int> d = product(a, b);
	int size_d = d.size();
	for (int i = size_d-1; i >=0; i--) {
		std::cout << d[i];
	}

	return 0;
}
*/
