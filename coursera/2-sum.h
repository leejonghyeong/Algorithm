#pragma once
#include <list>
#include <iostream>
#include <cmath>

//----------------------------------------CHAINING----------------------------------
//int, ll 등 정수형 자료형 포함할 수 있게 한다.
template<typename T>
class ChainHash {
private:
	int BUCKET;
	int num_objects;
	std::list<T>* table; //연결리스트 포인터
public:
	//생성자 - BUCKET 사이즈만큼 포인터배열 동적할당
	ChainHash(int bucket_size) {
		BUCKET = bucket_size;
		table = new std::list<T>[BUCKET];
		num_objects = 0;
	}
	//소멸자
	~ChainHash() {
		delete[] table;
	}

	//삽입, 삭제, 탐색
	void insert(T key);
	void erase(T key);
	bool find(T key);

	//해쉬함수
	int chaining_hash_function(T key);
	int size();

};

template<typename T>
void ChainHash<T>::insert(T key) {
	if (ChainHash::find(key) == true) return;

	int index = chaining_hash_function(key);
	table[index].push_back(key);
	this->num_objects++;
}

template<typename T>
void ChainHash<T>::erase(T key) {
	int index = chaining_hash_function(key);
	
	auto it = table[index].begin();
	for (it = table[index].begin(); it != table[index].end(); it++) {
		if (*it == key) {
			table[index].erase(it);
			this->num_objects--;
			break;
		}
	}
	if (it == table[index].end())
		std::cout << "No such key value in the Hash Table: " << key << std::endl;
}

template<typename T>
bool ChainHash<T>::find(T key) {
	int index = chaining_hash_function(key);
	auto it = table[index].begin();
	for (it = table[index].begin(); it != table[index].end(); it++) {
		if (*it == key) {
			return true;
		}
	}
	return false;
}

template<typename T>
int ChainHash<T>::chaining_hash_function(T key) {
	while (key < 0) {
		key += this->BUCKET;
	}
	return key % this->BUCKET;
}

template<typename T>
int ChainHash<T>::size() {
	return this->num_objects;
}


//--------------------------OPEN ADDRESSING---------------------------------------
template<typename KEYTYPE>
struct node {
	bool is_in_use = false;
	KEYTYPE data;
};

template<typename T>
class OpenAddressingHash {
private:
	//링크드리스트 필요없음
	//
	int BUCKET;
	int num_objects;
	int prime;
	node<T>* table;
public: 
	//생성자
	OpenAddressingHash(int bucket_size);
	~OpenAddressingHash();

	//삽입, 삭제, 탐색
	void insert(T key);
	void erase(T key);
	bool find(T key);

	//해쉬함수
	//오픈 어드레싱 방법은 두개 해쉬함수 사용함.
	int hash_function(T key);
	int step_function(T key, int prime);

	//load factor
	double get_load_factor();
	int size();
};

//num미만의 랜덤소수
int find_prime(int num) {
	while (true) {
		bool find_p = true;
		int p = 2 + std::rand() % (num - 2);
		int sqrt_p = (int)std::sqrt(p);

		for (int d = 2; d <= sqrt_p; d++) {
			if (p % d == 0) {
				find_p = false;
				break;
			}
		}

		if (find_p) return p;
	}
}

//생성자
template<typename T>
OpenAddressingHash<T>::OpenAddressingHash(int bucket_size) : BUCKET(bucket_size) {
	this->prime = find_prime(BUCKET);
	this->table = new node<T>[BUCKET];
	this->num_objects = 0;
}
//소멸자
template<typename T>
OpenAddressingHash<T>::~OpenAddressingHash() {
	delete[] table;
}

template<typename T>
void OpenAddressingHash<T>::insert(T key) {
	if (OpenAddressingHash::find(key) == true)
		return;

	if (get_load_factor() > 0.75) {
		int new_bucket = BUCKET * 1.5;
		node<T>* newtable = new node<T>[new_bucket];
		std::copy(this->table, this->table + BUCKET, newtable);
		delete[] table;
		
		this->table = newtable;
		this->BUCKET = new_bucket;
		std::cout << "NEW LOAD FACTOR" << get_load_factor() << std::endl;
	}

	int index = hash_function(key);
	int step = step_function(key, this->prime);

	for (int i = 0; i < this->BUCKET; i++) {
		node<T>* current_node = &table[(index + i * step) % this->BUCKET];
		if (current_node->is_in_use == false) {
			current_node->is_in_use = true;
			current_node->data = key;
			this->num_objects++;
			break;
		}
	}
}

template<typename T>
void OpenAddressingHash<T>::erase(T key) {
	int index = hash_function(key);
	int step = step_function(key, this->prime);

	for (int i = 0; i < this->BUCKET; i++) {
		node<T>* current_node = &table[(index + i * step) % this->BUCKET];
		if (current_node->is_in_use == true && current_node->data == key) {
			current_node->is_in_use = false;
			this->num_objects--;
			break;
		}
	}
}

template<typename T>
bool OpenAddressingHash<T>::find(T key) {
	int index = hash_function(key);
	int step = step_function(key, this->prime);

	for (int i = 0; i < this->BUCKET; i++) {
		node<T> current_node = table[(index + i * step) % this->BUCKET];
		if (current_node.is_in_use == true && current_node.data == key) {
			return true;
		}
	}
	return false;
}

template<typename T>
int OpenAddressingHash<T>::hash_function(T key) {
	while (key < 0) {
		key += this->BUCKET;
	}
	return key % this->BUCKET;
}

template<typename T>
int OpenAddressingHash<T>::step_function(T key, int prime) {
	while (key < 0) {
		key += 100*this->prime;
	}
	return 1 + key % this->prime;
}

template<typename T>
double OpenAddressingHash<T>::get_load_factor() {
	return (double)this->num_objects / this->BUCKET;
}

template<typename T>
int OpenAddressingHash<T>::size() {
	return this->num_objects;
}