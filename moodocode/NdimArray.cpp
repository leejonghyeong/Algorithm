#include <iostream>

//note c++���� ����ü�� ��� ����Լ�, ������ public�� Ŭ����

namespace MyArray {

	class Array {
		friend Int; // IntŬ���� ���𿡼� ������� ���

		const int dim;
		int* size; // �� ������ �ش�Ǵ� �迭�� ����

		struct Address {
			//level dim -1 �� ������ �迭�� ����Ŵ
			//���� level�� ���� Address �迭�� ����Ŵ
			int level;
			void* next;
		};

		Address* top;

	public:
		Array(int dim, int* array_size) : dim(dim) {
			size = new int[dim];
			for (int i = 0; i < size; i++)
				size[i] = array_size[i];

			//top Address �� ���� Address �ʱ�ȭ
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
			if (!current) return; // �ʱ�ȭ�� �迭�� ���� ���
			if (current->level == dim - 1) {
				current->next = new int[size[current->level]]; // ������ ������ �����͹迭�� �ʱ�ȭ
			}
			// �� ���� Address �迭 �ʱ�ȭ
			current->next = new Address[size[current->level]];
			// �� �迭���Ҹ��� ��������� level+1�� Address�迭 �ʱ�ȭ
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
			//address�迭�� ����Ű�� ���� address�迭���� �����.
			for (int i = 0; i < size[current->level]; i++) {
				delete_address(static_cast<Address*>(current->next) + i);
			}
			//address�迭�� ����Ű�� �����͵� ��������.
			delete[] current->next;
		}

	};


	/*
	������ �迭�� arr[i][j][k]...[l] �� �����ϰ� operand[]�� ��������!
	�� �Լ��� ����Ÿ���� Array& �̴ٰ� ���������� Int&�̱� �ٶ���.

	*/

	//Int�� Wrapper class
	//arr[i][j]..[k]�� ���� Ÿ��
	class Int {
		void* data;
		int level;
		Array* array;
	public:
		Int(int index, int _level = 0, void* _data = NULL, Array* _array = NULL) : _level(level), _data(data), array(_array) {
			//level 0: arr ����ü, ��ȯ�� ������ ����
			if (_level < 0 || index >= _array->size[_level - 1]) {
				data = NULL;
				return;
			}
			if (_level == _array->dim) {
				data = static_cast<void*>(static_cast<int*>(static_cast<Address*>(data)->next) + index));
			}
			else {
				data = static_cast<void*>(static_cast<Address*>(static_cast<Address*>(data)->next) + index); //note: data�� next�� void*Ÿ���̹Ƿ� ĳ�������ش�.
			}
		}
		//���������
		Int(const Int& i) : data(i.data), level(i.level), array(i.array) {};

		//Ÿ�� ��ȯ ������!
		operator int() {
			if (data)
				return *static_cast<int*>(data);
			return 0;
		}

		//���� ������
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