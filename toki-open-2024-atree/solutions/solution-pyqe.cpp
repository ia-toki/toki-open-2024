#include <bits/stdc++.h>
#include "copper.h"

using namespace std;

#define mp make_pair
#define fr first
#define sc second

const int MXN = 2e5, MXM = 2e5;
const int MXLOG = 18;

int A[MXM + 69];

int par[MXN + 69], dep[MXN + 69];
vector<int> adj[MXN + 69];
bool vis[MXN + 69];

void dfs(int u)
{
	vis[u] = true;
	
	for(int i = 0; i < (int)adj[u].size(); i++)
	{
		int v = adj[u][i];
		if(!vis[v])
		{
			par[v] = u;
			dep[v] = dep[u] + 1;
			
			dfs(v);
		}
	}
}

int nxtLeft[MXM + 69][MXLOG], wrapLeft[MXM + 69][MXLOG];
int nxtRight[MXM + 69][MXLOG], wrapRight[MXM + 69][MXLOG];

int moveToLCA(int pos1, int pos2, int maxWraps)
{
	int u, v;
	u = A[pos1];
	v = A[pos2];
	
	int wraps = 0;
	
	for(int i = MXLOG - 1; i >= 0; i--)
	{
		if(dep[u] - (1 << i) >= dep[v])
		{
			wraps += wrapRight[pos1][i];
			pos1 = nxtRight[pos1][i];
			u = A[pos1];
		}
		
		if(dep[v] - (1 << i) >= dep[u])
		{
			wraps += wrapLeft[pos2][i];
			pos2 = nxtLeft[pos2][i];
			v = A[pos2];
		}
	}
	
	for(int i = MXLOG - 1; i >= 0; i--)
	{
		if(dep[u] - (1 << i) >= 0)
		{
			int newPos1, newPos2;
			newPos1 = nxtRight[pos1][i];
			newPos2 = nxtLeft[pos2][i];
			
			if(A[newPos1] != A[newPos2])
			{
				wraps += wrapRight[pos1][i] + wrapLeft[pos2][i];
				pos1 = newPos1;
				pos2 = newPos2;
				u = A[pos1];
				v = A[pos2];
			}
		}
	}
	
	if(u != v)
	{
		wraps += wrapRight[pos1][0] + wrapLeft[pos2][0];
		pos1 = nxtRight[pos1][0];
		pos2 = nxtLeft[pos2][0];
	}
	
	if(pos1 > pos2)
	{
		wraps++;
	}
	
	return wraps <= maxWraps;
}

long long min_risk(int N, int M, int L, vector<int> U, vector<int> V, vector<int> curA, vector<int> S)
{
	for(int i = 0; i < M; i++)
	{
		A[i] = curA[i];
	}
	
	for(int i = 0; i < N - 1; i++)
	{
		adj[U[i]].push_back(V[i]);
		adj[V[i]].push_back(U[i]);
	}
	
	dep[0] = 0;
	dfs(0);
	
	int lastPos[N + 69];
	
	for(int i = 0; i < N; i++)
	{
		lastPos[i] = -1;
	}
	for(int i = 0; i < M; i++)
	{
		if(A[i] != 0)
		{
			int nxt = lastPos[par[A[i]]];
			if(nxt != -1)
			{
				nxtLeft[i][0] = nxt;
				wrapLeft[i][0] = 0;
			}
			else
			{
				nxtLeft[i][0] = -1;
			}
		}
		
		lastPos[A[i]] = i;
	}
	for(int i = 0; i < M; i++)
	{
		if(A[i] != 0 && nxtLeft[i][0] == -1)
		{
			nxtLeft[i][0] = lastPos[par[A[i]]];
			wrapLeft[i][0] = 1;
		}
	}
	
	for(int i = 0; i < N; i++)
	{
		lastPos[i] = -1;
	}
	for(int i = M - 1; i >= 0; i--)
	{
		if(A[i] != 0)
		{
			int nxt = lastPos[par[A[i]]];
			if(nxt != -1)
			{
				nxtRight[i][0] = nxt;
				wrapRight[i][0] = 0;
			}
			else
			{
				nxtRight[i][0] = -1;
			}
		}
		
		lastPos[A[i]] = i;
	}
	for(int i = 0; i < M; i++)
	{
		if(A[i] != 0 && nxtRight[i][0] == -1)
		{
			nxtRight[i][0] = lastPos[par[A[i]]];
			wrapRight[i][0] = 1;
		}
	}
	
	for(int i = 1; i < MXLOG; i++)
	{
		for(int j = 0; j < M; j++)
		{
			nxtLeft[j][i] = nxtLeft[nxtLeft[j][i - 1]][i - 1];
			wrapLeft[j][i] = wrapLeft[j][i - 1] + wrapLeft[nxtLeft[j][i - 1]][i - 1];
			
			nxtRight[j][i] = nxtRight[nxtRight[j][i - 1]][i - 1];
			wrapRight[j][i] = wrapRight[j][i - 1] + wrapRight[nxtRight[j][i - 1]][i - 1];
		}
	}
	
	vector<int> posses[N + 69];
	for(int i = 0; i < M; i++)
	{
		posses[A[i]].push_back(i);
	}
	
	long long ans = 0;
	int cur;
	cur = 0;
	for(int i = 0; i < L; i++)
	{
		int sz = posses[S[i]].size();
		
		long long l, r, z;
		l = 0;
		r = (long long)sz * N;
		while(l <= r)
		{
			long long mid = (l + r) / 2;
			
			int wraps, p, nxtPos;
			wraps = mid / sz;
			p = mid % sz;
			nxtPos = posses[S[i]][p];
			
			if(moveToLCA(cur, nxtPos, wraps))
			{
				z = mid;
				r = mid - 1;
			}
			else
			{
				l = mid + 1;
			}
		}
		
		int p = z % sz;
		cur = posses[S[i]][p];
		ans += z / sz;
	}
	
	return ans;
}
