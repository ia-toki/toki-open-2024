#include <bits/stdc++.h>
#include "purwokerto.h"
using namespace std;

const int MOD = 1e9 + 7;
const long long INF = 4e18;

pair<long long, int> purwokerto(int N, int M, vector<int> T) {
    int nPow = 1 << N;
    vector<vector<pair<long long, int>>> dp(M+1, vector<pair<long long, int>>(nPow));
    dp[M][0] = {0, 1};
    for (int i = 1 ; i < nPow ; i++) {
        dp[M][i] = {-INF, 0};
    }

    for (int i = M-1 ; i >= 0 ; i--) {
        for (int mask = 0 ; mask < nPow ; mask++) {
            dp[i][mask] = dp[i+1][mask]; // not doing anything

            for (int j = 0 ; j < N ; j++) {
                if (T[i] == 0 && (mask & (1 << j)) == 0) { // buy
                    int nMask = mask ^ (1 << j);

                    if (dp[i][mask].first == dp[i+1][nMask].first) {
                        dp[i][mask].second += dp[i+1][nMask].second;
                        dp[i][mask].second %= MOD;
                    } else if (dp[i][mask].first < dp[i+1][nMask].first) {
                        dp[i][mask] = dp[i+1][nMask];
                    }
                } else if (T[i] == 1 && (mask & (1 << j)) != 0) { // sell
                    int nMask = mask ^ (1 << j);

                    if (dp[i][mask].first == dp[i+1][nMask].first + (j+1)) {
                        dp[i][mask].second += dp[i+1][nMask].second;
                        dp[i][mask].second %= MOD;
                    } else if (dp[i][mask].first < dp[i+1][nMask].first + (j+1)) {
                        dp[i][mask] = dp[i+1][nMask];
                        dp[i][mask].first += (j+1);
                    }
                }
            }
        }
    }

    return dp[0][0];
}
