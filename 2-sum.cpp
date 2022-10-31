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

using namespace std;
typedef long long ll;

//chaining hash structure
void chaining_hash();

//open addressing hash structure
void openaddressing_hash();

/*
algorithm

1. for each t and x in input_file, search t-y in input_file -> O(n) * 20000 = 20 billion

2. 
	2-1 get sorted set xs
	2-2 for each t, 


*/


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
	if (input_file.is_open()) {
		string integer;
		while (input_file >> integer) {
			summands.push_back(stoll(integer));
			summand_set.insert(stoll(integer));
		}
	}

	int counter = 0;
	for (int t = -10000; t <= 10000; t++) {
		if (findSummandPair(summand_set, t))
			counter += 1;
		std::cout << t << "th iteration.." << endl;
	}

	std::cout << "Number of target values t: " << counter << endl;

	return 0;
}