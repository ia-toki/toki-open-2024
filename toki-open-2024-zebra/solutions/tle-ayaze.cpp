// only query-ing 1 pair at a time
// very bad for star graph
#include "zebra.h"

#include <bits/stdc++.h>
using namespace std;

const int kMaxN = 505;

vector<pair<int, int>> adj[kMaxN];
vector<int> edgeColor[kMaxN];

void dfs(int now, int prv, int color, vector<int> &colorList) {
    for (auto [nxt, idx] : adj[now]) {
        if (nxt == prv) continue;
        colorList[idx] = color;
        dfs(nxt, now, color^1, colorList);
    }
}

int find_root(int N, std::vector<int> U, std::vector<int> V) {
    for (int i = 0 ; i+1 < N ; i++) {
        adj[U[i]].push_back({V[i], i});
        adj[V[i]].push_back({U[i], i});
    }

    for (int i = 0 ; i < N ; i++) {
        edgeColor[i].resize(N-1);
        dfs(i, i, 0, edgeColor[i]);
    }

    vector<int> candidates(N);
    iota(candidates.begin(), candidates.end(), 0);
    while (candidates.size() > 1) {
        pair<int, int> bestPair = {-1, -1};
        int mins = 1e9;

        for (int i = 0 ; i+1 < N ; i++) {
            for (int j = i+1 ; j+1 < N ; j++) {
                int cntr0 = 0;
                for (int idx : candidates) {
                    if (edgeColor[idx][i] == edgeColor[idx][j]) {
                        cntr0++;
                    }
                }
                int cntr1 = candidates.size() - cntr0;
                int worst = max(cntr1, cntr0);
                if (worst < mins) {
                    mins = worst;
                    bestPair = {i, j};
                }
            }
        }

        int val = is_zebra({bestPair.first, bestPair.second});
        vector<int> newCandidates;
        for (int idx : candidates) {
            if ((edgeColor[idx][bestPair.first] ^ edgeColor[idx][bestPair.second]) == val) {
                newCandidates.push_back(idx);
            }
        }

        candidates = newCandidates;
    }

    return candidates[0];
}
