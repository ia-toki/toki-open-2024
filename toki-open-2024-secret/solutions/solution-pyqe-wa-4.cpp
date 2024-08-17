#include <bits/stdc++.h>
#include "secret.h"

using namespace std;

#define mp make_pair
#define fr first
#define sc second

const int MXN = 1e5;
const int MXLOG = 17;
const int INF = 1e9;

int N;
pair<int, int> sortedA[MXN + 69];
int posA[MXN + 69];

int cntBlocked[MXN + 69];
map<int, bool> isBlocked;
set<int> blockedPosses;

long long prefSum[MXN + 69];

int sparseMin[MXLOG][MXN + 69], sparseMax[MXLOG][MXN + 69];
int lg2[MXN + 69];

inline void sparseBuild()
{
	for(int i = 0; i < N; i++)
	{
		sparseMin[0][i] = posA[i];
		sparseMax[0][i] = posA[i];
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

int sums[MXN + 69];

struct segtree
{
	int l, r;
	long long sum;
	segtree *lef, *rig;
	
	void build(int lh, int rh, bool useSums)
	{
		l = lh;
		r = rh;
		
		if(l == r)
		{
			if(!useSums)
			{
				sum = 0;
			}
			else
			{
				sum = sums[l];
			}
		}
		else
		{
			lef = new segtree;
			rig = new segtree;
			
			int mid = (l + r) / 2;
			lef->build(l, mid, useSums);
			rig->build(mid + 1, r, useSums);
			
			sum = lef->sum + rig->sum;
		}
	}
	
	void update(int x, int w)
	{
		if(l == r && l == x)
		{
			sum += w;
		}
		else
		{
			if(!(x < lef->l || lef->r < x))
			{
				segtree *tmp = lef;
				lef = new segtree;
				*lef = *tmp;
				
				lef->update(x, w);
			}
			
			if(!(x < rig->l || rig->r < x))
			{
				segtree *tmp = rig;
				rig = new segtree;
				*rig = *tmp;
				
				rig->update(x, w);
			}
			
			sum = lef->sum + rig->sum;
		}
	}
	
	long long query(int lh, int rh)
	{
		if(rh < l || r < lh)
		{
			return 0;
		}
		else if(lh <= l && r <= rh)
		{
			return sum;
		}
		else
		{
			return lef->query(lh, rh) + rig->query(lh, rh);
		}
	}
}
seg[MXN + 69], seg2[MXN + 69];

void init(int curN, int M, vector<int> A)
{
	N = curN;
	for(int i = 0; i < N; i++)
	{
		sortedA[i] = {A[i], i};
	}
	sort(sortedA, sortedA + N);
	for(int i = 0; i < N; i++)
	{
		posA[sortedA[i].sc] = i;
	}
	
	for(int i = 0; i < N - 1; i++)
	{
		int dis = abs(A[i] - A[i + 1]);
		prefSum[i + 1] = prefSum[i] + min(dis, M - dis);
	}
	
	sparseBuild();
	
	seg[0].build(0, N - 1, false);
	for(int i = 0; i < N; i++)
	{
		seg[i + 1] = seg[i];
		seg[i + 1].update(posA[i], 1);
	}
	
	vector<pair<int, int>> updates[N + 69];
	for(int i = 0; i < N - 1; i++)
	{
		int minA, maxA;
		minA = min(posA[i], posA[i + 1]);
		maxA = max(posA[i], posA[i + 1]);
		
		int dis = abs(A[i] - A[i + 1]);
		sums[i] = dis;
		
		updates[minA].push_back({i, M - dis - dis});
		updates[maxA].push_back({i, dis - (M - dis)});
	}
	
	seg2[0].build(0, N - 2, true);
	for(int i = 0; i < N; i++)
	{
		seg2[i + 1] = seg2[i];
		for(int j = 0; j < (int)updates[i].size(); j++)
		{
			int x, w;
			x = updates[i][j].fr;
			w = updates[i][j].sc;
			
			seg2[i + 1].update(x, w);
		}
	}
}

void toggle(int R)
{
	int pos = upper_bound(sortedA, sortedA + N, mp(R, INF)) - sortedA;
	if(pos == N)
	{
		pos = 0;
	}
	
	if(!isBlocked[R])
	{
		isBlocked[R] = true;
		
		if(cntBlocked[pos] == 0)
		{
			blockedPosses.insert(pos);
		}
		cntBlocked[pos]++;
	}
	else
	{
		isBlocked[R] = false;
		
		cntBlocked[pos]--;
		if(cntBlocked[pos] == 0)
		{
			blockedPosses.erase(pos);
		}
	}
}

long long query(int S, int T)
{
	if(blockedPosses.empty())
	{
		return prefSum[T] - prefSum[S];
	}
	else
	{
		int isPossible = false;
		
		int l, r;
		
		l = sparseQueryMin(S, T);
		r = sparseQueryMax(S, T);
		
		set<int>::iterator it = blockedPosses.upper_bound(l);
		if(it == blockedPosses.end() || *it > r)
		{
			isPossible = true;
		}
		
		l = *blockedPosses.begin();
		r = *prev(blockedPosses.end());
		
		int cntA = seg[T + 1].query(l, r - 1) - seg[S].query(l, r - 1);
		if(cntA == 0)
		{
			isPossible = true;
		}
		
		if(!isPossible)
		{
			return -1;
		}
		
		return seg2[l].query(S, T - 1) + 1;
	}
}
