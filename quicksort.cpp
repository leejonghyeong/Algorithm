
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

inline void swapVectorElem(vector<int>& vec, const int x, const int y) {
	int temp = vec[x];
	vec[x] = vec[y];
	vec[y] = temp;
}

int medianOfThree(const vector<int>& vec, const int left, const int right) {
	int middle = (left + right) / 2 ;
	int max_value = max({ vec[left], vec[middle], vec[right] });
	int min_value = min({ vec[left], vec[middle], vec[right] });
	vector<int> indices = { left, middle, right };

	for(const auto& elem: indices)
		if (vec[elem] != max_value && vec[elem] != min_value)
			return elem;
}

int quicksort(vector<int>& a, const int left, const int right) {
	//use last element as pivot element
	//swapVectorElem(a, left, right);

	//use median-of-three element as pivot element
	int median = 0;
	if (right - left >= 2)
		median = medianOfThree(a, left, right);
		swapVectorElem(a, left, median);

	int pivot = a[left];
	int i = left + 1;
	int num_comparison = right - left;

	// comparison step
	for (int j = left + 1; j <= right; j++) {
		if (a[j] < pivot) {
			swapVectorElem(a, i, j);
			i++;
		}
	}
	swapVectorElem(a, left, i - 1);

	// pivot position = i - 1
	// implement quicksort recurrently
	if ((i - 1) > left + 1)
		num_comparison += quicksort(a, left, (i - 1) - 1);
	if ((i - 1) < right - 1)
		num_comparison += quicksort(a, i, right);

	return num_comparison;
}


int main() {
	vector<int> input_array;
	
	fstream input_file("./quicksort.txt");

	if (input_file) {
		int data;
		while(input_file >> data) {
			input_array.push_back(data);
		}
	}
	
	std::cout << "Number of comparisons: " << quicksort(input_array, 0, input_array.size() - 1) << endl;

	vector<int> test_indices = { 0,7,8,5,2, 1 };
	//std::cout << "test median index: " << medianOfThree(test_indices, 1, 4)<<endl;
}