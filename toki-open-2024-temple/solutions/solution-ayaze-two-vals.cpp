#include "temple.h"

#include <bits/stdc++.h>
using namespace std;

long long min_weight(int N, int M, int K,
                     std::vector<int> U, std::vector<int> V, std::vector<int> W,
                     std::vector<int> P) {
    int mins = *min_element(W.begin(), W.end());
    int maks = *max_element(W.begin(), W.end());

    vector<vector<pair<int, int>>> adj(N);
    for (int i = 0 ; i < M ; i++) {
        adj[U[i]].push_back({V[i], W[i]});
        adj[V[i]].push_back({U[i], W[i]});
    }

    vector<bool> visited(N, false);
    queue<int> q;
    q.push(0); visited[0] = 1;
    while (!q.empty()) {
        int now = q.front();
        q.pop();
        for (auto [nxt, w] : adj[now]) {
            if (w != mins) continue;
            if (visited[nxt]) continue;
            visited[nxt] = true;
            q.push(nxt);
        }
    }

    // we can just use min weight
    for (int p : P) {
        if (visited[p]) {
            return mins;
        }
    }

    // we can remove any min weight used in the path
    for (int p : P) {
        for (auto [_, w] : adj[p]) {
            if (w == maks) {
                return maks;
            }
        }
    }

    // otherwise we are forced to keep both
    return mins + maks;
}
