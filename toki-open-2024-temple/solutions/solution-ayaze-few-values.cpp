#include "temple.h"

#include <bits/stdc++.h>
using namespace std;

long long min_weight(int N, int M, int K,
                     std::vector<int> U, std::vector<int> V, std::vector<int> W,
                     std::vector<int> P) {
    vector<vector<pair<int, int>>> adj(N);
    for (int i = 0 ; i < M ; i++) {
        adj[U[i]].push_back({V[i], W[i]});
        adj[V[i]].push_back({U[i], W[i]});
    }
    set<int> setOfP(P.begin(), P.end());

    set<pair<int, vector<int>>> seen;
    queue<pair<int, vector<int>>> q;
    q.push({0, {}});
    seen.insert({0, vector<int>()});

    long long ret = 4e18;

    while (!q.empty()) {
        auto [now, weights] = q.front();
        q.pop();

        if (setOfP.count(now)) {
            ret = min(ret, accumulate(weights.begin(), weights.end(), 0ll));
        }

        for (auto [nxt, w] : adj[now]) {
            vector<int> nWeights = weights;
            while (!nWeights.empty() && nWeights.back() <= w) {
                nWeights.pop_back();
            }
            nWeights.push_back(w);

            if (!seen.count({nxt, nWeights})) {
                seen.insert({nxt, nWeights});
                q.push({nxt, nWeights});
            }
        }
    }

    return ret;
}
