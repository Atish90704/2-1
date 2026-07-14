#include <bits/stdc++.h>
using namespace std;

struct Edge {
    int u, v, w;
};

struct DSU {
    vector<int> parent, rank;

    DSU(int n) {
        parent.resize(n);
        rank.resize(n, 0);
        for (int i=0; i < n; i++)
            parent[i]=i;
    }

    int find(int x) {
        if (parent[x]!=x)
            parent[x]=find(parent[x]);
        return parent[x];
    }

    void unite(int a, int b) {
        a = find(a);
        b=find(b);
        if (a != b) {
            if (rank[a]<rank[b])
                parent[a]=b;
            else if (rank[a]>rank[b])
                parent[b] = a;
            else {
                parent[b] = a;
                rank[a]++;
            }
        }
    }
};

int main() {
    int N, M;
    cin >> N >> M;

    vector<Edge> edges(M);
    for (int i = 0; i < M; i++) {
        cin >> edges[i].u >> edges[i].v >> edges[i].w;
    }

    sort(edges.begin(), edges.end(),[](Edge &a, Edge &b) {
             return a.w < b.w;
         });

    DSU dsu(N);
    int totW=0;
    vector<pair<int,int>> mstEdges;

    for (auto &e : edges) {
        if (dsu.find(e.u) != dsu.find(e.v)) {
            dsu.unite(e.u, e.v);
            totW += e.w;
            mstEdges.push_back({e.u, e.v});
            if (mstEdges.size()==N - 1) break;
        }
    }

    cout<<"Total weight "<<totW<<"\n";
    for (auto &e: mstEdges) {
        cout<<e.first << " "<<e.second<<"\n";
    }

    return 0;
}
