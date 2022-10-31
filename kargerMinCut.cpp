/*

- minimum cut problem
- randomized contraction algorithm

*/

#include <fstream>
#include <iostream>
#include <vector>
#include <random>
#include <string>
#include <set>
#include <utility>
#include <sstream>

using namespace std;


//random device�� ���� ������������ ���� �ʱ�ȭ
random_device rd;
mt19937 gen(rd());

void printVector(const vector<int>& vec) {
	for (const auto& elem : vec)
		std::cout << elem << " ";
	std::cout << endl;
}

void print2dVector(const vector<vector<int>>& vec) {
	for (const auto& elem : vec)
		printVector(elem);
}

//choose random edge
pair<int, int> randomEdge(vector<vector<int>>& adj_list, set<int> vertex_set) {
	//std::cout << "choose random vertex" << endl;
	//choose random vertex from vertex set
	uniform_int_distribution<int> dist(0, vertex_set.size() - 1);
	int rand_position = dist(gen);
	auto it = vertex_set.begin();
	advance(it, rand_position);
	int rand_vertex = *it - 1;

	//std::cout << "choose random opposite nodes" << endl;
	//choose random nodes connected to the vertex
	uniform_int_distribution<int> dist2(1, adj_list[rand_vertex].size() - 1);
	int opposite_vertex = dist2(gen);
	return pair<int, int>(rand_vertex, adj_list[rand_vertex][opposite_vertex] - 1);
}

//edge contraction
void randomEdgeContraction(vector<vector<int>>& adj_list, set<int>& vertex_set) {
	//std::cout << "get random edge" << endl;
	pair<int, int> rand_edge = randomEdge(adj_list, vertex_set);
	//v1, v2 indicates index number. v1 = (vertex name of v1) - 1
	int v1 = rand_edge.first;
	int v2 = rand_edge.second;

	//std::cout << "vertex " << v1+1 << ", " << v2+1 << endl;
	//degree of v1, v2
	int d1 = adj_list[v1].size();
	int d2 = adj_list[v2].size();
	//std::cout << "degree " << d1 << ", " << d2 << endl;

	//std::cout << "remove v2 from adjacent list of v1" << endl;
	//remove v2 from adjacent list of v1
	vector<int> removed_vec;
	for (const auto& elem: adj_list[v1]) {
		if (elem != (v2 + 1)) {
			removed_vec.push_back(elem);
		}
	}
	removed_vec.swap(adj_list[v1]);

	//std::cout << "copy-and-paste" << endl;
	//copy-and-paste all nodes connected with v2 onto v1 except v1 itself and removed nodes
	for (int i = 1; i < d2; i++) {
		int vertex = adj_list[v2][i];
		if (vertex == (v1 + 1))
			continue;

		adj_list[v1].push_back(vertex);
	}

	//std::cout << "change all v2 into v1" << endl;
	//change v2 into v1 in adjacent list
	int n = adj_list.size();
	for (int v = 0; v < n; v++) {
		if (v == v1 || v == v2)
			continue;

		int d = adj_list[v].size();
		for (int w = 0; w < d; w++) {
			if (adj_list[v][w] == (v2 + 1)) {
				adj_list[v][w] = v1 + 1;
			}
		}
	}

	//std::cout << "remove v2" << endl;
	//remove v2 node and its adjacent nodes
	vector<int>().swap(adj_list[v2]);
	//remove v2 from vertex set
	vertex_set.erase((v2 + 1));
}

int findMinimumCut(vector<vector<int>> adj_list, set<int> vertex_set) {
	int cnt = 1;
	//print2dVector(adj_list);
	while (vertex_set.size() > 2) {
		randomEdgeContraction(adj_list, vertex_set);
		//std::cout << endl << cnt << "th try: " << endl;
		//print2dVector(adj_list);
		cnt += 1;
	}

	int last_vertex = *begin(vertex_set) - 1;
	int minimum_cut = 0;


	for (const auto& vertex : adj_list[last_vertex]) {
		if (vertex != (last_vertex + 1))
			minimum_cut += 1;
	}
	std::cout << endl;
	for (const auto& elem : vertex_set) {
		std::cout << elem << ", ";
	}
	
	std::cout << endl << "Minimum Cut: " << minimum_cut << endl;
	return minimum_cut;
}

int main() {
	ifstream input_file("./kargerMinCut.txt");
	vector<vector<int>> adj_list;
	set<int> vertex_set;

	//2���� �迭 �б�
	string eachrow;
	if (input_file) {
		while (getline(input_file, eachrow)) {
			stringstream eachrow_stream(eachrow);
			string word;
			vector<int> row;
			while (getline(eachrow_stream, word, '\t')) {
				row.push_back(stoi(word));
			}

			adj_list.push_back(row);
			//row[0] is vertex number(name)
			vertex_set.insert(row[0]);
		}
	}

	vector<vector<int>> test_list = { vector<int>{1,2,4,5},
										vector<int>{2,1,3},
										vector<int>{3,2,4,7},
										vector<int>{4,1,3,6},
										vector<int>{5,1,6,8},
										vector<int>{6,4,5,7},
										vector<int>{7,3,6,8},
										vector<int>{8,5,7} };
	set<int> test_vertex = { 1,2,3,4,5,6,7,8 };

	int repeat_num = 200;
	int minimum = 0;
	for (int i = 0; i < repeat_num; i++) {
		int temp_min = findMinimumCut(adj_list, vertex_set);
		if (minimum == 0 || minimum > temp_min)
			minimum = temp_min;
	}

	std::cout << "Minimum Cut over 20 tries: " << minimum << endl;

	return 0;
}


/*
1. edge������ �ƴ϶� vertex�� �ΰ� ������ ���� �ݺ���
	1-1. edge contraction�ϸ� adjacent vertices ����ȭ
	1-2. self edge ����
2. ������ ���� edge ������ �����ϸ� ��.


algorithm
1. edge �����ϰ� ����
2. edge ���� �� vertices v1 < v2 �� v1�� �����.
3. self edge�� �����Ѵ�.
	: adj_list�� ������ �ʿ䰡 ����. -> component set�� ��������
	adj_list�� �����Ѵ� -> edge set ����
4. vertex�� �� �� v, w�� ������ �� ���� edge������ ����.

edge set �����ÿ�
(x, v2), (v2, y) -> (x, v1), (v1, y)�� �ٲ۴�.

(v1, v2), (v2, v1)�� ������ ���� �� �ִ�.
-> v1, v2 contraction �ÿ��� �����ؾ���. �� ������ �ߺ�����ؾ���
-> edge_set���� multiset �����̳� ���.

adj_list ������ �ؾ��ϴ°�??

adj_list and edge_set �� �� �ϳ���!!
edge_set�� �� �� ������ O(m) �ɸ�
adj_list�� O(m) memory�� ���������� O(d_v) ��ŭ �ɸ�.

*/