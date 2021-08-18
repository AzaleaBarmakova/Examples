#include<vector>
#include<iostream>
#include<algorithm>
using namespace std;
void DFS_visit_TopSort(vector<vector<int> > &G, int N, vector<int> &order, vector<char> &colour, bool &hasCycle, int v) {
    colour[v] = 'G';
	N = G[v].size();
    for(int i = 0; i < N; ++i) {
		if (colour[G[v][i]] == 'G')
			hasCycle = true;
        if (colour[G[v][i]] == 'W')
            DFS_visit_TopSort(G, N, order, colour, hasCycle, G[v][i]);
        }
    colour[v] = 'B';
    order.push_back(v);
    return;
}

vector<int> TopSort(vector<vector<int> > &G) {
    int N = G.size();
    vector<int> order;
    vector<char> colour(N, 'W');
    bool hasCycle = false;
    for(int i = 0; i < N; ++i)
        if (colour[i] == 'W')
            DFS_visit_TopSort(G, N, order, colour, hasCycle, i);
    if (hasCycle) {
        return vector<int>(0); 
	}
    reverse(order.begin(), order.end());
    return order;
}

int main() {
    int N, M, x, y;
    cin >> N >> M;
    vector<vector<int> > G(N, vector<int>());
    for(int i = 0; i < M; ++i) {
        cin >> x >> y;
        G[x - 1].push_back(y - 1);
    }
    vector<int> order = TopSort(G);
    if (order.size() == 0) {
        cout << -1 << endl;
        return 0;
    }
	N = order.size();
    for(int i = 0; i < N; ++i)
        cout << order[i] + 1 << ' ';
    return 0;
}
