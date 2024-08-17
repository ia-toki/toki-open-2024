#include <bits/stdc++.h>
#include "purwokerto.h"

using namespace std;

#define mp make_pair
#define fr first
#define sc second

set<int> storage, unstorage;

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
	
	return {coins, 1};
}
