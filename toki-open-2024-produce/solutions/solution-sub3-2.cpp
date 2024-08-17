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
		bool doDecrement, hasZero;
		doDecrement = false;
		hasZero = false;
		for(int j = 0; j < (int)segments.size(); j++)
		{
			int type, len;
			type = segments[j].fr;
			len = segments[j].sc;
			
			if(len > 0)
			{
				if(type == 0)
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
							int type2, len2;
							type2 = segments[k].fr;
							len2 = segments[k].sc;
							
							if(len2 > 0 && type2 == 1)
							{
								break;
							}
							c += len2;
						}
						ways = (long long)ways * c % MOD;
						
						c = 0;
						for(int k = j; k < (int)segments.size(); k++)
						{
							int type2, len2;
							type2 = segments[k].fr;
							len2 = segments[k].sc;
							
							if(len2 > 0 && type2 == 0)
							{
								break;
							}
							c += len2;
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
		for(int j = 0; j < (int)segments.size(); j++)
		{
			int type, len;
			type = segments[j].fr;
			len = segments[j].sc;
			
			if(len > 0)
			{
				if(type == 0)
				{
					hasZero = true;
					pos = j;
				}
				else
				{
					if(hasZero)
					{
						segments[pos].sc--;
						segments[j].sc--;
						hasZero = false;
					}
				}
			}
		}
	}
	
	return {coins, ways};
}
