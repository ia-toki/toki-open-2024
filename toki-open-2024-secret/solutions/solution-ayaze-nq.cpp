#include "secret.h"

#include <bits/stdc++.h>
using namespace std;

const int kInf = 2e9;

set<int> active;
vector<int> A;
int M;

void init(int N, int M, std::vector<int> A) {
    ::M = M;
    ::A = A;
}

void toggle(int R) {
    if (active.count(R)) active.erase(R);
    else active.insert(R);
}

long long query(int S, int T) {
    long long ret = 0;

    for (int i = S ; i < T ; i++) {
        int a = A[i];
        int b = A[i+1];

        int vals[2] = {abs(a-b), M-abs(a-b)};
        {
            auto it = active.lower_bound(min(a, b));
            if (it != active.end() && *it < max(a, b)) vals[0] = kInf;
        }
        if (!active.empty() && (*(active.begin()) < min(a, b) || *(active.rbegin()) >= max(a, b))) {
            vals[1] = kInf;
        }

        int cur = min(vals[0], vals[1]);
        if (cur == kInf) return -1;
        ret += cur;
    }

    return ret;
}
