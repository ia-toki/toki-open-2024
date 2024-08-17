#include "copper.h"

#include <bits/stdc++.h>
using namespace std;

// if we can solve the path version of this problem, then we can use HLD to solve the full version
// can I implement it? perhaps. will I? nein.

const int kMaxL = 2e5 + 5;
const int kMaxN = 2e5 + 5;
const int kMaxM = 2e5 + 5;
const int kLog = 19;

struct SparseTable {
    vector<vector<pair<int, int>>> table;
    int n;

    SparseTable() {}

    SparseTable(vector<pair<int, int>> parents) {
        n = parents.size();
        table.resize(n);
        for (int i = 0 ; i < n ; i++) {
            table[i].resize(kLog);
            table[i][0] = parents[i];
        }

        init();
    }

    void init() {
        for (int i = 1 ; i < kLog ; i++) {
            for (int j = 0 ; j < n ; j++) {
                auto [par1, d1] = table[j][i-1];
                auto [par2, d2] = table[par1][i-1];
                table[j][i] = {par2, d1+d2};
            }
        }
    }

    pair<int, int> query(int v, int k) {
        int cur = v;
        int ret = 0;

        for (int i = 0 ; k > 0 ; i++) {
            if ((k & (1 << i)) == 0) continue;

            k -= (1 << i);
            ret += table[cur][i].second;
            cur = table[cur][i].first;
        }

        return {cur, ret};
    }
};

long long min_risk(int N, int M, int L, std::vector<int> U, std::vector<int> V,
                   std::vector<int> A, std::vector<int> S) {
    
    vector<vector<int>> buttons(N);
    for (int i = 0 ; i < M ; i++) {
        buttons[A[i]].push_back(i);
    }

    SparseTable toLeft, toRight;
    
    // greedy move: try to use the smallest, larger button if possible. 
    // if it doesn't exist, use the smallest button

    // construct toRight
    {
        vector<pair<int, int>> parents(M);
        for (int i = 0 ; i+1 < N ; i++) {
            int j = 0;
            for (int b : buttons[i]) {
                while (j < static_cast<int>(buttons[i+1].size()) && buttons[i+1][j] <= b) j++;

                if (j < static_cast<int>(buttons[i+1].size())) {
                    parents[b] = {buttons[i+1][j], 0};
                } else {
                    parents[b] = {buttons[i+1][0], 1};
                }
            }
        }
        for (int b : buttons[N-1]) {
            parents[b] = {b, 0};
        }
        toRight = SparseTable(parents);
    }

    // construct toLeft
    {
        vector<pair<int, int>> parents(M);
        for (int i = N-1 ; i-1 >= 0 ; i--) {
            int j = 0;
            for (int b : buttons[i]) {
                while (j < static_cast<int>(buttons[i-1].size()) && buttons[i-1][j] <= b) j++;

                if (j < static_cast<int>(buttons[i-1].size())) {
                    parents[b] = {buttons[i-1][j], 0};
                } else {
                    parents[b] = {buttons[i-1][0], 1};
                }
            }
        }
        for (int b : buttons[0]) {
            parents[b] = {b, 0};
        }
        toLeft = SparseTable(parents);
    }

    long long ret = 0;
    int lastButton = 0;

    for (int i = 0 ; i < L ; i++) {
        int prv = i ? S[i-1] : 0;
        int cur = S[i];

        if (prv < cur) {
            auto [nextButton, d] = toRight.query(lastButton, cur-prv);
            lastButton = nextButton;
            ret += d;
        } else {
            auto [nextButton, d] = toLeft.query(lastButton, prv-cur);
            lastButton = nextButton;
            ret += d;
        }
    }

    return ret;
}
