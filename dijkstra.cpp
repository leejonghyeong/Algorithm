#include <queue>
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>

using namespace std;

/*
���ͽ�Ʈ�� �˰���.
�⺻������ BFS �˰���� ���밡 ����. BFS�� �ٸ� ���� ũ��
1. Queue ��� Priority Queue�� ����Ѵ�.
2. visited �� üũ���� �ʴ´�.

1��: ���ͽ�Ʈ�� �˰��򿡼��� neihborhood nodes �߿��� ���� �Ÿ�(from start)�� ª�� node������ �����Ѵ�.
���� Priority Queue�� �̿��Ͽ� O(log E) ���⵵������ �ּ� �Ÿ� node�� ã�ư� �� �ִ�.

2��: ���ͽ�Ʈ�� �˰��򿡼� �̹� �湮�� node�� �� ���� path�� �湮�� �� ������ �����Ѵ�. ���� ���� �׶��� �湮�Ѵ�.
���� visited üũ�� faster path updates�� ��ü�ȴٰ� ��������.

*/

constexpr int INF = 1000000;
constexpr int N = 200;

vector<vector<pair<int,int>>> graph;
vector<int> dist(201, INF);

void dijkstra(vector<vector<pair<int,int>>>& graph, const int start) {
	priority_queue<pair<int, int>> pq;

	//������ �ʱ�ȭ
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
				//ť ��� �켱���� ť ���.
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
	
	//�� row���� "v,d" word�� ���� �а�, string�� pair<int, int>�� ��ȯ���ش�.
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