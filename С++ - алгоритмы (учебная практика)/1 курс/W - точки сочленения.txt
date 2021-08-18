#include<vector>
#include<iostream>
#include<utility>
#include<algorithm>
using namespace std;

void DFS_visit_cutpoints(vector<vector<int> > &G, vector<int> &cutpoints, vector<bool> &used, int &Gtime, vector<int> &tin, vector<int> &up, int numberV, int parentV) {
    used[numberV] = true;
	Gtime++;
    tin[numberV] = Gtime;
	up[numberV] = Gtime;
    int children = 0;
    int v;
    for(int i = 0; i < G[numberV].size(); ++i) {
        v = G[numberV][i];
        if (v == parentV)
            continue;
        if (used[v])
            up[numberV] = min(up[numberV], tin[v]);
        else {
            DFS_visit_cutpoints(G, cutpoints, used, Gtime, tin, up, v, numberV);
            up[numberV] = min(up[numberV], up[v]);
            if ((up[v] >= tin[numberV]) && (parentV != -1))
                cutpoints.push_back(numberV);
            children++;
        }
    }
    if (parentV == -1 && children > 1)
        cutpoints.push_back(numberV);
}

vector<int> findCutpoints(vector<vector<int> > &G) {
    int N = G.size();
    vector<bool> used(N, false);
    vector<int> cutpoints;
    vector<int> tin(N);
    vector<int> up(N);
    int Gtime = 0;
    for(int i = 0; i < N; ++i) 
        if (!used[i])
            DFS_visit_cutpoints(G, cutpoints, used, Gtime, tin, up, i, -1);
    sort(cutpoints.begin(), cutpoints.end());
    vector<int> res;
	N = cutpoints.size();
    if (N != 0)
        res.push_back(cutpoints[0]);
    for(int i = 1; i < N; ++i)
        if (cutpoints[i] != cutpoints[i - 1])
            res.push_back(cutpoints[i]);
    return res;
}

int main() {
    int N, M, a, b;
    cin >> N >> M;
    vector<vector<int> > G(N, vector<int> ());
    for(int i = 0; i < M; ++i) {               //lists
        cin >> a >> b;
        G[a - 1].push_back(b - 1);
        G[b - 1].push_back(a - 1);
    }
    vector<int> cutpoints = findCutpoints(G);
	a = cutpoints.size();
    cout << a << endl;
    for(int i = 0; i < a; ++i)
        cout << cutpoints[i] + 1 << endl;
	return 0;
}

