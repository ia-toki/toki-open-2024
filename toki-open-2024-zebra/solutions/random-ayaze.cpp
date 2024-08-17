// randomly add edge if it can distinguish more nodes
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
    // mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
    srand(11052024);
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
        vector<int> bestIndexes;
        vector<pair<int, int>> bestAsd;
        int bestWorst = 1e9;

        for (int trial = 0 ; trial < 500 ; trial++) {
            vector<pair<int, int>> asd;
            for (int p : candidates) asd.push_back({p, 0});
            int cntr0 = candidates.size();
            vector<int> edgeIndex = {rand() % (N-1)};
            // vector<int> edgeIndex = {uniform_int_distribution<int>(0, N-2)(rng)};

            while (1) {
                int newEdgeIndex = rand() % (N-1);
                // int newEdgeIndex = uniform_int_distribution<int>(0, N-2)(rng);
                int newCntr0 = cntr0;
                vector<pair<int, int>> newAsd = asd;

                for (int i = 0 ; i < newAsd.size() ; i++) {
                    if (newAsd[i].second == 1) continue;
                    int idx = newAsd[i].first;
                    if (edgeColor[idx][newEdgeIndex] != edgeColor[idx][edgeIndex[0]]) {
                        newAsd[i].second = 1;
                        newCntr0--;
                    }
                }

                if (newCntr0 == 0) {
                    continue;
                }
                if (max(cntr0, (int)candidates.size()-cntr0) <= max(newCntr0, (int)candidates.size()-newCntr0)) {
                    break;
                }

                asd = newAsd;
                edgeIndex.push_back(newEdgeIndex);
                cntr0 = newCntr0;
                // printf("cand = %d %d\n", candidates[0], candidates[1]);
                // printf("candidates size -- %d, new edge index -- %d, edge count -- %d, cntr0 -- %d\n", (int)candidates.size(), (int)newEdgeIndex, (int)edgeIndex.size(), cntr0);
            }

            if (max(cntr0, (int)candidates.size()-cntr0) < bestWorst) {
                bestWorst = max(cntr0, (int)candidates.size()-cntr0);
                bestIndexes = edgeIndex;
                bestAsd = asd;
            }
        }

        int val = is_zebra(bestIndexes);
        vector<int> newCandidates;
        for (auto [p, v] : bestAsd) {
            if (v == val) newCandidates.push_back(p);
        }

        candidates = newCandidates;
    }

    return candidates[0];
}
