#include <fstream>
#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>
#include <sstream>
#include <map>

//시간측정
#include <time.h>


using namespace std;

//테스트 방향그래프
//bool visited[8] = { false };
//bool visited_rev[8] = { false };
map<int, vector<int>> test{ {1, vector<int>{2}},
	{2, vector<int>{3,4}}, 
	{3, vector<int>{1}},
	{4, vector<int>{5}},
	{5, vector<int>{6}},
	{6, vector<int>{7}},
	{7, vector<int>{5}} };
map<int, vector<int>> test_rev{ {1,vector<int>{3}},
	{2, vector<int>{1}}, 
	{3, vector<int>{2}},
	{4, vector<int>{2}},
	{5, vector<int>{4, 7}},
	{6, vector<int>{5}}, 
	{7, vector<int>{6}} };


//visited map
map<int, bool> visited;
map<int, bool> visited_rev;


void dfs(map<int, vector<int>>& graph, const int start, map<int, bool>& visited, stack<int>& path) {
	stack<int> s;
	//magic order

	s.push(start);

	while (s.empty() == false) {
		int t = s.top();
		bool is_leaf = true;
		s.pop();
		visited[t] = true;

		for (int i = 0; i < graph[t].size(); i++) {
			//if (graph.find(t) == graph.end()) continue;
			
			int v = graph[t][i];
			if (visited[v] == true) continue;

			s.push(t);
			s.push(v);
			is_leaf = false;
			break;
		}

		if (is_leaf) path.push(t);
	}
}

vector<int> dfsLoop(map<int, vector<int>>& graph, map<int, vector<int>>& graph_rev) {
	int n = graph.size();
	stack<int> path;

	//dfs로 탐색이 끝난 순서로 stack 채우기
	for (const auto& edge : graph) {
		int k = edge.first;
		if (visited[k] == true) continue;
		dfs(graph, k, visited, path);
	}

	//stack 순서대로 역방향 그래프를 dfs로 탐색
	vector<int> scc;

	while (path.empty() == false) {
		stack<int> stack;
		int v = path.top();
		path.pop();

		if (visited_rev[v] == true) continue;

		dfs(graph_rev, v, visited_rev, stack);
		scc.push_back(stack.size());
	}

	sort(scc.rbegin(), scc.rend());

	//scc가 5개 미만인 경우 0으로 채움.
	while (scc.size() < 5){
		scc.push_back(0);
	}
	return vector<int>(scc.begin(), scc.begin() + 5);
	
}

void printVec(const vector<int>& vec) {
	for (const auto& elem : vec) {
		std::cout << elem << ",";
	}
}


int main() {

	map<int, vector<int>> graph, graph_rev;
	string eachrow;
	int row_num = 0;
	
	// 66s
	std::cout << "Reading files...: ";

	ifstream input_file("scc.txt");

	time_t start, end, end_dfs;

	start = time(NULL);
	if (input_file.is_open()) {

		while (getline(input_file, eachrow)) {
			stringstream eachrow_stream(eachrow);
			string word;
			vector<int> row;
			while (getline(eachrow_stream, word, ' ')) {
				row.push_back(stoi(word));
			}

			graph[row[0]].push_back(row[1]);
			graph_rev[row[1]].push_back(row[0]);
		}
	}
	end = time(NULL);
	std::cout << (double)(end-start) << endl;

	// 101s
	std::cout << "Starting DFS...: ";
	vector<int> vec = dfsLoop(graph, graph_rev);
	end_dfs = time(NULL);
	std::cout << (double)(end_dfs - end) << endl;
	printVec(vec);

}
