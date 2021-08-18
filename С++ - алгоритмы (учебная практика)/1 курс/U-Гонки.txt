#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <utility>
using namespace std;
int min_distance_cars(int N, int M, vector< vector<int> >& G, int u_x, int u_y, int v_x, int v_y){
    int i;
    int t_x, t_y, x, y;
    pair<int, int> t;
    vector<vector<int> > used(N, vector<int> (M, 0));
    vector<vector<int> > d(N, vector<int> (M, -1));
    queue<pair<int, int> > q;
    q.push(make_pair(u_x, u_y));
    d[u_x][u_y] = 0;
    used[u_x][u_y] = 1;
    while(!q.empty()) {
		t = q.front();
        q.pop();
        t_x = t.first;
        t_y = t.second;
        x = t_x;								//moving right
        y = t_y;
        while (y < M) {
            if(G[x][y] != 0)
                break;
            y++;
        }
        y = y - (y - t_y - 1)/2 - 1;
        if (used[x][y] == 0) {
            used[x][y] = 1;
            q.push(make_pair(x, y));
            d[x][y] = d[t_x][t_y] + 1;
            if (x == v_x && y == v_y)
                break;
        }

        x = t_x;							//moving left
        y = t_y;
        while (y >= 0) {
            if(G[x][y] != 0)
                break;
            y--;
        }

        y = y + (t_y - y - 1)/2 + 1;
        if (used[x][y] == 0) {
            used[x][y] = 1;
            q.push(make_pair(x, y));
            d[x][y] = d[t_x][t_y] + 1;
            if (x == v_x && y == v_y)
                break;
        }

        x = t_x;							//moving down
        y = t_y;
        while (x < N) {
            if(G[x][y] != 0)
                break;
            x++;
        }

        x = x - (x - t_x - 1)/2 - 1;
        if (used[x][y] == 0) {
            used[x][y] = 1;
            q.push(make_pair(x, y));
            d[x][y] = d[t_x][t_y] + 1;
            if (x == v_x && y == v_y)
                break;
        }

        x = t_x;							//moving up
        y = t_y;
        while (x >= 0) {
            if(G[x][y] != 0)
                break;
            x--;
        }
        x = x + (t_x - x - 1)/2 + 1;
        if (used[x][y] == 0) {
            used[x][y] = 1;
            q.push(make_pair(x, y));
            d[x][y] = d[t_x][t_y] + 1;
            if (x == v_x && y == v_y)
                break;
        }
    }
    return d[v_x][v_y];
}

int main() {
    int N, M, s_x, s_y, f_x, f_y;
    cin >> N >> M;
    char c;
    vector<vector<int> > G(N, vector<int>(M));
    for (int i = 0; i < N; ++i) {
        for(int j = 0; j < M; ++j) {
                cin >> c;
                if (c == '#')
                    G[i][j] = 1;
                if (c == '.')
                    G[i][j] = 0;
                if (c == 'S') {
                    G[i][j] = 0;
                    s_x = i;
                    s_y = j;
                }
                if (c == 'T') {
                    G[i][j] = 0;
                    f_x = i;
                    f_y = j;
                }
		}
    }
	cout << min_distance_cars(N, M, G, s_x, s_y, f_x, f_y);
    return 0;
 }

