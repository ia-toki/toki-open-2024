#include "secret.h"

#include <bits/stdc++.h>
using namespace std;

const int kMaxN = 2e5 + 5;

long long psum[kMaxN];

void init(int N, int M, std::vector<int> A) {
    for (int i = 1 ; i < N ; i++) {
        int a = abs(A[i-1] - A[i]);
        int b = M-a;
        psum[i] = psum[i-1] + min(a, b);
    }
}

void toggle(int R) {
    return;
}

long long query(int S, int T) {
    return psum[T]-psum[S];
}
