/*
Implement 2-SUM algorithm
The file contains 1 million integers, both positive and negative (there might be some repetitions!).

Compute number of values t in [-10000, 10000] such that distict numbers x, y in input file such that x + y = t

*/

#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <algorithm>
#include <ctime>
#include "2-sum.h"

using namespace std;
typedef long long ll;


/*
algorithm

1. for each t and x in input_file, search t-y in input_file -> O(n) * 20000 = 20 billion

2. Binary search
	2-1 get sorted set xs
	2-2 for each x in xs, find interval I of xs such that -10000-x <= y <=10000-x. (binary search)
	2-3 Insert x+xs[I] elements in a hash table.
	2-4 repeat 2, 3 for every elements in xs
	
	Time Complexity: O(nlogn) for n=|xs|

*/

template<typename T>
void insertValidTvalue(const vector<ll>& summands, T&  hash) {
	int counter = 1;
	for (const auto& x : summands) {
		int start = lower_bound(summands.begin(), summands.end(), - 10000 - x) - summands.begin();
		if (start == summands.size()) continue;
		int end = upper_bound(summands.begin(), summands.end(), 10000 - x) - summands.begin();
		counter++;
		//std::cout << counter++ << "th intervals: " << start << ", " << end << endl;
		//if (++counter % 1000 == 0) { std::cout << counter << "th iterations..." << endl; }
		for (int i = start; i < end; i++) {
			hash.insert(x + summands[i]);
		}
	}
}



bool findSummandPair(const set <ll> & summand_set, ll t) {
	for (const auto& x : summand_set) {		
		if (t-x == x) continue;
		if (summand_set.find(t-x) != summand_set.end())
			return true;
	}

	return false;
}

int main() {
	vector<ll> summands;
	set<ll> summand_set;
	ifstream input_file("2sum.txt");

	time_t start_read_file, start_sorting, start_binary_search, end_binary_search;

	start_read_file = clock();
	if (input_file.is_open()) {
		string integer;
		while (input_file >> integer) {
			summands.push_back(stoll(integer));
			summand_set.insert(stoll(integer));
		}
	}
	start_sorting = clock();
	//binary search
	sort(summands.begin(), summands.end()); //sort summands array

	set<int> hash_table;
	start_binary_search = clock();
	insertValidTvalue<set<int>>(summands, hash_table);
	end_binary_search = clock();

	std::cout << "Reading file...: " << (double)(start_sorting - start_read_file) / CLOCKS_PER_SEC << endl;
	std::cout << "Sorting arrays...: " << (double)(start_binary_search - start_sorting) / CLOCKS_PER_SEC << endl;
	std::cout << "Binary Searching...: " << (double)(end_binary_search - start_binary_search) / CLOCKS_PER_SEC << endl;
	std::cout << "Number of target values t: " << hash_table.size() << endl;


	std::cout << "Hash Performance - Chaining Method" << std::endl;
	ChainHash<int> chain_hash_table(10007);
	start_binary_search = clock();
	insertValidTvalue<ChainHash<int>>(summands, chain_hash_table);
	end_binary_search = clock();
	std::cout << "Binary Searching...: " << (double)(end_binary_search - start_binary_search) / CLOCKS_PER_SEC << endl;
	std::cout << "Number of target values t: " << chain_hash_table.size() << endl;

	std::cout << "Hash Performance - OpenAddressing Method" << std::endl;
	OpenAddressingHash<int> open_addressing_hash_table(10007);
	start_binary_search = clock();
	insertValidTvalue<OpenAddressingHash<int>>(summands, open_addressing_hash_table);
	end_binary_search = clock();
	std::cout << "Binary Searching...: " << (double)(end_binary_search - start_binary_search) / CLOCKS_PER_SEC << endl;
	std::cout << "Number of target values t: " << open_addressing_hash_table.size() << endl;


	return 0;
}