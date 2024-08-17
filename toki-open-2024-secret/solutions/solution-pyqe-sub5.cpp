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

long long sum0, sums[MXN + 69];

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
	
	sum0 = 0;
	for(int i = 0; i < N - 1; i++)
	{
		int dis = abs(A[i] - A[i + 1]);
		sum0 += min(dis, M - dis);
	}
	
	vector<int> updates[N + 69];
	for(int i = 0; i < N - 1; i++)
	{
		int minA, maxA;
		minA = min(posA[i], posA[i + 1]);
		maxA = max(posA[i], posA[i + 1]);
		
		int dis = abs(A[i] - A[i + 1]);
		sums[0] += dis;
		
		updates[minA].push_back(M - dis - dis);
		updates[maxA].push_back(dis - (M - dis));
	}
	
	for(int i = 0; i < N; i++)
	{
		sums[i + 1] = sums[i];
		for(int j = 0; j < (int)updates[i].size(); j++)
		{
			int w = updates[i][j];
			sums[i + 1] += w;
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
		return sum0;
	}
	else if(next(blockedPosses.begin()) == blockedPosses.end())
	{
		int p = *blockedPosses.begin();
		return sums[p];
	}
	else
	{
		return -1;
	}
}
