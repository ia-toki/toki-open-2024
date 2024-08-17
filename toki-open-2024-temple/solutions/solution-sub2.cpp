#include <bits/stdc++.h>
#include "temple.h"

using namespace std;

const int MXN = 1e5;
const int INF = 1e9;

int dsu[MXN + 69];
bool isExit[MXN + 69];

int unionFind(int x)
{
	if(dsu[x] != x)
	{
		dsu[x] = unionFind(dsu[x]);
	}
	return dsu[x];
}

long long min_weight(int N, int M, int K, vector<int> U, vector<int> V, vector<int> W, vector<int> P)
{
	int mn, mx;
	mn = INF;
	mx = -INF;
	for(int i = 0; i < M; i++)
	{
		mn = min(mn, W[i]);
		mx = max(mx, W[i]);
	}
	
	for(int i = 0; i < N; i++)
	{
		dsu[i] = i;
	}
	for(int i = 0; i < M; i++)
	{
		if(W[i] == mn)
		{
			dsu[unionFind(V[i])] = unionFind(U[i]);
		}
	}
	
	for(int i = 0; i < K; i++)
	{
		if(unionFind(P[i]) == unionFind(0))
		{
			return mn;
		}
	}
	
	for(int i = 0; i < N; i++)
	{
		isExit[i] = false;
	}
	for(int i = 0; i < K; i++)
	{
		isExit[P[i]] = true;
	}
	for(int i = 0; i < M; i++)
	{
		if(W[i] == mx && (isExit[U[i]] || isExit[V[i]]))
		{
			return mx;
		}
	}
	
	return mn + mx;
}
