#include <bits/stdc++.h>
#include "temple.h"

using namespace std;

#define mp make_pair
#define fr first
#define sc second

const int MXN = 1e3, CNTW = 10;
const long long INF = 1e18;

int cntW;
vector<int> comW;
vector<pair<int, int>> adj[MXN + 69];
queue<pair<int, int>> q;
bool vis[MXN + 69][1 << CNTW];

inline int getPosW(int w)
{
	return lower_bound(comW.begin(), comW.end(), w) - comW.begin();
}

long long min_weight(int N, int M, int K, vector<int> U, vector<int> V, vector<int> W, vector<int> P)
{
	comW = W;
	sort(comW.begin(), comW.end());
	comW.erase(unique(comW.begin(), comW.end()), comW.end());
	cntW = comW.size();
	
	for(int i = 0; i < M; i++)
	{
		adj[U[i]].push_back({V[i], W[i]});
		adj[V[i]].push_back({U[i], W[i]});
	}
	
	q.push({0, 0});
	vis[0][0] = true;
	while(!q.empty())
	{
		int u, mask;
		u = q.front().fr;
		mask = q.front().sc;
		q.pop();
		
		for(int i = 0; i < (int)adj[u].size(); i++)
		{
			int v, w, posW;
			v = adj[u][i].fr;
			w = adj[u][i].sc;
			posW = getPosW(w);
			
			int mask2 = mask;
			for(int j = 0; j < posW; j++)
			{
				if(mask2 >> j & 1)
				{
					mask2 ^= 1 << j;
				}
			}
			mask2 |= 1 << posW;
			
			if(!vis[v][mask2])
			{
				q.push({v, mask2});
				vis[v][mask2] = true;
			}
		}
	}
	
	long long ans = INF;
	for(int i = 0; i < K; i++)
	{
		for(int mask = 0; mask < 1 << cntW; mask++)
		{
			if(vis[P[i]][mask])
			{
				long long sum = 0;
				for(int j = 0; j < cntW; j++)
				{
					if(mask >> j & 1)
					{
						sum += comW[j];
					}
				}
				
				ans = min(ans, sum);
			}
		}
	}
	
	return ans;
}
