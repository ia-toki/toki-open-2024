#include "copper.h"

#include <bits/stdc++.h>
using namespace std;

long long min_risk(int N, int M, int L, std::vector<int> U, std::vector<int> V,
                   std::vector<int> A, std::vector<int> S) {
    
    vector<int> buttons(N+1, 0);
    for (int i = 0 ; i < M ; i++) {
        buttons[A[i]+1] = i+1;
    }

    vector<int> psum(N+1, 0);
    for (int i = 1 ; i < N ; i++) {
        psum[i] = psum[i-1];
        if (buttons[i] > buttons[i+1]) psum[i] += 1;
    }

    long long ret = 0;
    for (int i = 0 ; i < L ; i++) {
        int prv = i ? S[i-1] : 0;
        int cur = S[i];
        prv++; cur++;

        if (prv < cur) {
            ret += (psum[cur-1] - psum[prv-1]);
        } else {
            ret += (prv - cur);
            ret -= (psum[prv-1] - psum[cur-1]);
        }
    }

    return ret;
}
