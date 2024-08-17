#include <bits/stdc++.h>
#include "secret.h"

using namespace std;

#define mp make_pair
#define fr first
#define sc second

const int MXN = 1e5;
const int INF = 1e9;

long long prefSum[MXN + 69];

void init(int N, int M, vector<int> A)
{
	for(int i = 0; i < N - 1; i++)
	{
		int dis = abs(A[i] - A[i + 1]);
		prefSum[i + 1] = prefSum[i] + min(dis, M - dis);
	}
}

void toggle(int R)
{
}

long long query(int S, int T)
{
	return prefSum[T] - prefSum[S];
}
