#include <iostream>

//note c++에서 구조체는 모든 멤버함수, 변수가 public인 클래스

namespace MyArray {

	class Array {
		friend Int; // Int클래스 선언에서 멤버변수 사용

		const int dim;
		int* size; // 각 차원에 해당되는 배열의 길이

		struct Address {
			//level dim -1 은 데이터 배열을 가르킴
			//상위 level은 다음 Address 배열을 가르킴
			int level;
			void* next;
		};

		Address* top;

	public:
		Array(int dim, int* array_size) : dim(dim) {
			size = new int[dim];
			for (int i = 0; i < size; i++)
				size[i] = array_size[i];

			//top Address 및 하위 Address 초기화
			top = new Address;
			top->level = 0;
			initialize_address(top);
		}
		Array(const Array& arr) : dim(arr.dim) {
			size = new int[dim];
			for (int i = 0; i < size; i++)
				size[i] = arr.size[i];

			top = new Address;
			top->level = 0;
			initialize_address(top);
		}
		~Array() {
			delete_address(top);
			delete[] size;
		}

		void initialize_address(Address* current) {
			if (!current) return; // 초기화할 배열이 없는 경우
			if (current->level == dim - 1) {
				current->next = new int[size[current->level]]; // 마지막 레벨에 데이터배열로 초기화
			}
			// 현 레벨 Address 배열 초기화
			current->next = new Address[size[current->level]];
			// 각 배열원소마다 재귀적으로 level+1의 Address배열 초기화
			for (int i = 0; i < size[current->level]; i++) {
				(static_cast<Address*>(current->next) + i)->level = current->level + 1;
				initialize_address(static_cast<Address*>(current->next) + i);
			}

		}

		void delete_address(Address* current) {
			if (!current) return;
			if (current->level == dim - 1) {
				delete[] current->next;
			}
			//address배열이 가르키는 다음 address배열들을 지운다.
			for (int i = 0; i < size[current->level]; i++) {
				delete_address(static_cast<Address*>(current->next) + i);
			}
			//address배열이 가르키는 포인터도 지워야함.
			delete[] current->next;
		}

	};


	/*
	다차원 배열을 arr[i][j][k]...[l] 로 접근하게 operand[]를 정의하자!
	이 함수의 리턴타입은 Array& 이다가 마지막에는 Int&이길 바란다.

	*/

	//Int의 Wrapper class
	//arr[i][j]..[k]의 리턴 타입
	class Int {
		void* data;
		int level;
		Array* array;
	public:
		Int(int index, int _level = 0, void* _data = NULL, Array* _array = NULL) : _level(level), _data(data), array(_array) {
			//level 0: arr 그자체, 반환할 내용이 없음
			if (_level < 0 || index >= _array->size[_level - 1]) {
				data = NULL;
				return;
			}
			if (_level == _array->dim) {
				data = static_cast<void*>(static_cast<int*>(static_cast<Address*>(data)->next) + index));
			}
			else {
				data = static_cast<void*>(static_cast<Address*>(static_cast<Address*>(data)->next) + index); //note: data도 next도 void*타입이므로 캐스팅해준다.
			}
		}
		//복사생성자
		Int(const Int& i) : data(i.data), level(i.level), array(i.array) {};

		//타입 변환 연산자!
		operator int() {
			if (data)
				return *static_cast<int*>(data);
			return 0;
		}

		//대입 연산자
		Int& operator=(const Int& a) {
			if (data)
				*static_cast<int*>(data) = a;
			return *this;
		}

		//operator[]
		Int operator[](const int index) {
			if (!data) return 0;
			return Int(index, level + 1, data, array);
		}

	};

	//array[index]
	Int Array::operator[](const Int index) {
		return Int(index, 1, static_cast<void*>(top), this);
	}
} // namespace MyArray


int main() {
	int size[] = { 2,3,4 };
	MyArray::Array arr[3, size];

	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 3; j++) {
			for (int k = 0; k < 4; k++) {
				arr[i][j][k] = (i + 1) * (j + 1) * (k + 1);
			}
		}
	}

	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 3; j++) {
			for (int k = 0; k < 4; k++) {
				std::cout << i << " " << j << " " << k << " " << arr[i][j][k] << std::endl;
			}
		}
	}
}