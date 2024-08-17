#include "secret.h"

#include <bits/stdc++.h>
using namespace std;

const int kMaxN = 2e5 + 5;

struct Node {
    int mins;
    int maks;

    Node(int _mins = -1, int _maks = -1) {
        mins = _mins;
        maks = _maks;
    }

    Node operator +(Node other) const {
        Node ret(mins, maks);
        ret.mins = min(mins, other.mins);
        ret.maks = max(maks, other.maks);
        return ret;
    }
};

vector<int> A;
int M;
int N;

long long psum[kMaxN];
set<int> active;
Node stree[4*kMaxN];

void build(int id, int l, int r) {
    if (l == r) {
        stree[id] = Node(A[l], A[l]);
    } else {
        int m = (l + r) / 2;
        int chld = id << 1;
        build(chld, l, m);
        build(chld+1, m+1, r);
        stree[id] = stree[chld] + stree[chld+1];
    }
}

Node query(int id, int l, int r, int x, int y) {
    if (x <= l && r <= y) {
        return stree[id];
    } else {
        int m = (l + r) / 2;
        int chld = id << 1;
        Node ret(M+1, -(M+1));

        if (x <= m) ret = ret + query(chld, l, m, x, y);
        if (y > m) ret = ret + query(chld+1, m+1, r, x, y);

        return ret;
    }
}

void init(int N, int M, std::vector<int> A) {
    ::M = M;
    ::A = A;
    ::N = N;

    for (int i = 1 ; i < N ; i++) {
        int a = abs(A[i-1] - A[i]);
        psum[i] = psum[i-1] + a;
    }
    build(1, 0, N-1);
}

void toggle(int R) {
    if (active.count(R)) active.erase(R);
    else active.insert(R);
}

long long query(int S, int T) {
    Node lohi = query(1, 0, N-1, S, T);
    auto lowestNext = active.lower_bound(lohi.mins);
    if (lowestNext == active.end() || *lowestNext >= lohi.maks) {
        return psum[T]-psum[S];
    }

    return -1;
}
