#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <set>
#include <cmath>

using namespace std;

typedef long long ll;

struct Edge{
    int from;
    int to;
    ll weight;
};

int main(){
    int n, m;
    cin >> n >> m;
    vector<Edge> edges(m);
    for(int i=0; i < m; i++) {
        cin >> edges[i].from >> edges[i].to >> edges[i].weight;
    }

    vector<ll> dist(n + 1, LLONG_MAX);
    vector<int> parent(n + 1, -1);

    int cycle_node=-1;
    //Bellman-Ford
    for(int start=1; start<=n && cycle_node == -1; start++) {
        if (dist[start] != LLONG_MAX) continue;
        dist[start]=0;
        for(int i = 1; i<=n; i++) {
            for (int j=0; j < m; j++) {
                int u = edges[j].from;
                int v=edges[j].to;
                ll w=edges[j].weight;

                if(dist[u] != LLONG_MAX && dist[u] + w < dist[v]) {
                    dist[v]=dist[u] + w;
                    parent[v] = u;

                    if(i==n) {
                        cycle_node=v;
                        break;
                    }
                }
            }
            if(cycle_node != -1) break;
        }
    }
    //
    if (cycle_node == -1) {
        cout << -1 << endl;
        return 0;
    }

    int x=cycle_node;
    for(int i = 0; i < n; i++) {
        x=parent[x];
    }

    vector<int> cycle;
    int cur=x;
    do{
        cycle.push_back(cur);
        cur=parent[cur];
    } while(cur != x);

    reverse(cycle.begin(), cycle.end());

      for(int v : cycle) {
        cout << v << " ";
      }


    cout << endl;

    return 0;
}
