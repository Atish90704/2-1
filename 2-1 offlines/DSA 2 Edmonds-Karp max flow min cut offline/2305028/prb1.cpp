#include <bits/stdc++.h>
using namespace std;

struct Edge {
    int v;
    int capacity;
    int flow;
    int rev;
};

vector<vector<Edge>> graph;
vector<int> parent_v, parent_e;

void addEdge(int u, int v, int cap) {
    Edge a = {v, cap, 0, (int)graph[v].size()};
    Edge b = {u, 0, 0, (int)graph[u].size()};
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
            if (parent_v[e.v] == -1 && e.capacity > 0) {
                parent_v[e.v] = u;
                parent_e[e.v] = i;
                if (e.v == t) return true;
                q.push(e.v);
            }
        }
    }
    return false;
}

int edmondsKarp(int s, int t) {
    int maxFlow = 0;

    while (bfs(s, t)) {
        int pathFlow = INT_MAX;

        for (int v = t; v != s; v = parent_v[v]) {
            int u = parent_v[v];
            Edge &e = graph[u][parent_e[v]];
            pathFlow = min(pathFlow, e.capacity);
        }

        for (int v = t; v != s; v = parent_v[v]) {
            int u = parent_v[v];
            Edge &e = graph[u][parent_e[v]];
            e.capacity -= pathFlow;
            graph[v][e.rev].capacity += pathFlow;
            e.flow += pathFlow;
            graph[v][e.rev].flow -= pathFlow;
        }

        maxFlow += pathFlow;
    }
    return maxFlow;
}

int main() {
    int N, M;
    cin >> N >> M;

    graph.assign(N, vector<Edge>());
    parent_v.resize(N);
    parent_e.resize(N);

    vector<tuple<int,int,int>> edges;

    for (int i = 0; i < M; i++) {
        int u, v, c;
        cin >> u >> v >> c;
        edges.push_back({u, v, c});
        addEdge(u, v, c);
    }

    int s, t;
    cin >> s >> t;

    int maxFlow = edmondsKarp(s, t);
    cout << maxFlow << "\n";

    for (auto &[u, v, c] : edges) {
        for (auto &e : graph[u]) {
            if (e.v == v) {
                cout << u << " " << v << " " << e.flow << "/" << c << "\n";
                break;
            }
        }
    }
    return 0;
}
