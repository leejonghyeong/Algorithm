#include <ostream>
#include <fstream>
#include <iostream>
#include <vector>
#include <stack>
#include <string>
#include <stack>

//vector와 stack은 따로 구현할 것
//Question: 왜 Cell과 Table 객체는 복사생성자를 왜 정의안하는가?

using namespace std;

namespace MyExcel {
	class Table;

	/*
	class Cell {
	protected:
		Table* table; // 어느 테이블에 속해있는지
		int x, y; //테이블에서 위치
		string data;

	public:
		virtual string stringify() { return data; } // private 변수에 접근하기 위한 멤버함수
		virtual int to_numeric() { return 0; }		// 

		Cell(string data, int x, int y, Table* table) : data(data), x(x), y(y), table(table) {}
		

	};*/

	//문자열 이외에도 여러 종류 데이터를 가지도록 Cell 인터페이스 정의
	class Cell {
	protected:
		int x, y;
		Table* table;
		
	public:
		virtual string stringify() = 0;
		virtual int to_numeric() = 0;

		Cell(int x, int y, Table* table);
	};

	class Table {
	protected:
		int max_row_size, max_col_size;

		Cell*** data_table; //3중 포인터
	public:
		Table(int max_row_size, int max_col_size);
		~Table();


		void reg_cell(Cell* c, int row, int col); // cell 등록	

		int to_numeric(const string& s); 	//셀이름(eg: A3, B6)으로 셀의 정수값 반환
		int to_numeric(int x, int y);  //행, 열 번호로 셀의 정수값 반환

		string stringify(const string& s); // 해당 위치의 Cell 데이터를 얻는다.
		string stringify(int row, int col);

		virtual string print_table() = 0; // overriding할 함수

	};

	Table::Table(int max_row_size, int max_col_size) : max_row_size(max_row_size), max_col_size(max_col_size) {
		data_table = new Cell * *[max_row_size];
		for (int i = 0; i < max_row_size; i++) {
			data_table[i] = new Cell * [max_col_size];
		}
		//NULL포인터로 초기화
		for (int i = 0; i < max_row_size; i++) {
			for (int j = 0; j < max_col_size; j++) {
				data_table[i][j] = NULL;
			}
		}
	}

	Table::~Table() {
		for (int i = 0; i < max_row_size; i++) {
			for (int j = 0; j < max_col_size; j++) {
				if (data_table[i][j]) delete data_table[i][j];
			}
		}

		for (int i = 0; i < max_row_size; i++) {
			delete[] data_table[i];
		}
		delete[] data_table;
	}

	void Table::reg_cell(Cell* c, int row, int col) {
		if (row >= max_row_size || col >= max_col_size) return;

		//이미 등록되어있는 경우 삭제 후 등록
		if (data_table[row][col]) delete data_table[row][col];

		data_table[row][col] = c;
	}

	int Table::to_numeric(const string& s) {
		//A3 -> (0,2), D6 -> (3,5)
		int row = s[0] - 'A';
		int col = atoi(s.c_str() + 1) - 1;

		if (row < max_row_size && col < max_col_size)
			if (data_table[row][col])
				return data_table[row][col]->to_numeric();

		return 0;
	}

	int Table::to_numeric(int row, int col) {
		if (row < max_row_size && col < max_col_size)
			if (data_table[row][col])
				return data_table[row][col]->to_numeric();

		return 0;
	}

	string Table::stringify(const string& s) {
		int row = s[0] - 'A';
		int col = atoi(s.c_str() + 1) - 1;

		if (row < max_row_size && col < max_col_size) {
			if (data_table[row][col]) {
				return data_table[row][col]->stringify();
			}
		}
		return 0;
	}

	string Table::stringify(int row, int col) {

		if (row < max_row_size && col < max_col_size) {
			if (data_table[row][col]) {
				return data_table[row][col]->stringify();
			}
		}
		return 0;
	}

	std::ostream& operator<<(std::ostream& o, Table& table) {
		o << table.print_table();
		return o;
	}


	/*
	Table class 를 상속받는 TextTable 구현

	*/
	class TextTable : public Table {
		string repeat_char(int n, char c);

		string col_num_to_str(int n);

	public:
		TextTable(int row, int col) : Table(row, col) {}

		string print_table();
	};

	string TextTable::repeat_char(int n, char c) {
		string s = "";
		for (int i = 0; i < n; i++) {
			s.push_back(c);
		}
		return s;
	}

	string TextTable::col_num_to_str(int n) {
		string s = "";
		if (n < 26) {
			s.push_back('A' + n);
		}
		else {
			s.push_back('A' + n / 26 - 1);
			s.push_back('A' + n % 26);
		}
		return s;
	}

	string TextTable::print_table() {
		//출력할 테이블 문자열
		string total_table;
		
		//각 열마다 가장 긴 문자열의 길이를 저장. 
		int* col_max_wide = new int[max_col_size];

		for (int i = 0; i < max_col_size; i++) {
			unsigned int max_wide = 2;
			for (int j = 0; j < max_row_size; j++) {
				if (data_table[j][i] && data_table[j][i]->stringify().length() > max_wide) {
					max_wide = data_table[j][i]->stringify().length();
				}
			}
			col_max_wide[i] = max_wide;
		}

		//테이블 초기화; 테이블 좌측상단
		total_table += "    ";
		int total_wide = 4;
		//테이블 첫 번째 줄 작성; 열 이름 A,B,...,AA,AB,...
		for (int i = 0; i < max_col_size; i++) {
			if (col_max_wide[i]) {
				int max_len = max(2, col_max_wide[i]);
				total_table += " | " + col_num_to_str(i);
				total_table += repeat_char(max_len - col_num_to_str(i).length(), ' ');

				total_wide += (max_len + 3);
			}
		}

		total_table += "\n";

		for (int i = 0; i < max_row_size; i++) {
			//가로 구분선
			total_table += repeat_char(total_wide, '-');
			total_table += "\n";
			//행 번호
			total_table += to_string(i + 1) + repeat_char(4 - to_string(i + 1).length(), ' ');
			
			//각 셀마다 데이터 문자열 입력
			for (int j = 0; j < max_col_size; j++) {
				string s = "";

				if (data_table[i][j]) {
					s = data_table[i][j]->stringify();

				}
				else {
					s = repeat_char(max(2, col_max_wide[j]), ' ');
				}
				total_table += " | " + s;
				total_table += repeat_char(col_max_wide[j] - s.length(), ' ');
			}
			total_table += "\n";
		}

		return total_table;

	}

	class CSVTable : public Table {};
	class HTMLTable : public Table {};

	/*
	데이터 종류별 Cell 피상속클래스 선언
	string, int, time_t 데이터 타입.
	*/

	class StringCell :public Cell {
		string data;

	public:
		string stringify();
		int to_numeric();

		StringCell(string data, int x, int y, Table* table);
	};

	class NumberCell : public Cell {
		int data;

	public:
		string stringify();
		int to_numeric();

		NumberCell(int data, int x, int y, Table* table);

	};

	class DateCell : public Cell {
		// yyyy-mm-dd 형식의 입력만 받는다고 가정.
		// time_t 자료형은 1970년부터 현재까지 흐른 시간을 초단위로 저장한 정수형 변수.
		time_t data;

	public:
		string stringify();
		int to_numeric();

		DateCell(string data, int x, int y, Table* table);
	};


	/*
	Cell 피상속 클래스 멤버함수 선언
	*/
	
	string StringCell::stringify() {
		return data;
	}
	int StringCell::to_numeric() {
		return 0;
	}
	StringCell::StringCell(string data, int x, int y, Table* table) : data(data), Cell(x, y, table) {}

	string NumberCell::stringify() {
		return "";
	}
	int NumberCell::to_numeric() {
		return data;
	}
	NumberCell::NumberCell(int data, int x, int y, Table* table) : data(data), Cell(x, y, table) {}


 	string DateCell::stringify() {
		char buf[50];
		tm temp;
		localtime_s(&temp, &data); // 초단위 지역시간

		strftime(buf, 50, "%F", &temp); //시간문자열로 바꿔줌

		return string(buf);
	}
	int DateCell::to_numeric() {
		return static_cast<int>(data);
	}
	DateCell::DateCell(string data, int x, int y, Table* table) : Cell(x, y, table) {
		//data: yyyy-mm-dd 꼴 문자열
		int year = atoi(data.c_str());
		int month = atoi(data.c_str() + 5);
		int day = atoi(data.c_str() + 8);

		tm timeinfo;
		timeinfo.tm_year = year - 1900;
		timeinfo.tm_mon = month - 1;
		timeinfo.tm_mday = day;
		timeinfo.tm_hour = 0;
		timeinfo.tm_min = 0;
		timeinfo.tm_sec = 0;

		data = mktime(&timeinfo);
	}

	class ExprCell : public Cell {
		string data;
		string* parsed_expr;

		//parse_expression을 통해 후위표기법 토큰이(셀이름, 숫자, 연산자) 저장되어있는 벡터 클래스
		vector<string> exp_vec;

		int precedence(char c);
		//중위표기법 수식을 후위표기법으로 변환하는 함수.
		void parse_expression();

	public:
		ExprCell(string data, int x, int y, Table* table);

		string stringify();
		int to_numeric();
	};

	//후위 표기법으로 수식 계산
	int ExprCell::to_numeric() {
		stack<double> stack;
		double result = 0;

		for (int i = 0; i < exp_vec.size(); i++) {
			string s = exp_vec[i];

			//셀이름인 경우
			if (isalpha(s[0])) {
				stack.push(table->to_numeric(s));
			}
			else if (isdigit(s[0])) {
				stack.push(atoi(s.c_str()));
			}
			else {
				double y = stack.top();
				stack.pop();
				double x = stack.top();
				stack.pop();
				
				switch (s[0]) {
				case '+':
					stack.push(x + y);
					break;
				case '-':
					stack.push(x - y);
					break;
				case '*':
					stack.push(x * y);
					break;
				case '/':
					stack.push(x / y);
					break;
				}
			}
		}
		return stack.top();
	}

	int ExprCell::precedence(char c) {
		switch (c) {
		case '(':
		case '[':
		case '{':
			return 0;
		case '+':
		case '-':
			return 1;
		case '*':
		case '/':
			return 2;
		}
		return 0;
	}

	void ExprCell::parse_expression() {

	}

} // namespace MyExcel

int main() {
	MyExcel::TextTable table(5, 5);
	std::ofstream out("test.txt");

	table.reg_cell(new MyExcel::Cell("Hello~", 0, 0, &table), 0, 0);
	table.reg_cell(new MyExcel::Cell("C++", 0, 1, &table), 0, 1);
	table.reg_cell(new MyExcel::Cell("Programming", 1, 1, &table), 1, 1);

	std::cout << std::endl << table;
	out << table;

	return 0;
};