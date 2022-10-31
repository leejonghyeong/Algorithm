#include <iostream>

class Complex {
	double real, img;
public:
	//������ �ʱ�ȭ
	Complex(double real, double img) : real(real), img(img) {}
	Complex(const Complex& c) {
		real = c.real;
		img = c.img;
	}

	//���Ҽ� ��Ģ����
	//����: ����Ÿ���� ���۷���Ÿ�� Complex&�� �ƴϴ�.
	//����Ǵ� ���Ҽ����� ��굵�� ���� �ٲ�� �ȵǱ� ����.
	Complex operator+(const Complex& c);
	Complex operator-(const Complex& c);
	Complex operator*(const Complex& c);
	Complex operator/(const Complex& c);

	//���� ������
	Complex& operator=(const Complex& c);

	friend Complex operator+(const Complex& a, const Complex& b);

	void println() {
		std::cout << "(" << real << "," << img << ")" << std::endl;
	}
};

Complex Complex::operator+(const Complex& c) {
	Complex plus(real + c.real, img + c.img);
	return plus;
}


Complex Complex::operator-(const Complex& c) {
	Complex minus(real - c.real, img - c.img);
	return minus;
}


Complex Complex::operator*(const Complex& c) {
	Complex times(real * c.real - img * c.img, real * c.img + img + c.real);
	return times;
}


Complex Complex::operator+(const Complex& c) {
	double square_sum = c.real * c.real + c.img * c.img;
	Complex divide((real * c.real + img * c.img) / square_sum, (c.real * img - real * c.img);
	return divide;
}


Complex& Complex::operator=(const Complex& c) {
	real = c.real;
	img = c.img;
	return *this;
}
