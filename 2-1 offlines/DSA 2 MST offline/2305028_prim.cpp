#include<bits/stdc++.h>
using namespace std;

typedef pair<int, int> pii;

int main()
{
    int N,M;
    cin>>N>>M;
    vector<vector<pii>> adj(N);

    for(int i=0;i<M;i++){
        int u, v, w;
        cin>>u>>v>>w;
        adj[u].push_back({v, w});
        adj[v].push_back({u, w});
    }

    int root;
    cin>>root;
    vector<int> key(N, INT_MAX), parent(N, -1);
    vector<bool> inMST(N, false);
    priority_queue<pii, vector<pii>, greater<pii>> pq;

    key[root] = 0;
    int totWeight = 0;

    pq.push({0, root});

    while(!pq.empty()) {
        auto[w, u] = pq.top();
        pq.pop();

        if(!inMST[u]) {
            inMST[u] = true;
            totWeight+=w;

            for(auto[v, weight] : adj[u]){
                if(!inMST[v] && weight<key[v]) {
                    key[v] = weight;
                    parent[v] = u;
                    pq.push({key[v], v});
                }
            }
        }
    }

    cout<<"Total weight: "<<totWeight<<endl<<"Root node: "<<root<<endl;
    for(int v=0;v<N; v++) {
        if (parent[v] != -1) {
            cout << parent[v] << " " << v << "\n";
        }
    }
    return 0;
}
