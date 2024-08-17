#include <bits/stdc++.h>
#include "purwokerto.h"

using namespace std;

#define mp make_pair
#define fr first
#define sc second

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
	
	return {coins, 1};
}
