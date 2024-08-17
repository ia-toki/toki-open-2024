#include <bits/stdc++.h>
#include "purwokerto.h"

using namespace std;

#define mp make_pair
#define fr first
#define sc second

const int MOD = 1e9 + 7;

pair<long long, int> purwokerto(int N, int M, vector<int> T)
{
	long long coins = 0;
	bool isFull = false;
	for(int i = 0; i < M; i++)
	{
		if(T[i] == 0)
		{
			isFull = true;
		}
		else
		{
			if(isFull)
			{
				coins++;
				isFull = false;
			}
		}
	}
	
	int ways = 1;
	for(int i = 0; i < M - 1; i++)
	{
		if(T[i] == 0 && T[i + 1] == 1)
		{
			int c = 0;
			for(int j = i; j >= 0 && T[j] == 0; j--)
			{
				c++;
			}
			ways = (long long)ways * c % MOD;
			
			c = 0;
			for(int j = i + 1; j < M && T[j] == 1; j++)
			{
				c++;
			}
			ways = (long long)ways * c % MOD;
		}
	}
	
	return {coins, ways};
}
