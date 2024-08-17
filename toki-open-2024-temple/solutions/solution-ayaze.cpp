#include "temple.h"

#include <bits/stdc++.h>
using namespace std;

const int kMaxN = 4e5 + 5;
const long long INF = 4e18;

struct Node {
    long long lazy;
    long long mins;

    Node(long long _mins = INF) {
        mins = _mins;
        lazy = INF;
    }

    Node operator +(Node other) const {
        Node ret;
        ret.mins = min(mins, other.mins);
        ret.lazy = INF;
        return ret;
    }

    void update(long long upd) {
        if (upd == INF) return;
        mins = min(mins, upd);
        lazy = min(lazy, upd);
    }
};

vector<int> adjUncompressed[kMaxN];
int pset[kMaxN];
int tin[kMaxN], tout[kMaxN];
Node stree[4*kMaxN];

int finds(int x) {
    return pset[x] == x ? x : pset[x] = finds(pset[x]);
}

void join(int u, int v, int intm) {
    adjUncompressed[intm].push_back(u);
    adjUncompressed[intm].push_back(v);
    pset[u] = intm;
    pset[v] = intm;
}

void dfs(int now, int prv) {
    static int cntr = -1;
    tin[now] = ++cntr;
    for (int nex : adjUncompressed[now]) {
        dfs(nex, now);
    }
    tout[now] = cntr;
}

void build(int id, int l, int r) {
    if (l == r) {
        stree[id] = Node();
    } else {
        int m = (l + r) / 2;
        int chld = id << 1;
        build(chld, l, m);
        build(chld+1, m+1, r);
        stree[id] = stree[chld] + stree[chld+1];
    }
}

void propagate(int id) {
    int chld = id << 1;
    stree[chld].update(stree[id].lazy);
    stree[chld+1].update(stree[id].lazy);
    stree[id].lazy = INF;
}

void update(int id, int l, int r, int x, int y, long long val) {
    if (x <= l && r <= y) {
        stree[id].update(val);
    } else {
        int m = (l + r) / 2;
        int chld = id << 1;
        propagate(id);

        if (x <= m) update(chld, l, m, x, y, val);
        if (y > m)  update(chld+1, m+1, r, x, y, val);
        stree[id] = stree[chld] + stree[chld+1];
    }
}

Node query(int id, int l, int r, int x, int y) {
    if (x <= l && r <= y) {
        return stree[id];
    } else {
        int m = (l + r) / 2;
        int chld = id << 1;
        propagate(id);

        Node ret;
        if (x <= m) ret = ret + query(chld, l, m, x, y);
        if (y > m) ret = ret + query(chld+1, m+1, r, x, y);

        return ret;
    }
}

long long min_weight(int N, int M, int K,
                     std::vector<int> U, std::vector<int> V, std::vector<int> W,
                     std::vector<int> P) {
    vector<tuple<int, int, int>> sortedEdges;
    for (int i = 0 ; i < M ; i++) {
        sortedEdges.push_back({U[i], V[i], W[i]});
    }

    sort(sortedEdges.begin(), sortedEdges.end(), [](
        tuple<int, int, int> lhs, 
        tuple<int, int, int> rhs) {
        return get<2>(lhs) < get<2>(rhs);
    });

    // construct tree
    iota(pset, pset + kMaxN, 0);
    int cntr = 0;
    for (int i = 0 ; i < M ;) {
        int j = i;
        while (j < M && get<2>(sortedEdges[j]) == get<2>(sortedEdges[i])) {
            auto [u, v, _] = sortedEdges[j];
            int uu = finds(u);
            int vv = finds(v);
            if (uu != vv) {
                cntr++;
                int newNode = cntr + N;
                join(uu, vv, newNode);
            }
            j++;
        }

        i = j;
    }

    // preparation
    int root = N+cntr;
    dfs(root, -1);
    build(1, tin[root], tout[root]);
    for (int p : P)
        update(1, tin[root], tout[root], tin[p], tin[p], 0);

    // shortest path
    iota(pset, pset + kMaxN, 0);
    cntr = 0;
    for (int i = 0 ; i < M ;) {
        int j = i;
        vector<tuple<int, int, int>> pendings;
        map<int, long long> mv;
        while (j < M && get<2>(sortedEdges[j]) == get<2>(sortedEdges[i])) {
            auto [u, v, w] = sortedEdges[j];
            int uu = finds(u);
            int vv = finds(v);
            if (uu != vv) {
                cntr++;
                int newNode = cntr + N;
                join(uu, vv, newNode);
            }
            mv[u] = query(1, tin[root], tout[root], tin[u], tin[u]).mins + w;
            mv[v] = query(1, tin[root], tout[root], tin[v], tin[v]).mins + w;
            j++;
        }

        for (auto [k, v] : mv) {
            int kk = finds(k);
            if (v < INF) {
                update(1, tin[root], tout[root], tin[kk], tout[kk], v);
            }
        }

        i = j;
    }

    Node retNode = query(1, tin[root], tout[root], tin[0], tin[0]);
    return retNode.mins;
}
