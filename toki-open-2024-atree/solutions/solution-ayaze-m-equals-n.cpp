#include "copper.h"

#include <bits/stdc++.h>
using namespace std;

const int kMaxL = 2e5 + 5;
const int kMaxN = 2e5 + 5;
const int kMaxM = 2e5 + 5;
const int kLog = 19;

vector<int> buttons;
vector<vector<int>> adj;
vector<vector<pair<int, int>>> sparseTable;
vector<int> depth;

void dfs(int now, int prv) {
    for (int nxt : adj[now]) {
        if (nxt == prv) continue;

        sparseTable[nxt][0].first = now;
        sparseTable[nxt][0].second = buttons[now] < buttons[nxt];
        depth[nxt] = depth[now]+1;
        dfs(nxt, now);
    }
}

void prepare() {
    depth[0] = 0;
    dfs(0, -1);
    sparseTable[0][0] = {0, 0};

    for (int i = 1 ; i < kLog ; i++) {
        for (int j = 0 ; j < static_cast<int>(sparseTable.size()) ; j++) {
            auto [par1, d1] = sparseTable[j][i-1];
            auto [par2, d2] = sparseTable[par1][i-1];
            sparseTable[j][i] = {par2, d1+d2};
        }
    }
}

pair<int, int> query(int u, int v) {
    int d = 0;
    int diff = abs(depth[u]-depth[v]);

    for (int i = 0 ; diff > 0 ; i++) {
        if ((diff & (1 << i)) == 0) continue;
        diff -= (1 << i);

        if (depth[u] > depth[v]) {
            d += sparseTable[u][i].second;
            u = sparseTable[u][i].first;
        } else {
            d += (1 << i);
            d -= sparseTable[v][i].second;
            v = sparseTable[v][i].first;
        }
    }

    if (u == v) return {u, d};

    for (int i = kLog-1 ; i >= 0 ; i--) {
        if (sparseTable[u][i].first == sparseTable[v][i].first) continue;

        d += sparseTable[u][i].second;
        d += (1 << i);
        d -= sparseTable[v][i].second;

        u = sparseTable[u][i].first;
        v = sparseTable[v][i].first;
    }

    d += sparseTable[u][0].second;
    d += (1 - sparseTable[v][0].second);
    return {sparseTable[u][0].first, d};
}

long long min_risk(int N, int M, int L, std::vector<int> U, std::vector<int> V,
                   std::vector<int> A, std::vector<int> S) {
    buttons.resize(N);
    for (int i = 0 ; i < M ; i++) {
        buttons[A[i]] = i;
    }

    adj.resize(N);
    for (int i = 0 ; i+1 < N ; i++) {
        adj[U[i]].push_back(V[i]);
        adj[V[i]].push_back(U[i]);
    }

    sparseTable.resize(N);
    for (int i = 0 ; i < N ; i++) {
        sparseTable[i].resize(kLog);
    }
    depth.resize(N);

    prepare();
    
    long long ret = 0;
    for (int i = 0 ; i < L ; i++) {
        int prv = i ? S[i-1] : 0;
        int cur = S[i];
        auto [_, d] = query(prv, cur);
        ret += d;
    }

    return ret;
}
