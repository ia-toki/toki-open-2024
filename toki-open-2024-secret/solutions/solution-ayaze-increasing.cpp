#include "secret.h"

#include <bits/stdc++.h>
using namespace std;

const int kMaxN = 2e5 + 5;

vector<int> A;
int M;

long long psum[kMaxN];
set<int> active;

void init(int N, int M, std::vector<int> A) {
    ::M = M;
    ::A = A;
    for (int i = 1 ; i < N ; i++) {
        int a = abs(A[i-1] - A[i]);
        int b = M-a;
        psum[i] = psum[i-1] + min(a, b);
    }
}

void toggle(int R) {
    if (active.count(R)) active.erase(R);
    else active.insert(R);
}

long long query(int S, int T) {
    if (active.empty()) {
        return psum[T]-psum[S];
    }

    auto lowestNext = active.lower_bound(A[S]);
    if (lowestNext == active.end() || *lowestNext >= A[T]) {
        return A[T] - A[S];
    }

    // from here on, some falls inside

    // sandwiched
    if (*active.begin() < A[S] || *active.rbegin() >= A[T]) {
        return -1;
    }

    auto it = upper_bound(A.begin(), A.end(), *lowestNext);
    int villageNext = *it;
    // sandwiched, not everything is contained in between a segment
    if (*active.rbegin() >= villageNext) {
        return -1;
    }

    // safe
    int villagePrev = *(--it);
    int ret = 0;
    ret += (villagePrev - A[S]);
    ret += (M - abs(villageNext-villagePrev));
    ret += (A[T] - villageNext);

    return ret;
}
