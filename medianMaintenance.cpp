/*
Median Maintenance

The goal of this problem is to implement the "Median Maintenance" algorithm (covered in the Week 3 lecture on heap applications).  The text file contains a list of the integers from 1 to 10000 in unsorted order; you should treat this as a stream of numbers, arriving one by one.  
Letting x_i denote the i th number of the file, the kth median m_k is defined as the median of the numbers x_1,..,x_k.
(So, if k is odd, then m_k is (k+1)/2 th smallest number among x_1,...,x_k;
if k is even, then m_k is the k/2 th smallest number among x_1,...,x_k)

In the box below you should type the sum of these 10000 medians, modulo 10000 (i.e., only the last 4 digits).  
That is, you should compute (m_1+m_2+m_3 + ... + m_10000 ) mod 10000.


Moreover, compare heap-based and search-tree-base implementations of the algorithm


- 알고리즘:
각 수의 입력마다 최소힙 h_min, 최대힙 h_max 둘 중 하나에 집어넣는다.
최소힙은 median보다 큰 수가 들어가고 최대힙에는 median 이하의 수가 들어가게 만든다.
median은 최대힙의 최대값이 되도록 만든다.

만약 입력이 있을때마다 median과 비교하여 최소힙 혹은 최대힙에 집어넣게 되면 median이 최소힙에 있을 수도 있다.
따라서 최소힙과 최대힙의 사이즈를 같게 만들되* 최소힙의 최소값이 최대힙의 최대값보다 항상 크도록 수정한다.
*최대힙 사이즈 = 최소힙 사이즈 or 최소힙 사이즈 + 1 이 되어야 median이 최대힙의 최대값이 된다.

1 5 3 7 6
h_min h_max median
		1		1
5		1		1
5		3,1		3
7,5		3,1		3
7,5		6,3,1 -> 5,6 swap

*/

#include <fstream>
#include <iostream>
#include <sstream>
#include <queue>
#include <vector>
#include <cassert>

using namespace std;

int findMedian(priority_queue<int>& h_high, priority_queue<int>& h_low, const int integer) {
	// h_high는 최소힙, h_low는 최대힙

	if (h_low.size() == h_high.size()) {
		h_low.push(integer);
	}
	else {
		h_high.push(-integer);
		assert(h_high.size() == h_low.size());
	}

	//check h_high.top() > h_low.top()
	if (h_low.empty() || h_high.empty()) return integer;

	int min_high = -h_high.top();
	int max_low = h_low.top();
	if (min_high < max_low) {
		h_high.pop();
		h_low.pop();
		h_high.push(-max_low);
		h_low.push(min_high);
	}

	return h_low.top();
}

int main() {
	vector<int> numbers;
	priority_queue<int> h_high, h_low;
	ifstream input_file("median.txt");

	if (input_file.is_open()) {
		string integer;
		while (input_file >> integer) {
			numbers.push_back(stoi(integer));
		}
	}

	int median_sum = 0;
	for (const auto& integer : numbers) {
		median_sum += findMedian(h_high, h_low, integer);
	}
	median_sum = median_sum % 10000;

	std::cout << "Sum of medians: " << median_sum << endl;

	return 0;
}
