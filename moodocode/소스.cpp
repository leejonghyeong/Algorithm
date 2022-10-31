#include <iostream>
#include <vector>

/*
멤버변수:
	1. 문자열 데이터 포인터 (가변길이를 가지기 위해)
	2. 문자열 데이터 길이
*/
class MyString {
	char* string_content;
	int string_length;
	int memory_capacity;

public:
	//생성자-문자 하나로 생성
	MyString(char c);
	//문자열로 생성
	MyString(const char* str);
	//복사 생성자
	MyString(const MyString& str);
	//소멸자
	~MyString();

	int length() const;
	void print() const;
	void println() const;

	MyString& assign(const MyString& str);
	MyString& assign(const char* str);

	//할당된 메모리 크키 확인
	int capacity();
	//할당할 문자열 크기 예약
	void reserve(int size);
	
	//특정 위치 문자 반환
	char at(int i) const;

	//문자열 삽입
	MyString& insert(int loc, const MyString& str);
	MyString& insert(int loc, const char* str);
	MyString& insert(int loc, char c);

	//문자열 제거
	MyString& erase(int loc, int num);

	//문자열 찾기
	int find(int find_from, MyString& str) const;
	int find(int find_from, const char* str) const;
	int find(int find_from, char c) const;

	//문자열 비교
	int compare(const MyString& str) const;

	//연산자 오버로딩
	bool operator==(MyString& str);
};

MyString::MyString(char c) {
	string_content = new char[1];
	string_content[0] = c;
	string_length = 1;
}

MyString::MyString(const char* str) {
	string_length = strlen(str);
	string_content = new char[string_length];
	
	for (int i = 0; i < string_length; i++) string_content[i] = str[i];
}

MyString::MyString(const MyString& str) {
	string_length = str.string_length;
	for (int i = 0; i < string_length; i++) string_content[i] = str.string_content[i];
}
MyString::~MyString() { delete[] string_content; }

int MyString::length() const {
	return string_length;
}
void MyString::print() const {
	for (int i = 0; i < string_length; i++)
		std::cout << string_content[i];
}
void MyString::println() const {
	for (int i = 0; i < string_length; i++)
		std::cout << string_content[i];
	std::cout << std::endl;
}

MyString& MyString::assign(const MyString& str) {
	if (str.string_length > memory_capacity) {
		delete[] string_content;
		string_content = new char[str.string_length];
		memory_capacity = str.string_length;
	}
	for (int i = 0; i < str.string_length; i++)
		string_content[i] = str.string_content[i];
	
	string_length = str.string_length;
	return *this;
}

MyString& MyString::assign(const char* str) {
	int new_string_length = strlen(str);
	if (new_string_length > memory_capacity) {
		delete[] string_content;
		string_content = new char[new_string_length];
		memory_capacity = new_string_length;
	}
	for (int i = 0; i < new_string_length; i++)
		string_content[i] = str[i];

	string_length = new_string_length;
	return *this;
}

int MyString::capacity() { return memory_capacity; }

void MyString::reserve(int size) {
	//memory capacity보다 큰 메모리를 할당할 경우에만
	//새로 동적할당한 후 문자열 복사
	if (size > memory_capacity) {
		char* prev_string_content = string_content;
		string_content = new char[size];
		memory_capacity = size;

		for (int i = 0; i < string_length; i++)
			string_content[i] = prev_string_content[i];

		//기존 문자열포인터 메모리해제
		delete[] prev_string_content;
	}
}

char MyString::at(int i) const {
	if (i < 0 || i >= string_length) return NULL;
	
	return string_content[i];
}

MyString& MyString::insert(int loc, const MyString& str) {
	if (loc <0 || loc > string_length)
		return *this;

	if (string_length + str.string_length > memory_capacity) {
		char* prev_string_content = string_content;

		//필요한만큼 메모리 할당
		//insert작업이 잦은 경우, 빈번한 메모리할당은 비효율적이므로 보통
		//memeory_capacity *= 2 로 확장한다.
		memory_capacity = string_length + str.string_length;

		string_content = new char[memory_capacity];
		for (int i = 0; i < memory_capacity; i++) {
			if (i < loc)	string_content[i] = prev_string_content[i];
			else if (i < loc + str.string_length) string_content[i] = str.string_content[i - loc];
			else string_content[i] = prev_string_content[i - str.string_length];
		}
		delete[] prev_string_content;
		string_length = string_length + str.string_length;
		return *this;
	}

	//새로 동적할당할 필요가 없는 경우
	//기존 문자열을 미리 전처리하여 기존 문자열을 복사하지 않고 삽입할 수 있도록 한다.
	for (int i = string_length; i >= loc; i--)
		string_content[i + str.string_length] = string_content[i];

	for (int i = 0; i < str.string_length; i++)
		string_content[loc + i] = str.string_content[i];
	
	string_length = string_length + str.string_length;
	return *this;

}
MyString& MyString::insert(int loc, const char* str) {
	MyString mystring(str);
	return insert(loc, mystring);
}
MyString& MyString::insert(int loc, char c) {
	MyString mystring(c);
	return insert(loc, mystring);
}

MyString& MyString::erase(int loc, int num) {
	if (loc < 0 || loc >= string_length) return *this;

	if (loc + num >= string_length)
		num = string_length - loc;

	for (int i = loc+num; i < string_length; i++)
		string_content[i-num] = string_content[i];

	string_length = string_length - num;
	return *this;
}

//N길이 문자열에서 M길이 부분문자열을 찾는 알고리즘
//기존 brute force 알고리즘 O(NM)
//KMP 알고리즘을 사용하면 O(N+M)

std::vector<int> get_pi(MyString& str) {
	int m = str.length();
	int j = 0;
	std::vector<int> pi(m, 0);
	//0,1,2...,m 의 길이에 따라
	//0~i까지 부분수열이 prefix와 surfix가 얼마나 겹치는지 확인
	for (int i = 1; i < m; i++) {
		//0~i까지 부분수열과 0~j까지 부분수열(prefix)을 생각하자. (j<i)
		//str[i]와 str[j]가 다른 경우, 원래라면 prefix를 한 칸씩 이동하여 surfix와 prefix가 같은 최대길이를 비교
		//pi[j-1] 을 이용하면 중간단계 스킵가능
		while (j > 0 && str.at(i) != str.at(j)) {
			j = pi[j - 1];
		}
		//str[i]와 str[j]가 같은 경우, 한 글자 추가된 prefix와 surfix가 일치하므로 pi[i]=j+1
		if (str.at(i) == str.at(j))
			pi[i] = ++j;
	}
	return pi;
}

int MyString::find(int find_from, MyString& str) const {
	/*
	int i, j;
	if (str.string_length == 0) return -1;
	for (i = find_from; i < string_length - str.string_length; i++) {
		for (j = 0; j < str.string_length; j++) {
			if (string_content[i + j] != str.string_content[j]) break;
		}
		if (j == str.string_length) return i;
	}
	return -1;
	*/
	std::vector<int> pi = get_pi(str);
	int j = 0;
	if (str.string_length == 0) return -1;
	for (int i = find_from; i < string_length; i++) {
		while (j > 0 && string_content[i] != str.at(j)) {
			j = pi[j - 1];
		}
		if (string_content[i] == str.at(j)) {
			//주어진 문자열을 찾은 경우 위치 반환
			if (j == str.string_length - 1) return i - str.string_length + 1;
			else j++;
		}
	}


}
int MyString::find(int find_from, const char* str) const {
	MyString mystring(str);

	return find(find_from, mystring);
}
int MyString::find(int find_from, char c) const {
	MyString mystring(c);

	return find(find_from, mystring);
}

//문자열 비교
int MyString::compare(const MyString& str) const {
	//(*this)와 str를 사전식으로 비교한다.
	//(*this)가 더 뒤에 올 경우, 1을 반환. 같은 경우 0, 더 앞에 오는 경우 -1을 반환.

	for (int i = 0; i < std::min(string_length, str.string_length); i++) {
		if (string_content[i] > str.string_content[i]) return 1;
		else if (string_content[i] < str.string_content[i]) return -1;
	}

	if (string_length == str.string_length) return 0;

	else if (string_length > str.string_length) return 1;
	
	return -1;
}

int main() {
	MyString str1("this is a very very long string");
	std::cout << "Location of first <very> in the string : " << str1.find(0, "very") << std::endl;
	std::cout << "Location of second <very> in the string : " << str1.find((str1.find(0,"very")+1), "very") << std::endl;

}

bool MyString::operator==(MyString& str) {
	return !compare(str);
}