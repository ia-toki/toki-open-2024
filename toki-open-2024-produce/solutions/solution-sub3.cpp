#include <bits/stdc++.h>
#include "purwokerto.h"

using namespace std;

#define mp make_pair
#define fr first
#define sc second

const int MXN = 2e5;
const int MOD = 1e9 + 7;

set<int> storage, unstorage;
bool vis[MXN + 69];

pair<long long, int> purwokerto(int N, int M, vector<int> T)
{
	for(int i = 1; i <= N; i++)
	{
		unstorage.insert(i);
	}
	
	long long coins = 0;
	for(int i = 0; i < M; i++)
	{
		if(T[i] == 0)
		{
			if(!unstorage.empty())
			{
				int val = *prev(unstorage.end());
				unstorage.erase(val);
				storage.insert(val);
			}
		}
		else
		{
			if(!storage.empty())
			{
				int val = *prev(storage.end());
				coins += val;
				storage.erase(val);
				unstorage.insert(val);
			}
		}
	}
	
	int ways = 1;
	for(int i = N; i > 0; i--)
	{
		bool doDecrement, hasZero;
		doDecrement = false;
		hasZero = false;
		for(int j = 0; j < M; j++)
		{
			if(!vis[j])
			{
				if(T[j] == 0)
				{
					hasZero = true;
				}
				else
				{
					if(hasZero)
					{
						int c = 0;
						for(int k = j - 1; k >= 0; k--)
						{
							if(!vis[k])
							{
								if(T[k] == 1)
								{
									break;
								}
								c++;
							}
						}
						ways = (long long)ways * c % MOD;
						
						c = 0;
						for(int k = j; k < M; k++)
						{
							if(!vis[k])
							{
								if(T[k] == 0)
								{
									break;
								}
								c++;
							}
						}
						ways = (long long)ways * c % MOD;
						
						hasZero = false;
						
						doDecrement = true;
					}
				}
			}
		}
		
		if(!doDecrement)
		{
			break;
		}
		
		int pos;
		hasZero = false;
		for(int j = 0; j < M; j++)
		{
			if(!vis[j])
			{
				if(T[j] == 0)
				{
					hasZero = true;
					pos = j;
				}
				else
				{
					if(hasZero)
					{
						vis[pos] = true;
						vis[j] = true;
						hasZero = false;
					}
				}
			}
		}
	}
	
	return {coins, ways};
}
