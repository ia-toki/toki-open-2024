#include <bits/stdc++.h>
#include "temple.h"

using namespace std;

#define mp make_pair
#define fr first
#define sc second

const int MXN = 1e5, MXM = 2e5;
const long long INF = 1e18;

int pos[MXN + 69];
bool vis[MXN + 69];
vector<int> adj[MXN + 69];

void dfs(int u, int curPos)
{
	vis[u] = true;
	pos[u] = curPos;
	
	for(int i = 0; i < (int)adj[u].size(); i++)
	{
		int v = adj[u][i];
		if(!vis[v])
		{
			dfs(v, curPos);
		}
	}
}

long long min_weight(int N, int M, int K, vector<int> U, vector<int> V, vector<int> W, vector<int> P)
{
	pair<int, pair<int, int>> edges[MXM + 69];
	for(int i = 0; i < M; i++)
	{
		edges[i] = {W[i], {U[i], V[i]}};
	}
	sort(edges, edges + M);
	
	long long ans = INF;
	for(int i = 0; i < K; i++)
	{
		for(int j = 0; j < N; j++)
		{
			adj[j].clear();
			vis[j] = false;
		}
		vis[P[i]] = true;
		pos[P[i]] = 0;
		
		vector<pair<int, int>> adj2[MXM + 69];
		int cnt, lastW;
		cnt = 0;
		lastW = 0;
		for(int j = 0; j < M; j++)
		{
			int u, v, w;
			w = edges[j].fr;
			u = edges[j].sc.fr;
			v = edges[j].sc.sc;
			
			adj[u].push_back(v);
			adj[v].push_back(u);
			
			if(w != lastW)
			{
				cnt++;
				adj2[cnt].push_back({cnt - 1, 0});
				
				lastW = w;
			}
			
			if(!vis[u])
			{
				swap(u, v);
			}
			if(vis[u] && !vis[v])
			{
				dfs(v, cnt);
			}
			
			if(vis[u] && vis[v])
			{
				adj2[pos[u]].push_back({cnt, w});
				adj2[pos[v]].push_back({cnt, w});
			}
		}
		
		long long dis[cnt + 69];
		priority_queue<pair<long long, int>> pq;
		for(int j = 0; j <= cnt; j++)
		{
			dis[j] = INF;
		}
		pq.push({0, 0});
		dis[0] = 0;
		
		while(!pq.empty())
		{
			long long d;
			int u;
			d = -pq.top().fr;
			u = pq.top().sc;
			pq.pop();
			
			if(u == pos[0])
			{
				break;
			}
			
			if(d > dis[u])
			{
				continue;
			}
			
			for(int j = 0; j < (int)adj2[u].size(); j++)
			{
				int v, w;
				v = adj2[u][j].fr;
				w = adj2[u][j].sc;
				
				if(d + w < dis[v])
				{
					pq.push({-(d + w), v});
					dis[v] = d + w;
				}
			}
		}
		
		ans = min(ans, dis[pos[0]]);
	}
	
	return ans;
}
