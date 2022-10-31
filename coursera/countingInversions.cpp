#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include "karatsuba.h"

using namespace std;

/*
template<typename T>
class Subvector {
    vector<T>* const vec;
    size_t start;
    size_t end;
public:
    Subvector(vector<T>& vector, size_t start, size_t end) :
        vec(&vector),
        start(start),
        end(end)
    {}
    size_t size() const { return end - start; }
    T& operator[](size_t i) {
        return (*vec)[start + i];
    }
    const T& operator[](size_t i) const {
        return (*vec)[start + i];
    }
};
*/


vector<int> counting_split_inversions(const vector<int>& left, const vector<int>& right, vector<int>& original) {
    int i = 0;
    int j = 0;
    int n_left = left.size();
    int n_right = right.size();
    int n = original.size();
    vector<int> num_inversions = {0};

    //merge two sorted arrays
    for (int k = 0; k < n && j < n_right; k++) {
        if (i < n_left && left[i] < right[j]) {
            original[k] = left[i];
            i++;
        }
        else {
            original[k] = right[j];
            j++;
            karatsubaAdd(num_inversions, intToVec(n_left - i), 0);
        }
    }
    int i_stop = i;

    while (i + j < n) {
        original[i + j] = left[i];
        i++;
    }


    return num_inversions;
}

//inversion 카운팅 동시에 머지정렬수행
// a의 길이가 작다면 
vector<int> counting_inversion(vector<int>& a) {
    //초기값
    if (a.size() == 1) {
        return vector<int> {0};
    }
    if (a.size() <= 2) {
        if (a[0] < a[1]) return vector<int> {0};
        else {
            int first_term = a[0];
            a[0] = a[1];
            a[1] = first_term;
            return vector<int> {1};
        }
    }
    
    //call recurrence
	size_t half = a.size() / 2;
    vector<int> left(a.begin(), a.begin() + half);
    vector<int> right(a.begin() + half, a.end());


    vector<int> num_inversions_left = counting_inversion(left);
    vector<int> num_inversions_right = counting_inversion(right);
    // 정렬된 left, right를 다음 함수에 대입
    // left, right를 이용하여 a를 정렬
    vector<int> num_split_inversions = counting_split_inversions(left, right, a);
    
    karatsubaAdd(num_inversions_left, num_inversions_right, 0);
    karatsubaAdd(num_inversions_left, num_split_inversions, 0);
    return num_inversions_left;

}

void printvec(vector<int>& a) {
    reverse(a.begin(), a.end());
    for (const auto& elem: a) {
        std::cout << elem;
    }
    std::cout << endl;
}

int main() {
    //create vector<int> from file
    vector<int> a;
    ifstream inputFile("./countingInversion.txt");

    if (inputFile) {
        int data;
        while (inputFile >> data) {
            a.push_back(data);
        }
    }

    std::cout << "Number of inversions: ";
    vector<int> total_inversion = counting_inversion(a);
    printvec(total_inversion);

    return 0;
}
