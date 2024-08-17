#include <bits/stdc++.h>
#include "purwokerto.h"
using namespace std;

const int MOD = 1e9 + 7;

void removeOneFront(deque<pair<int, int>> &dq) {
    while (!dq.empty() && dq.front().first == 1) dq.pop_front();
}

void removeZeroEnd(deque<pair<int, int>> &dq) {
    while (!dq.empty() && dq.back().first == 0) dq.pop_back();
}

void mergeContiguous(deque<pair<int, int>> &dq) {
    deque<pair<int, int>> newDq;
    for (auto [val, cnt] : dq) {
        if (cnt == 0) continue;
        if (!newDq.empty() && newDq.back().first == val) {
            newDq[newDq.size()-1].second += cnt;
        } else {
            newDq.push_back({val, cnt});
        }
    }
    dq = newDq;
}

void simplify(deque<pair<int, int>> &dq) {
    mergeContiguous(dq);
    removeOneFront(dq);
    removeZeroEnd(dq);
}

pair<long long, int> purwokerto(int N, int M, vector<int> T) {
    deque<pair<int, int>> dq;
    for (int v : T) {
        dq.push_back({v, 1});
    }
    simplify(dq);

    long long total = 0;
    int ways = 1;
    for (int i = N ; i >= 1 && !dq.empty() ; i--) {
        for (pair<int, int> &cur : dq) {
            ways = 1ll * ways * cur.second % MOD;
            cur.second -= 1;
        }
        total += 1ll * i * dq.size() / 2;

        simplify(dq);
    }

    return {total, ways};
}
