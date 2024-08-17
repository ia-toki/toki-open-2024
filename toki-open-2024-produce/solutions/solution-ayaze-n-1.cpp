#include <bits/stdc++.h>
#include "purwokerto.h"
using namespace std;

const int MOD = 1e9 + 7;
const long long INF = 4e18;

pair<long long, int> purwokerto(int N, int M, vector<int> T) {
    vector<vector<pair<long long, int>>> dp(M+1, vector<pair<long long, int>>(2));
    dp[M][0] = {0, 1};
    dp[M][1] = {-INF, 0};

    for (int i = M-1 ; i >= 0 ; i--) {
        // not doing anything
        dp[i][0] = dp[i+1][0];
        dp[i][1] = dp[i+1][1];

        if (T[i] == 0) {
            // buy
            if (dp[i][0].first == dp[i+1][1].first) {
                dp[i][0].second += dp[i+1][1].second;
                dp[i][0].second %= MOD;
            } else if (dp[i][0].first < dp[i+1][1].first) {
                dp[i][0] = dp[i+1][1];
            }
        }
        if (T[i] == 1) {
            // sell
            if (dp[i][1].first == dp[i+1][0].first+1) {
                dp[i][1].second += dp[i+1][0].second;
                dp[i][1].second %= MOD;
            } else if (dp[i][1].first < dp[i+1][0].first+1) {
                dp[i][1] = dp[i+1][0];
                dp[i][1].first += 1;
            }
        }
    }

    return dp[0][0];
}
