#include <bits/stdc++.h>
#include "temple.h"

using namespace std;

#define mp make_pair
#define fr first
#define sc second

const int MXN = 1e5, MXM = 2e5;
const long long INF = 1e18;

int dsu[MXN + 69], head[MXN + 69];
long long dis[MXN * 2 + 69];
pair<int, pair<int, int>> edges[MXM + 69];
vector<pair<int, int>> adj[MXN * 2 + 69];
priority_queue<pair<long long, int>> pq;

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
	for(int i = 0; i < M; i++)
	{
		edges[i] = {W[i], {U[i], V[i]}};
	}
	sort(edges, edges + M);
	
	for(int i = 0; i < N; i++)
	{
		dsu[i] = i;
		head[i] = i;
	}
	
	int cnt = N;
	for(int i = 0; i < M; i++)
	{
		int u, v, w;
		u = edges[i].sc.fr;
		v = edges[i].sc.sc;
		w = edges[i].fr;
		
		u = unionFind(u);
		v = unionFind(v);
		if(u != v)
		{
			adj[cnt].push_back({head[u], 0});
			adj[cnt].push_back({head[v], 0});
			
			dsu[v] = u;
			head[u] = cnt;
			cnt++;
		}
		
		if(i == M - 1 || w != edges[i + 1].fr)
		{
			for(int j = i; j >= 0 && edges[j].fr == w; j--)
			{
				u = edges[j].sc.fr;
				v = edges[j].sc.sc;
				
				int target = head[unionFind(u)];
				adj[u].push_back({target, w});
				adj[v].push_back({target, w});
			}
		}
	}
	
	for(int i = 0; i < cnt; i++)
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
		int d, u;
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
			
			if(d + w < dis[v])
			{
				pq.push({-(d + w), v});
				dis[v] = d + w;
			}
		}
	}
	
	return dis[0];
}
