#include <iostream>
#include <vector>

/*
�������:
	1. ���ڿ� ������ ������ (�������̸� ������ ����)
	2. ���ڿ� ������ ����
*/
class MyString {
	char* string_content;
	int string_length;
	int memory_capacity;

public:
	//������-���� �ϳ��� ����
	MyString(char c);
	//���ڿ��� ����
	MyString(const char* str);
	//���� ������
	MyString(const MyString& str);
	//�Ҹ���
	~MyString();

	int length() const;
	void print() const;
	void println() const;

	MyString& assign(const MyString& str);
	MyString& assign(const char* str);

	//�Ҵ�� �޸� ũŰ Ȯ��
	int capacity();
	//�Ҵ��� ���ڿ� ũ�� ����
	void reserve(int size);
	
	//Ư�� ��ġ ���� ��ȯ
	char at(int i) const;

	//���ڿ� ����
	MyString& insert(int loc, const MyString& str);
	MyString& insert(int loc, const char* str);
	MyString& insert(int loc, char c);

	//���ڿ� ����
	MyString& erase(int loc, int num);

	//���ڿ� ã��
	int find(int find_from, MyString& str) const;
	int find(int find_from, const char* str) const;
	int find(int find_from, char c) const;

	//���ڿ� ��
	int compare(const MyString& str) const;

	//������ �����ε�
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
	//memory capacity���� ū �޸𸮸� �Ҵ��� ��쿡��
	//���� �����Ҵ��� �� ���ڿ� ����
	if (size > memory_capacity) {
		char* prev_string_content = string_content;
		string_content = new char[size];
		memory_capacity = size;

		for (int i = 0; i < string_length; i++)
			string_content[i] = prev_string_content[i];

		//���� ���ڿ������� �޸�����
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

		//�ʿ��Ѹ�ŭ �޸� �Ҵ�
		//insert�۾��� ���� ���, ����� �޸��Ҵ��� ��ȿ�����̹Ƿ� ����
		//memeory_capacity *= 2 �� Ȯ���Ѵ�.
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

	//���� �����Ҵ��� �ʿ䰡 ���� ���
	//���� ���ڿ��� �̸� ��ó���Ͽ� ���� ���ڿ��� �������� �ʰ� ������ �� �ֵ��� �Ѵ�.
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

//N���� ���ڿ����� M���� �κй��ڿ��� ã�� �˰���
//���� brute force �˰��� O(NM)
//KMP �˰����� ����ϸ� O(N+M)

std::vector<int> get_pi(MyString& str) {
	int m = str.length();
	int j = 0;
	std::vector<int> pi(m, 0);
	//0,1,2...,m �� ���̿� ����
	//0~i���� �κм����� prefix�� surfix�� �󸶳� ��ġ���� Ȯ��
	for (int i = 1; i < m; i++) {
		//0~i���� �κм����� 0~j���� �κм���(prefix)�� ��������. (j<i)
		//str[i]�� str[j]�� �ٸ� ���, ������� prefix�� �� ĭ�� �̵��Ͽ� surfix�� prefix�� ���� �ִ���̸� ��
		//pi[j-1] �� �̿��ϸ� �߰��ܰ� ��ŵ����
		while (j > 0 && str.at(i) != str.at(j)) {
			j = pi[j - 1];
		}
		//str[i]�� str[j]�� ���� ���, �� ���� �߰��� prefix�� surfix�� ��ġ�ϹǷ� pi[i]=j+1
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
			//�־��� ���ڿ��� ã�� ��� ��ġ ��ȯ
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

//���ڿ� ��
int MyString::compare(const MyString& str) const {
	//(*this)�� str�� ���������� ���Ѵ�.
	//(*this)�� �� �ڿ� �� ���, 1�� ��ȯ. ���� ��� 0, �� �տ� ���� ��� -1�� ��ȯ.

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