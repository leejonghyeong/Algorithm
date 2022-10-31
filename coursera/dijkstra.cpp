#include <queue>
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>

using namespace std;

/*
다익스트라 알고리즘.
기본적으로 BFS 알고리즘과 뼈대가 같다. BFS와 다른 점은 크게
1. Queue 대신 Priority Queue를 사용한다.
2. visited 를 체크하지 않는다.

1번: 다익스트라 알고리즘에서는 neihborhood nodes 중에서 가장 거리(from start)가 짧은 node순으로 접근한다.
따라서 Priority Queue를 이용하여 O(log E) 복잡도만으로 최소 거리 node를 찾아갈 수 있다.

2번: 다익스트라 알고리즘에서 이미 방문한 node라도 더 빠른 path로 방문할 수 있으면 갱신한다. 또한 오직 그때만 방문한다.
따라서 visited 체크는 faster path updates로 대체된다고 생각하자.

*/

constexpr int INF = 1000000;
constexpr int N = 200;

vector<vector<pair<int,int>>> graph;
vector<int> dist(201, INF);

void dijkstra(vector<vector<pair<int,int>>>& graph, const int start) {
	priority_queue<pair<int, int>> pq;

	//시작점 초기화
	dist[start] = 0;
	pq.push(make_pair(0, start));

	while (pq.empty() == false) {
		pair<int, int> node = pq.top();
		pq.pop();

		int d = - node.first;
		int v = node.second;

		for (auto& nhd_node : graph[v]) {
			int edge_len = nhd_node.first;
			int nhd_w = nhd_node.second;
			if (d + edge_len < dist[nhd_w]) {
				dist[nhd_w] = d + edge_len;
				//큐 대신 우선순위 큐 사용.
				pq.push(make_pair(-dist[nhd_w], nhd_w));
			}
		}
		

	}

}


int main() {
	/*
	The file contains an adjacency list representation of an undirected weighted graph with 200 vertices labeled 1 to 200.
	Each row consists of the node tuples that are adjacent to that particular vertex along with the length of that edge.
	For example, the 6th row has 6 as the first entry indicating that this row corresponds to the vertex labeled 6.
	The next entry of this row "141,8200" indicates that there is an edge between vertex 6 and vertex 141 that has length 8200. 
	The rest of the pairs of this row indicate the other vertices adjacent to vertex 6 and the lengths of the corresponding edges.
	*/

	ifstream input_file("dijkstra.txt");
	
	//각 row마다 "v,d" word를 끊어 읽고, string을 pair<int, int>로 변환해준다.
	if (input_file.is_open()) {
		string eachrow;
		graph.push_back(vector<pair<int,int>>());

		while (getline(input_file, eachrow)) {
			stringstream eachrow_stream(eachrow);
			string word;
			bool drop_first = true;
			vector<pair<int, int>> row;
			while (getline(eachrow_stream, word, '\t')) {
				if (drop_first) { drop_first = false; continue; }

				stringstream word_stream(word);
				string vertex, distance;
				getline(word_stream, vertex, ',');
				getline(word_stream, distance);
				row.push_back(make_pair(stoi(distance), stoi(vertex)));
			}

			graph.push_back(row);
		}
	}

	dijkstra(graph, 1);

	vector<int> vertices = { 7,37,59,82,99,115,133,165,188,197 };
	for (const auto& elem : vertices) {
		std::cout << dist[elem] << ",";
	}

}
