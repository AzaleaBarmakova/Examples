#include<vector>
#include<iostream>
#include<utility>
#include<algorithm>
using namespace std;

void DFS_visit_bridges(vector<vector<pair<int, int> > > &G, vector<int> &bridges, vector<bool> &used, int &Gtime, vector<int>& tin, vector<int> &up, int numberV, int parentEdge) {
    used[numberV] = true;
    Gtime++;
    tin[numberV] = Gtime;
	up[numberV] = Gtime;
    for(int i = 0; i < G[numberV].size(); ++i) {
        int v = G[numberV][i].first;
        int numberE = G[numberV][i].second;
        if (numberE == parentEdge)
            continue;
        if (used[v])
            up[numberV] = min(up[numberV], tin[v]);
        else {
            DFS_visit_bridges(G, bridges, used, Gtime, tin, up, v, numberE);
            up[numberV] = min(up[numberV], up[v]);
            if (up[v] > tin[numberV])
                bridges.push_back(numberE);
        }
    }
}


vector<int> find_bridges(vector<vector<pair<int, int> > > &G) {
    int N = G.size();
    vector<bool> used(N, false);
    vector<int> bridges;
    vector<int> tin(N);
    vector<int> up(N);
    int Gtime = 0;
    for(int i = 0; i < N; ++i)
        if (!used[i])
            DFS_visit_bridges(G, bridges, used, Gtime, tin, up, i, -1);
    sort(bridges.begin(), bridges.end());
    return bridges;
}

int main() {
    int N, M, a, b;
    cin >> N >> M;
    vector<vector<pair<int, int> > > G(N, vector<pair<int, int> > ());
    for(int i = 0; i < M; ++i) {
        cin >> a >> b;
        G[a - 1].push_back(pair<int, int>(b - 1, i + 1));
        G[b - 1].push_back(pair<int, int>(a - 1, i + 1));
    }
    vector<int> bridges = find_bridges(G);
    cout << bridges.size() << endl;
    for(int i = 0; i < bridges.size(); ++i)
        cout << bridges[i] << endl;
	return 0;
}
