#include <bits/stdc++.h>
#include "purwokerto.h"

using namespace std;

#define mp make_pair
#define fr first
#define sc second

const int MOD = 1e9 + 7;

set<int> storage, unstorage;
vector<pair<int, int>> segments;

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
	
	for(int i = 0; i < M; i++)
	{
		if(segments.empty() || T[i] != segments.back().fr)
		{
			segments.push_back({T[i], 0});
		}
		segments.back().sc++;
	}
	
	int ways = 1;
	for(int i = N; i > 0; i--)
	{
		for(int j = 0; j < (int)segments.size() - 1; j++)
		{
			if(segments[j].fr == 0)
			{
				ways = (long long)ways * segments[j].sc % MOD;
				ways = (long long)ways * segments[j + 1].sc % MOD;
				
				segments[j].sc--;
				segments[j + 1].sc--;
			}
		}
		
		vector<pair<int, int>> tmp;
		for(int j = 0; j < (int)segments.size(); j++)
		{
			int type, len;
			type = segments[j].fr;
			len = segments[j].sc;
			
			if(len > 0)
			{
				if(tmp.empty() || type != tmp.back().fr)
				{
					tmp.push_back({type, 0});
				}
				tmp.back().sc += len;
			}
		}
		segments = tmp;
	}
	
	return {coins + 1, ways};
}
