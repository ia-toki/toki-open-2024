#include <bits/stdc++.h>
#include "temple.h"

using namespace std;

long long min_weight(int N, int M, int K, vector<int> U, vector<int> V, vector<int> W, vector<int> P)
{
	long long ans = 0;
	int mx = 0;
	for(int i = M - 1; i >= 0; i--)
	{
		if(W[i] > mx)
		{
			mx = W[i];
			ans += W[i];
		}
	}
	
	return ans;
}
