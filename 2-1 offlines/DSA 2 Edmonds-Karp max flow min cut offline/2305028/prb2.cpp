#include <bits/stdc++.h>
using namespace std;

struct Edge {
    int v, cap, rev;
};

vector<vector<Edge>> graph;
vector<int> level, parent_v, parent_e;

void addEdge(int u, int v, int cap) {
    Edge a = {v, cap, (int)graph[v].size()};
    Edge b = {u, 0, (int)graph[u].size()};
    graph[u].push_back(a);
    graph[v].push_back(b);
}

bool bfs(int s, int t) {
    fill(parent_v.begin(), parent_v.end(), -1);
    queue<int> q;
    q.push(s);
    parent_v[s] = s;

    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int i = 0; i < graph[u].size(); i++) {
            Edge &e = graph[u][i];
            if (parent_v[e.v] == -1 && e.cap > 0) {
                parent_v[e.v] = u;
                parent_e[e.v] = i;
                if (e.v == t) return true;
                q.push(e.v);
            }
        }
    }
    return false;
}

int maxFlow(int s, int t) {
    int flow = 0;
    while (bfs(s, t)) {
        int f = INT_MAX;
        for (int v = t; v != s; v = parent_v[v]) {
            int u = parent_v[v];
            f = min(f, graph[u][parent_e[v]].cap);
        }
        for (int v = t; v != s; v = parent_v[v]) {
            int u = parent_v[v];
            Edge &e = graph[u][parent_e[v]];
            e.cap -= f;
            graph[v][e.rev].cap += f;
        }
        flow += f;
    }
    return flow;
}

int main() {
    int N, K, M;
    cin >> N >> K >> M;

    int source = N;
    int sink = N + 1;
    graph.assign(N + 2, vector<Edge>());
    parent_v.resize(N + 2);
    parent_e.resize(N + 2);

    for (int i = 0; i < K; i++)
        addEdge(source, i, 1);

    for (int i = K; i < N; i++)
        addEdge(i, sink, 1);

    vector<pair<int,int>> compat;
    for (int i = 0; i < M; i++) {
        int u, v;
        cin >> u >> v;
        compat.push_back({u, v});
        addEdge(u, v, 1);
    }

    int ans = maxFlow(source, sink);
    cout << ans << "\n";

    for (auto &[u, v] : compat) {
        for (auto &e : graph[v]) {
            if (e.v == u && e.cap == 1) {
                cout << u << " " << v << "\n";
            }
        }
    }
    return 0;
}

