#include<iostream>
#include<vector>
using namespace std;
struct Graph {
	vector<char> colour;        //'W' 'G' 'B'
	vector<vector<int> > matrix;
	Graph(int v) {
		colour.resize(v, 'W');
		matrix.resize(v, vector<int>(v, 0));
	}
	Graph(Graph& g) {
		colour = g.colour;
		matrix = g.matrix;
	}
	~Graph() {
	}
	void buildMatrix(int s) {						//ready matrix will be enter
		int x = 0;
		for (int i = 1; i <= s; ++i) {
			cin >> x ;
			matrix[x - 1][i - 1] = 1;
			matrix[i - 1][x - 1] = 1;
		}	
	}
	int DFS_round(Graph &g) {     
		int k = 0, V = g.colour.size();
		for (int i = 0; i < V; ++i) 
			if (g.colour[i] == 'W') {
				DFS_visit(g, i);
				k++;
			}
		return k;
	} 
	void DFS_visit(Graph &g, int numberV) {
		g.colour[numberV] = 'G';
		int i = 0, V = g.colour.size();
		while (i < V) {
			if (g.matrix[numberV][i] == 1) 
				if (g.colour[i] == 'W')
					DFS_visit(g, i);
			i++;
		}
		g.colour[numberV] = 'B';
	}
};
int main() {
	int n = 0, k = 0;
	cin >> n;
	Graph g(n);
	g.buildMatrix(n);
	k = g.DFS_round(g);
	cout << k << endl;
	return 0;
}