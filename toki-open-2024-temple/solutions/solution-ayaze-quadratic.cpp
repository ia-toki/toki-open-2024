#include "temple.h"

#include <bits/stdc++.h>
using namespace std;

long long encode(int N, int node, int lastWeight) {
    return node + 1ll * lastWeight * N;
}

// pair<int, int> decode(int N, long long vertex) {
//     return {vertex % N, vertex / N};
// }

long long min_weight(int N, int M, int K,
                     std::vector<int> U, std::vector<int> V, std::vector<int> W,
                     std::vector<int> P) {
    vector<vector<pair<int, int>>> adj(N);
    for (int i = 0 ; i < M ; i++) {
        adj[U[i]].push_back({V[i], W[i]});
        adj[V[i]].push_back({U[i], W[i]});
    }

    unordered_map<long long, long long> dist;
    priority_queue<pair<long long, pair<int, int>>> pq;
    for (int p : P) {
        dist[encode(N, p, 0)] = 0;
        pq.push({0, {p, 0}});
    }

    while (!pq.empty()) {
        long long d = -pq.top().first;
        pair<int, int> vertex = pq.top().second;
        pq.pop();

        if (dist[encode(N, vertex.first, vertex.second)] != d) continue;
        if (vertex.first == 0) return d;
        
        for (auto [nxt, w] : adj[vertex.first]) {
            pair<int, int> nextVert = {nxt, vertex.second};
            long long newD = d;

            if (w > nextVert.second) {
                nextVert.second = w;
                newD += w;
            }

            long long encodedNextVert = encode(N, nextVert.first, nextVert.second);
            if (!dist.count(encodedNextVert) || dist[encodedNextVert] > newD) {
                dist[encodedNextVert] = newD;
                pq.push({-newD, nextVert});
            }
        }
    }

    assert(false);
}
