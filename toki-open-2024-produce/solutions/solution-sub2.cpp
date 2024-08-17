#include <bits/stdc++.h>
#include "purwokerto.h"

using namespace std;

#define mp make_pair
#define fr first
#define sc second

const int MOD = 1e9 + 7;
const long long INF = 1e18;

pair<long long, int> purwokerto(int N, int M, vector<int> T)
{
	long long dp[1 << N];
	int dp2[1 << N];
	for(int i = 0; i < 1ll << N; i++)
	{
		dp[i] = -INF;
	}
	dp[0] = 0;
	dp2[0] = 1;
	
	for(int i = 0; i < M; i++)
	{
		if(T[i] == 0)
		{
			for(int j = (1ll << N) - 1; j >= 0; j--)
			{
				for(int k = 0; k < N; k++)
				{
					if(!(j >> k & 1))
					{
						int l = j | 1 << k;
						if(dp[j] > dp[l])
						{
							dp[l] = dp[j];
							dp2[l] = 0;
						}
						if(dp[j] == dp[l])
						{
							dp2[l] = (dp2[l] + dp2[j]) % MOD;
						}
					}
				}
			}
		}
		else
		{
			for(int j = 0; j < 1 << N; j++)
			{
				for(int k = 0; k < N; k++)
				{
					if(j >> k & 1)
					{
						int l = j ^ 1 << k;
						if(dp[j] + k + 1 > dp[l])
						{
							dp[l] = dp[j] + k + 1;
							dp2[l] = 0;
						}
						if(dp[j] + k + 1 == dp[l])
						{
							dp2[l] = (dp2[l] + dp2[j]) % MOD;
						}
					}
				}
			}
		}
	}
	
	return {dp[0], dp2[0]};
}
