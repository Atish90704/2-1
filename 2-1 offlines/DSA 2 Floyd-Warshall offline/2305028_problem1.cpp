#include<bits/stdc++.h>
using namespace std;

typedef long long ll;
const ll INF = 1e18;

int main() {
    int n, m, q;
    cin >> n >> m >> q;
    vector<vector<ll>> dist(n + 1, vector<ll>(n + 1, INF));
    
    int a, b;
    ll c;
    for(int i=0;i<m;i++) {
        cin >> a >> b >> c;
        dist[a][b] = min(dist[a][b], c);
        dist[b][a] = min(dist[b][a], c);
    }

    for (int i = 1 ; i <= n ; i ++) {
        dist[i][i] = 0;
    }

    for(int k = 1;k<=n;k++) 
        for(int i=1;i<=n;i++) 
            for(int j=1;j<=n;j++) 
                if(dist[i][k]<INF && dist[k][j]<INF) 
                    dist[i][j] = min(dist[i][j], dist[i][k]+dist[k][j]);

    int x, y;
    for(int i=0;i<q;i++){
        cin>>x>>y;
        if(dist[x][y]!=INF) cout<<dist[x][y]<<endl;
        else cout <<-1<<endl;
    }

    return 0;
}