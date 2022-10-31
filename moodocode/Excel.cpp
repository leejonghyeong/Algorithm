#include <ostream>
#include <fstream>
#include <iostream>
#include <vector>
#include <stack>
#include <string>
#include <stack>

//vector�� stack�� ���� ������ ��
//Question: �� Cell�� Table ��ü�� ��������ڸ� �� ���Ǿ��ϴ°�?

using namespace std;

namespace MyExcel {
	class Table;

	/*
	class Cell {
	protected:
		Table* table; // ��� ���̺� �����ִ���
		int x, y; //���̺��� ��ġ
		string data;

	public:
		virtual string stringify() { return data; } // private ������ �����ϱ� ���� ����Լ�
		virtual int to_numeric() { return 0; }		// 

		Cell(string data, int x, int y, Table* table) : data(data), x(x), y(y), table(table) {}
		

	};*/

	//���ڿ� �̿ܿ��� ���� ���� �����͸� �������� Cell �������̽� ����
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

		Cell*** data_table; //3�� ������
	public:
		Table(int max_row_size, int max_col_size);
		~Table();


		void reg_cell(Cell* c, int row, int col); // cell ���	

		int to_numeric(const string& s); 	//���̸�(eg: A3, B6)���� ���� ������ ��ȯ
		int to_numeric(int x, int y);  //��, �� ��ȣ�� ���� ������ ��ȯ

		string stringify(const string& s); // �ش� ��ġ�� Cell �����͸� ��´�.
		string stringify(int row, int col);

		virtual string print_table() = 0; // overriding�� �Լ�

	};

	Table::Table(int max_row_size, int max_col_size) : max_row_size(max_row_size), max_col_size(max_col_size) {
		data_table = new Cell * *[max_row_size];
		for (int i = 0; i < max_row_size; i++) {
			data_table[i] = new Cell * [max_col_size];
		}
		//NULL�����ͷ� �ʱ�ȭ
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

		//�̹� ��ϵǾ��ִ� ��� ���� �� ���
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
	Table class �� ��ӹ޴� TextTable ����

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
		//����� ���̺� ���ڿ�
		string total_table;
		
		//�� ������ ���� �� ���ڿ��� ���̸� ����. 
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

		//���̺� �ʱ�ȭ; ���̺� �������
		total_table += "    ";
		int total_wide = 4;
		//���̺� ù ��° �� �ۼ�; �� �̸� A,B,...,AA,AB,...
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
			//���� ���м�
			total_table += repeat_char(total_wide, '-');
			total_table += "\n";
			//�� ��ȣ
			total_table += to_string(i + 1) + repeat_char(4 - to_string(i + 1).length(), ' ');
			
			//�� ������ ������ ���ڿ� �Է�
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
	������ ������ Cell �ǻ��Ŭ���� ����
	string, int, time_t ������ Ÿ��.
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
		// yyyy-mm-dd ������ �Է¸� �޴´ٰ� ����.
		// time_t �ڷ����� 1970����� ������� �帥 �ð��� �ʴ����� ������ ������ ����.
		time_t data;

	public:
		string stringify();
		int to_numeric();

		DateCell(string data, int x, int y, Table* table);
	};


	/*
	Cell �ǻ�� Ŭ���� ����Լ� ����
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
		localtime_s(&temp, &data); // �ʴ��� �����ð�

		strftime(buf, 50, "%F", &temp); //�ð����ڿ��� �ٲ���

		return string(buf);
	}
	int DateCell::to_numeric() {
		return static_cast<int>(data);
	}
	DateCell::DateCell(string data, int x, int y, Table* table) : Cell(x, y, table) {
		//data: yyyy-mm-dd �� ���ڿ�
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

		//parse_expression�� ���� ����ǥ��� ��ū��(���̸�, ����, ������) ����Ǿ��ִ� ���� Ŭ����
		vector<string> exp_vec;

		int precedence(char c);
		//����ǥ��� ������ ����ǥ������� ��ȯ�ϴ� �Լ�.
		void parse_expression();

	public:
		ExprCell(string data, int x, int y, Table* table);

		string stringify();
		int to_numeric();
	};

	//���� ǥ������� ���� ���
	int ExprCell::to_numeric() {
		stack<double> stack;
		double result = 0;

		for (int i = 0; i < exp_vec.size(); i++) {
			string s = exp_vec[i];

			//���̸��� ���
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