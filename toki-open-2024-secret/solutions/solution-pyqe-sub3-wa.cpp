#include <bits/stdc++.h>
#include "secret.h"

using namespace std;

#define mp make_pair
#define fr first
#define sc second

const int MXN = 1e5;
const int INF = 1e9;

int N, M, A[MXN + 69];

int cntBlocked[MXN + 69];
map<int, bool> isBlocked;
set<int> blockedPosses;

long long prefSum[MXN + 69];

void init(int curN, int curM, vector<int> curA)
{
	N = curN;
	M = curM;
	for(int i = 0; i < N; i++)
	{
		A[i] = curA[i];
	}
	
	for(int i = 0; i < N - 1; i++)
	{
		int dis = abs(A[i] - A[i + 1]);
		prefSum[i + 1] = prefSum[i] + dis;
	}
}

void toggle(int R)
{
	int pos = upper_bound(A, A + N, R) - A;
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
		set<int>::iterator it, it2;
		it = blockedPosses.upper_bound(S);
		it2 = blockedPosses.upper_bound(T);
		if(it == it2)
		{
			return A[T] - A[S];
		}
		else if(it == prev(it2) && it == blockedPosses.begin() && it2 == blockedPosses.end())
		{
			return A[*it - 1] - A[S] + M - (A[*it] - A[*it - 1]) + A[T] - A[*it];
		}
		else
		{
			return -1;
		}
	}
}
