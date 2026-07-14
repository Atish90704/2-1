#include <bits/stdc++.h>
using namespace std;

const double INF = 1e18;

int main(){
    int n;
    cin>>n;

    unordered_map<string, int> id;
    for(int i = 0; i < n; i++){
        string s;
        cin>>s;
        id[s]= i;
    }

    vector<vector<double>> dist(n, vector<double>(n, INF));

    for (int i = 0; i < n; i++)
        dist[i][i] = 0.0;

    int m;
    cin>>m;
    while (m--) {
        string a, b;
        double r;
        cin >> a >> r >> b;
        int u = id[a], v = id[b];
        dist[u][v] = min(dist[u][v], -log(r));
    }

    // Floyd–Warshall
    for(int k = 0; k < n; k++)
        for(int i = 0; i < n; i++)
            for(int j = 0; j < n; j++)
                if(dist[i][k] < INF && dist[k][j] < INF)
                    dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);

    for (int i = 0; i < n; i++) {
        if (dist[i][i] < 0) {
            cout << "Yes"<<endl;
            return 0;
        }
    }

    cout << "No"<<endl;
    return 0;
}
