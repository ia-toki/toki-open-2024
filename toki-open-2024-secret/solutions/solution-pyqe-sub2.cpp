#include <bits/stdc++.h>
#include "secret.h"

using namespace std;

#define mp make_pair
#define fr first
#define sc second

const int MXN = 1e5;
const int INF = 2e9;

int N, M, A[MXN + 69];
multiset<int> blockedRoads;

void init(int curN, int curM, vector<int> curA)
{
	N = curN;
	M = curM;
	for(int i = 0; i < N; i++)
	{
		A[i] = curA[i];
	}
	
	blockedRoads.insert(-INF);
	blockedRoads.insert(INF);
}

void toggle(int R)
{
	if(blockedRoads.find(R) == blockedRoads.end())
	{
		blockedRoads.insert(R);
	}
	else
	{
		blockedRoads.erase(R);
	}
}

long long query(int S, int T)
{
	long long ans = 0;
	for(int i = S; i < T; i++)
	{
		int minA, maxA, dis;
		minA = min(A[i], A[i + 1]);
		maxA = max(A[i], A[i + 1]);
		dis = maxA - minA;
		
		int mn = INF;
		if(*blockedRoads.lower_bound(minA) >= maxA)
		{
			mn = min(mn, dis);
		}
		if(*blockedRoads.lower_bound(0) >= minA && *prev(blockedRoads.upper_bound(M - 1)) < maxA)
		{
			mn = min(mn, M - dis);
		}
		
		if(mn == INF)
		{
			return -1;
		}
		ans += mn;
	}
	return ans;
}
