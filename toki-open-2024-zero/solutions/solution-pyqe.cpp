#include <bits/stdc++.h>
#include "buffet.h"

using namespace std;

#define mp make_pair
#define fr first
#define sc second

const int MXN = 2e5;
const int INF = 1e9;

int A[MXN + 69];

struct segtree
{
	int l, r, lazy;
	pair<int, int> minPos;
	segtree *lef, *rig;
	
	void build(int lh, int rh)
	{
		l = lh;
		r = rh;
		lazy = 0;
		if(l == r)
		{
			minPos = {A[l], l};
		}
		else
		{
			lef = new segtree;
			rig = new segtree;
			
			int mid = (l + r) / 2;
			lef->build(l, mid);
			rig->build(mid + 1, r);
			
			minPos = min(lef->minPos, rig->minPos);
		}
	}
	
	inline void propa()
	{
		lef->minPos.fr += lazy;
		lef->lazy += lazy;
		rig->minPos.fr += lazy;
		rig->lazy += lazy;
		lazy = 0;
	}
	
	void update(int lh, int rh, int w)
	{
		if(rh < l || r < lh);
		else if(lh <= l && r <= rh)
		{
			minPos.fr += w;
			lazy += w;
		}
		else
		{
			propa();
			
			lef->update(lh, rh, w);
			rig->update(lh, rh, w);
			
			minPos = min(lef->minPos, rig->minPos);
		}
	}
	
	pair<int, int> query(int lh, int rh)
	{
		if(rh < l || r < lh)
		{
			return {INF, INF};
		}
		else if(lh <= l && r <= rh)
		{
			return minPos;
		}
		else
		{
			propa();
			
			pair<int, int> mne = min(lef->query(lh, rh), rig->query(lh, rh));
			return mne;
		}
	}
}
seg;

long long min_leftover(int N, vector<int> curA)
{
	for(int i = 0; i < N; i++)
	{
		A[i] = curA[i];
	}
	
	int mn, c;
	mn = INF;
	for(int i = 0; i < N; i++)
	{
		mn = min(mn, A[i]);
	}
	for(int i = 0; i < N; i++)
	{
		A[i] -= mn;
	}
	c = mn;
	
	mn = INF;
	for(int i = 0; i < N; i++)
	{
		mn = min(mn, A[i]);
		A[i] -= mn;
	}
	
	mn = INF;
	for(int i = N - 1; i >= 0; i--)
	{
		mn = min(mn, A[i]);
		A[i] -= mn;
	}
	
	long long ans = 0;
	for(int i = 0; i < N; i++)
	{
		ans += A[i];
	}
	
	priority_queue<pair<int, int>> pq;
	pq.push({N, 0});
	seg.build(0, N - 1);
	while(c > 0 && !pq.empty())
	{
		int len, l, r;
		len = pq.top().fr;
		l = pq.top().sc;
		r = l + len - 1;
		
		pair<int, int> minPos = seg.query(l, r);
		int p;
		mn = minPos.fr;
		p = minPos.sc;
		
		int dec = min(c, mn);
		ans -= (long long)len * dec;
		c -= dec;
		seg.update(l, r, -dec);
		
		if(dec == mn)
		{
			pq.pop();
			if(l < p)
			{
				pq.push({p - l, l});
			}
			if(p < r)
			{
				pq.push({r - p, p + 1});
			}
		}
	}
	
	return ans;
}
