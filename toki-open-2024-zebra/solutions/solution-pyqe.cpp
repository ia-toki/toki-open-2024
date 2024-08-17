#include <bits/stdc++.h>
#include "zebra.h"

using namespace std;

#define mp make_pair
#define fr first
#define sc second

const int MXN = 1e5;

int deg[MXN + 69], dep[MXN + 69], depE[MXN + 69];
vector<pair<int, int>> adj[MXN + 69];
bool vis[MXN + 69];

void dfs(long long x)
{
	vis[x] = 1;
	for(int i = 0; i < adj[x].size(); i++)
	{
		int l, e;
		l = adj[x][i].fr;
		e = adj[x][i].sc;
		if(!vis[l])
		{
			dep[l] = dep[x] + 1;
			depE[e] = dep[x];
			dfs(l);
		}
	}
}

int find_root(int N, vector<int> U, vector<int> V)
{
	for(int i = 0; i < N - 1; i++)
	{
		adj[U[i]].push_back({V[i], i});
		adj[V[i]].push_back({U[i], i});
		deg[U[i]]++;
		deg[V[i]]++;
	}
	
	int root;
	for(int i = 0; i < N; i++)
	{
		if(deg[i] == 1)
		{
			root = i;
			break;
		}
	}
	
	dep[root] = 0;
	dfs(root);
	
	vector<pair<int, int>> sortedEdges[2];
	for(int i = 0; i < N - 1; i++)
	{
		int rem = depE[i] % 2;
		sortedEdges[rem].push_back({depE[i], i});
	}
	
	for(int i = 0; i < 2; i++)
	{
		vector<int> edges;
		for(int j = 0; j < sortedEdges[i].size(); j++)
		{
			edges.push_back(sortedEdges[i][j].sc);
		}
		if(!is_zebra(edges))
		{
			continue;
		}
		
		sort(sortedEdges[i].begin(), sortedEdges[i].end(), greater<pair<int, int>>());
		
		int l, r, z;
		l = 1;
		r = sortedEdges[i].size() - 1;
		while(l <= r)
		{
			int mid = (l + r) / 2;
			
			edges.clear();
			for(int j = 0; j <= mid; j++)
			{
				edges.push_back(sortedEdges[i][j].sc);
			}
			if(is_zebra(edges))
			{
				z = mid;
				r = mid - 1;
			}
			else
			{
				l = mid + 1;
			}
		}
		
		int p, p2, p3;
		p = sortedEdges[i][0].sc;
		p2 = sortedEdges[i][z].sc;
		p3 = sortedEdges[0].back().sc;
		
		edges = {p, p3};
		if(is_zebra(edges) ^ i)
		{
			swap(p, p2);
		}
		
		int k;
		if(dep[U[p]] > dep[V[p]])
		{
			k = U[p];
		}
		else
		{
			k = V[p];
		}
		
		vector<int> curEdges;
		for(int j = 0; j < adj[k].size(); j++)
		{
			int l, e;
			l = adj[k][j].fr;
			e = adj[k][j].sc;
			if(dep[l] == dep[k] + 1)
			{
				curEdges.push_back(e);
			}
		}
		
		l = 0;
		r = curEdges.size() - 1;
		z = -1;
		while(l <= r)
		{
			int mid = (l + r) / 2;
			
			edges = {p};
			for(int j = 0; j <= mid; j++)
			{
				edges.push_back(curEdges[j]);
			}
			if(is_zebra(edges))
			{
				z = mid;
				r = mid - 1;
			}
			else
			{
				l = mid + 1;
			}
		}
		
		if(z == -1)
		{
			return k;
		}
		else
		{
			p = curEdges[z];
			if(dep[U[p]] > dep[V[p]])
			{
				return U[p];
			}
			else
			{
				return V[p];
			}
		}
	}
	
	if(sortedEdges[0].size() == 1)
	{
		vector<int> edges;
		edges.push_back(sortedEdges[0][0].sc);
		edges.push_back(sortedEdges[1][0].sc);
		if(is_zebra(edges))
		{
			return root;
		}
		else
		{
			return adj[root][0].fr;
		}
	}
	else
	{
		return root;
	}
}
