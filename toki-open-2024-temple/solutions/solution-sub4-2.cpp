#include <bits/stdc++.h>
#include "temple.h"

using namespace std;

#define mp make_pair
#define fr first
#define sc second

const int MXN = 1e3;
const long long INF = 1e18;

long long dis[MXN + 69], visC, lastVis[MXN + 69];
vector<pair<int, int>> adj[MXN + 69];
priority_queue<pair<long long, int>> pq;

void dfs(int u, int curW, long long curD)
{
	lastVis[u] = visC;
	
	if(curD < dis[u])
	{
		pq.push({-curD, u});
		dis[u] = curD;
	}
	
	for(int i = 0; i < (int)adj[u].size(); i++)
	{
		int v, w;
		v = adj[u][i].fr;
		w = adj[u][i].sc;
		
		if(w <= curW && lastVis[v] < visC)
		{
			dfs(v, curW, curD);
		}
	}
}

long long min_weight(int N, int M, int K, vector<int> U, vector<int> V, vector<int> W, vector<int> P)
{
	for(int i = 0; i < M; i++)
	{
		adj[U[i]].push_back({V[i], W[i]});
		adj[V[i]].push_back({U[i], W[i]});
	}
	
	for(int i = 0; i < N; i++)
	{
		dis[i] = INF;
	}
	for(int i = 0; i < K; i++)
	{
		pq.push({0, P[i]});
		dis[P[i]] = 0;
	}
	
	while(!pq.empty())
	{
		long long d;
		int u;
		d = -pq.top().fr;
		u = pq.top().sc;
		pq.pop();
		
		if(d > dis[u])
		{
			continue;
		}
		
		for(int i = 0; i < (int)adj[u].size(); i++)
		{
			int v, w;
			v = adj[u][i].fr;
			w = adj[u][i].sc;
			
			visC++;
			dfs(v, w, d + w);
		}
	}
	
	return dis[0];
}
