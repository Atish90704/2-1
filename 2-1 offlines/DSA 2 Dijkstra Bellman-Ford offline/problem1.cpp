
#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <set>
#include <cmath>
#include <queue>
#include <tuple>

using namespace std;

typedef long long ll;
const ll INF=1e18;

int main() {
    int n, m;
    cin >> n >> m;

    // graph[u]=list of (v, cost)
    vector<vector<pair<int, ll>>> graph(n + 1);

    for (int i = 0; i < m; i++) {
        int a, b;
        ll c;
        cin >> a >> b >> c;
        graph[a].push_back(make_pair(b, c));
    }

    //
    vector<vector<ll>> dist(n + 1, vector<ll>(2, INF));

    // priority queue -- (cost, node, coupon_used)
    priority_queue<tuple<ll, int, int>, vector<tuple<ll, int, int>>, greater<tuple<ll, int, int>>> pq;

    dist[1][0]=0;
    pq.push(make_tuple(0, 1, 0));

    while (!pq.empty()) {
        ll cost = get<0>(pq.top());
        int u  =get<1>(pq.top());
        int used=get<2>(pq.top());
        pq.pop();

         if (cost > dist[u][used]) {
            continue;
         }

        for (int i=0; i < graph[u].size(); i++) {
            int v=graph[u][i].first;
            ll w =graph[u][i].second;
            // Case 1: coupon chara
            if (dist[v][used] > cost + w) {
                dist[v][used] = cost + w;
                pq.push(make_tuple(dist[v][used], v, used));
            }

            // Case 2: cooupon soho
            if (used == 0) {
                ll discounted_cost = w / 2;
                if (dist[v][1] > cost + discounted_cost) {
                    dist[v][1]=cost + discounted_cost;
                    pq.push(make_tuple(dist[v][1], v, 1));
                }
            }
        }
    }

    cout << min(dist[n][0], dist[n][1]) << endl;

    return 0;
}
