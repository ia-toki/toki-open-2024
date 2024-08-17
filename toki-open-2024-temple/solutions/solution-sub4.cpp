#include <bits/stdc++.h>
#include "temple.h"

using namespace std;

#define mp make_pair
#define fr first
#define sc second

const int MXN = 1e3, MXM = 2e3;
const long long INF = 1e18;

long long dis[MXN + 69][MXM + 69];
vector<pair<int, int>> adj[MXN + 69];
priority_queue<pair<long long, pair<int, int>>> pq;

long long min_weight(int N, int M, int K, vector<int> U, vector<int> V, vector<int> W, vector<int> P)
{
	W.push_back(0);
	
	for(int i = 0; i < M; i++)
	{
		adj[U[i]].push_back({V[i], i});
		adj[V[i]].push_back({U[i], i});
	}
	
	for(int i = 0; i < N; i++)
	{
		for(int j = 0; j <= M; j++)
		{
			dis[i][j] = INF;
		}
	}
	for(int i = 0; i < K; i++)
	{
		pq.push({0, {P[i], M}});
		dis[P[i]][M] = 0;
	}
	
	while(!pq.empty())
	{
		long long d;
		int u, e;
		d = -pq.top().fr;
		u = pq.top().sc.fr;
		e = pq.top().sc.sc;
		pq.pop();
		
		if(d > dis[u][e])
		{
			continue;
		}
		
		for(int i = 0; i < (int)adj[u].size(); i++)
		{
			int v, e2;
			v = adj[u][i].fr;
			e2 = adj[u][i].sc;
			
			long long newD = d;
			int newE = e;
			if(W[e2] > W[e])
			{
				newD += W[e2];
				newE = e2;
			}
			
			if(newD < dis[v][newE])
			{
				pq.push({-newD, {v, newE}});
				dis[v][newE] = newD;
			}
		}
	}
	
	long long ans = INF;
	for(int i = 0; i <= M; i++)
	{
		ans = min(ans, dis[0][i]);
	}
	return ans;
}
