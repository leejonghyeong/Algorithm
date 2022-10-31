#include <iostream>

class Complex {
	double real, img;
public:
	//생성자 초기화
	Complex(double real, double img) : real(real), img(img) {}
	Complex(const Complex& c) {
		real = c.real;
		img = c.img;
	}

	//복소수 사칙연산
	//주의: 리턴타입이 레퍼런스타입 Complex&가 아니다.
	//연산되는 복소수들이 계산도중 값이 바뀌면 안되기 때문.
	Complex operator+(const Complex& c);
	Complex operator-(const Complex& c);
	Complex operator*(const Complex& c);
	Complex operator/(const Complex& c);

	//대입 연산자
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
