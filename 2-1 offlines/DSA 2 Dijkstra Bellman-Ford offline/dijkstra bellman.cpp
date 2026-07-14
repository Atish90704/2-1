#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
const ll INF = 1e18;

int main() {
    int n, m;
    cin >> n >> m;

    vector<vector<pair<int, ll>>> graph(n + 1);
    for (int i = 0; i < m; i++) {
        int u, v;
        ll w;
        cin >> u >> v >> w;
        graph[u].push_back({v, w});
    }

    vector<ll> dist(n + 1, INF);
    priority_queue<pair<ll, int>, vector<pair<ll, int>>, greater<pair<ll, int>>> pq;

    dist[1] = 0;
    pq.push({0, 1});

    while (!pq.empty()) {
        auto [cost, u] = pq.top();
        pq.pop();

        if (cost > dist[u]) continue;

        for (auto [v, w] : graph[u]) {
            if (dist[v] > cost + w) {
                dist[v] = cost + w;
                pq.push({dist[v], v});
            }
        }
    }

    for (int i = 1; i <= n; i++)
        cout << dist[i] << " ";
}


//bellman
#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
const ll INF = 1e18;

struct Edge {
    int u, v;
    ll w;
};

int main() {
    int n, m;
    cin >> n >> m;

    vector<Edge> edges(m);
    for (int i = 0; i < m; i++) {
        cin >> edges[i].u >> edges[i].v >> edges[i].w;
    }

    vector<ll> dist(n + 1, INF);
    vector<int> parent(n + 1, -1);

    dist[1] = 0;
    int cycle_node = -1;

    for (int i = 1; i <= n; i++) {
        cycle_node = -1;
        for (auto e : edges) {
            if (dist[e.u] != INF && dist[e.u] + e.w < dist[e.v]) {
                dist[e.v] = dist[e.u] + e.w;
                parent[e.v] = e.u;

                if (i == n)
                    cycle_node = e.v;
            }
        }
    }

    if (cycle_node == -1) {
        cout << "No negative cycle\n";
        return 0;
    }

    int x = cycle_node;
    for (int i = 0; i < n; i++)
        x = parent[x];

    vector<int> cycle;
    int cur = x;
    do {
        cycle.push_back(cur);
        cur = parent[cur];
    } while (cur != x);

    reverse(cycle.begin(), cycle.end());

    for (int v : cycle)
        cout << v << " ";
}
