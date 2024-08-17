#include <bits/stdc++.h>
#include "purwokerto.h"

using namespace std;

#define mp make_pair
#define fr first
#define sc second

const int MXN = 2e5;

bool storage[MXN + 69];

pair<long long, int> purwokerto(int N, int M, vector<int> T)
{
	long long coins = 0;
	for(int i = 0; i < M; i++)
	{
		if(T[i] == 0)
		{
			for(int j = N - 1; j >= 0; j--)
			{
				if(!storage[j])
				{
					storage[j] = true;
					break;
				}
			}
		}
		else
		{
			for(int j = N - 1; j >= 0; j--)
			{
				if(storage[j])
				{
					storage[j] = false;
					coins += j + 1;
					break;
				}
			}
		}
	}
	
	return {coins, 1};
}
