#include <iostream>
#include <vector>
#include <queue>
using namespace std;
bool BFS_bigraph(vector <vector<int> > G, int N) {
    int v, u;
    queue<int> q;
    vector<char> colour(N, 'W');
    for(int i = 0; i < N; ++i) {
        if(colour[i] == 'W') {
            q.push(i);
            colour[i] = 'R';
            while(!q.empty()) {
                u = q.front();
                q.pop();
                for(int j = 0; j < G[u].size(); ++j) {
                    v = G[u][j];
                    if (colour[v] == 'W') {
						if (colour[u] == 'B')
							colour[v] = 'R';
						else
							colour[v] = 'B';
                        q.push(v);
                    }
                    else 
						if (colour[v] == colour[u])
							return false;
                }
            }
        }
    }
    return true;
}

int main() {
    int N, M, a, b;
    cin >> N >> M;
    vector<vector<int> > G(N, vector<int>());
    for (int i = 0; i < M; ++i) {
         cin >> a >> b;
         G[a - 1].push_back(b - 1);
         G[b - 1].push_back(a - 1);
    }
    if(BFS_bigraph(G, N))
        cout << "YES";
    else
        cout << "NO";
    return 0;
}
