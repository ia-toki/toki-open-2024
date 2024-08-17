#include <bits/stdc++.h>
#include "secret.h"

using namespace std;

#define mp make_pair
#define fr first
#define sc second

const int MXN = 1e5;
const int MXLOG = 17;
const int INF = 1e9;

int N, A[MXN + 69];

set<int> blockedRoads;

long long prefSum[MXN + 69];

int sparseMin[MXLOG][MXN + 69], sparseMax[MXLOG][MXN + 69];
int lg2[MXN + 69];

inline void sparseBuild()
{
	for(int i = 0; i < N; i++)
	{
		sparseMin[0][i] = A[i];
		sparseMax[0][i] = A[i];
	}
	for(int i = 1; 1 << i <= N; i++)
	{
		for(int j = 0; j + (1 << i) - 1 < N; j++)
		{
			sparseMin[i][j] = min(sparseMin[i - 1][j], sparseMin[i - 1][j + (1 << i - 1)]);
			sparseMax[i][j] = max(sparseMax[i - 1][j], sparseMax[i - 1][j + (1 << i - 1)]);
		}
	}
	
	for(int i = 1; i <= N; i++)
	{
		int w = i;
		while(w > 1)
		{
			lg2[i]++;
			w /= 2;
		}
	}
}

inline int sparseQueryMin(int l, int r)
{
	int dep = lg2[r - l + 1];
	return min(sparseMin[dep][l], sparseMin[dep][r - (1 << dep) + 1]);
}

inline int sparseQueryMax(int l, int r)
{
	int dep = lg2[r - l + 1];
	return max(sparseMax[dep][l], sparseMax[dep][r - (1 << dep) + 1]);
}

void init(int curN, int M, vector<int> curA)
{
	N = curN;
	for(int i = 0; i < N; i++)
	{
		A[i] = curA[i];
	}
	
	for(int i = 0; i < N - 1; i++)
	{
		int dis = abs(A[i] - A[i + 1]);
		prefSum[i + 1] = prefSum[i] + dis;
	}
	
	sparseBuild();
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
	int l, r;
	
	l = sparseQueryMin(S, T);
	r = sparseQueryMax(S, T);
	
	set<int>::iterator it = blockedRoads.lower_bound(l);
	if(it == blockedRoads.end() || *it >= r)
	{
		return prefSum[T] - prefSum[S];
	}
	else
	{
		return -1;
	}
}
