#include <bits/stdc++.h>
#include <tcframe/spec.hpp>

using namespace std;
using namespace tcframe;

#define mp make_pair
#define fr first
#define sc second

const int MXN = 2e5, MXM = 2e5;
const int MXN2 = 11, MXM2 = 2e3;
const int MXN3 = 200;

class ProblemSpec : public BaseProblemSpec
{
	protected:
		int subtask, N, M, ans2;
		long long ans;
		vector<int> T;

		void InputFormat()
		{
			LINE(subtask);
			LINE(N, M);
			LINE(T % SIZE(M));
		}
		void OutputFormat()
		{
			LINE(ans, ans2);
		}
		void GradingConfig()
		{
			TimeLimit(2);
			MemoryLimit(512);
		}
		void Constraints()
		{
			CONS(0 <= subtask && subtask <= 4);
			CONS(1 <= N && N <= MXN);
			CONS(1 <= M && M <= MXM);
			CONS(eachElementBetween(T, 0, 1));
		}
		void StyleConfig()
		{
			CustomScorer();
		}
		void Subtask1()
		{
			Points(12);
			CONS(subtask == 1);
			CONS(N == 1);
		}
		void Subtask2()
		{
			Points(24);
			CONS(subtask == 2);
			CONS(N <= MXN2);
			CONS(M <= MXM2);
		}
		void Subtask3()
		{
			Points(40);
			CONS(subtask == 3);
			CONS(N <= MXN3);
		}
		void Subtask4()
		{
			Points(24);
			CONS(subtask == 4);
		}

	private:
		bool eachElementBetween(vector<int> &curV, int lo, int hi)
		{
			for(int i = 0; i < curV.size(); i++)
			{
				if(!(lo <= curV[i] && curV[i] <= hi))
				{
					return false;
				}
			}
			return true;
		}
};

class TestSpec : public BaseTestSpec<ProblemSpec>
{
	protected:
		void SampleTestCase1()
		{
			Subtasks({});
			Input
			({
				"0",
				"2 8",
				"0 0 1 0 0 1 1 1",
			});
			Output
			({
				"5 48",
			});
		}
		void SampleTestCase2()
		{
			Subtasks({});
			Input
			({
				"0",
				"100000 2",
				"1 0",
			});
			Output
			({
				"0 1",
			});
		}
		void SampleTestCase3()
		{
			Subtasks({});
			Input
			({
				"0",
				"8 16",
				"0 0 0 0 0 0 0 0 1 1 1 1 1 1 1 1",
			});
			Output
			({
				"36 625702393",
			});
		}
		
		vector<int> decrements;
		
		void BeforeTestCase()
		{
			T.clear();
		}
		void TestGroup1()
		{
			Subtasks({1});
			
			// Case (1-2) rand
			CASE(subtask = 1, N = 1, M = rnd.nextInt(1, MXM), T = rndArray(M, 0, 1));
			CASE(subtask = 1, N = 1, M = MXM, T = rndArray(M, 0, 1));
			
			// Case (3-4) 1 segment
			CASE(subtask = 1, N = 1, M = MXM, T = rndArray(M, 0, 0));
			CASE(subtask = 1, N = 1, M = MXM, T = rndArray(M, 1, 1));
			
			// Case (5-8) 2 segments
			CASE(subtask = 1, N = 1, M = MXM, T = segmentedArray({{0, M / 2}, {1, M / 2}}));
			CASE(subtask = 1, N = 1, M = MXM, T = segmentedArray({{0, M / 2 - (int)sqrt(M) / 2}, {1, M / 2 + (int)sqrt(M) / 2}}), T = rndFlip(T, sqrt(M) / 2));
			CASE(subtask = 1, N = 1, M = MXM, T = segmentedArray({{0, M - M / 3}, {1, M / 3}}));
			CASE(subtask = 1, N = 1, M = MXM, T = segmentedArray({{1, M / 2}, {0, M / 2}}));
			
			// Case (9) 4 segments
			CASE(subtask = 1, N = 1, M = MXM, T = segmentedArray({{0, M / 4}, {1, M / 8}, {0, M / 4}, {1, M * 3 / 8}}));
			
			// Case (10-11) many segments
			CASE(subtask = 1, N = 1, M = MXM, T = alternatingArray(M, 1));
			CASE(subtask = 1, N = 1, M = MXM, T = alternatingArray(M, 2));
			
			// Case (12-14) small M
			CASE(subtask = 1, N = 1, M = 1, T = {0});
			CASE(subtask = 1, N = 1, M = 1, T = {1});
			CASE(subtask = 1, N = 1, M = 2, T = {0, 1});
			
			// Case (15-17) many segments turns into few segments
			CASE(subtask = 1, N = 1, M = MXM, T = rndArray(M / 3, 0, 0), T = addMaxSegments(M, T));
			CASE(subtask = 1, N = 1, M = MXM, T = rndArray(M / 3, 1, 1), T = addMaxSegments(M, T));
			CASE(subtask = 1, N = 1, M = MXM, T = segmentedArray({{0, M / 6}, {1, M / 6}}), T = addMaxSegments(M, T));
			
			// Case (18-21) smart (T generated from number of decrements in each iteration)
			CASE(subtask = 1, N = 1, M = MXM, decrements = fixedSumArray(10, M / 2 - M / 20), T = smartArray(decrements, M / 20, M / 20));
			CASE(subtask = 1, N = 1, M = MXM, decrements = fixedSumArray(sqrt(M), M / 2 - (int)sqrt(M) / 2), T = smartArray(decrements, (int)sqrt(M) / 2, (int)sqrt(M) / 2));
			CASE(subtask = 1, N = 1, M = MXM, decrements = fixedSumArray(M / 10, M / 2 - (int)sqrt(M) / 2), T = smartArray(decrements, (int)sqrt(M) / 2, (int)sqrt(M) / 2));
			CASE(subtask = 1, N = 1, M = MXM, decrements = fixedSumArray(M / 4, M / 2 - (int)sqrt(M) / 2), T = smartArray(decrements, (int)sqrt(M) / 2, (int)sqrt(M) / 2));
			
			// Case (22) smart with unbalanced decrements
			CASE(subtask = 1, N = 1, M = MXM, decrements = fixedSumArray(M / 4, M / 4 + (int)sqrt(M) * 5), decrements[0] += M / 4 - (int)sqrt(M) * 5, T = smartArray(decrements, 0, 0));
		}
		void TestGroup2()
		{
			Subtasks({2});
			
			// Case (1-3) rand
			CASE(subtask = 2, N = rnd.nextInt(1, MXN2), M = rnd.nextInt(1, MXM2), T = rndArray(M, 0, 1));
			CASE(subtask = 2, N = rnd.nextInt(1, MXN2), M = MXM2, T = rndArray(M, 0, 1));
			CASE(subtask = 2, N = MXN2, M = MXM2, T = rndArray(M, 0, 1));
			
			// Case (4) small N
			CASE(subtask = 2, N = 1, M = MXM2, T = rndArray(M, 0, 1));
			
			// Case (5-6) 1 segment
			CASE(subtask = 2, N = MXN2, M = MXM2, T = rndArray(M, 0, 0));
			CASE(subtask = 2, N = MXN2, M = MXM2, T = rndArray(M, 1, 1));
			
			// Case (7-10) 2 segments
			CASE(subtask = 2, N = MXN2, M = MXM2, T = segmentedArray({{0, M / 2}, {1, M / 2}}));
			CASE(subtask = 2, N = MXN2, M = MXM2, T = segmentedArray({{0, M / 2 - (int)sqrt(M) / 2}, {1, M / 2 + (int)sqrt(M) / 2}}), T = rndFlip(T, sqrt(M) / 2));
			CASE(subtask = 2, N = MXN2, M = MXM2, T = segmentedArray({{0, M - M / 3}, {1, M / 3}}));
			CASE(subtask = 2, N = MXN2, M = MXM2, T = segmentedArray({{1, M / 2}, {0, M / 2}}));
			
			// Case (11) 4 segments
			CASE(subtask = 2, N = MXN2, M = MXM2, T = segmentedArray({{0, M / 4}, {1, M / 8}, {0, M / 4}, {1, M * 3 / 8}}));
			
			// Case (12-13) many segments
			CASE(subtask = 2, N = MXN2, M = MXM2, T = alternatingArray(M, 1));
			CASE(subtask = 2, N = MXN2, M = MXM2, T = alternatingArray(M, 2));
			
			// Case (14-16) small M
			CASE(subtask = 2, N = rnd.nextInt(1, MXN2), M = 1, T = {0});
			CASE(subtask = 2, N = rnd.nextInt(1, MXN2), M = 1, T = {1});
			CASE(subtask = 2, N = rnd.nextInt(1, MXN2), M = 2, T = {0, 1});
			
			// Case (17-19) many segments turns into few segments
			CASE(subtask = 2, N = MXN2, M = MXM2, T = rndArray(M / 3, 0, 0), T = addMaxSegments(M, T));
			CASE(subtask = 2, N = MXN2, M = MXM2, T = rndArray(M / 3, 1, 1), T = addMaxSegments(M, T));
			CASE(subtask = 2, N = MXN2, M = MXM2, T = segmentedArray({{0, M / 6}, {1, M / 6}}), T = addMaxSegments(M, T));
			
			// Case (20-23) smart (T generated from number of decrements in each iteration)
			CASE(subtask = 2, N = MXN2, M = MXM2, decrements = fixedSumArray(10, M / 2 - M / 20), T = smartArray(decrements, M / 20, M / 20));
			CASE(subtask = 2, N = MXN2, M = MXM2, decrements = fixedSumArray(sqrt(M), M / 2 - (int)sqrt(M) / 2), T = smartArray(decrements, (int)sqrt(M) / 2, (int)sqrt(M) / 2));
			CASE(subtask = 2, N = MXN2, M = MXM2, decrements = fixedSumArray(M / 10, M / 2 - (int)sqrt(M) / 2), T = smartArray(decrements, (int)sqrt(M) / 2, (int)sqrt(M) / 2));
			CASE(subtask = 2, N = MXN2, M = MXM2, decrements = fixedSumArray(M / 4, M / 2 - (int)sqrt(M) / 2), T = smartArray(decrements, (int)sqrt(M) / 2, (int)sqrt(M) / 2));
			
			// Case (24) smart with limiting N
			CASE(subtask = 2, N = 8, M = MXM2, decrements = fixedSumArray(10, M / 2 - M / 20), T = smartArray(decrements, M / 20, M / 20));
			
			// Case (25) smart with unbalanced decrements
			CASE(subtask = 2, N = MXN2, M = MXM2, decrements = fixedSumArray(M / 4, M / 4 + (int)sqrt(M) * 5), decrements[0] += M / 4 - (int)sqrt(M) * 5, T = smartArray(decrements, 0, 0));
		}
		void TestGroup3()
		{
			Subtasks({3});
			
			// Case (1-3) rand
			CASE(subtask = 3, N = rnd.nextInt(1, MXN3), M = rnd.nextInt(1, MXM), T = rndArray(M, 0, 1));
			CASE(subtask = 3, N = rnd.nextInt(1, MXN3), M = MXM, T = rndArray(M, 0, 1));
			CASE(subtask = 3, N = MXN3, M = MXM, T = rndArray(M, 0, 1));
			
			// Case (4) small N
			CASE(subtask = 3, N = 1, M = MXM, T = rndArray(M, 0, 1));
			
			// Case (5-6) 1 segment
			CASE(subtask = 3, N = MXN3, M = MXM, T = rndArray(M, 0, 0));
			CASE(subtask = 3, N = MXN3, M = MXM, T = rndArray(M, 1, 1));
			
			// Case (7-10) 2 segments
			CASE(subtask = 3, N = MXN3, M = MXM, T = segmentedArray({{0, M / 2}, {1, M / 2}}));
			CASE(subtask = 3, N = MXN3, M = MXM, T = segmentedArray({{0, M / 2 - (int)sqrt(M) / 2}, {1, M / 2 + (int)sqrt(M) / 2}}), T = rndFlip(T, sqrt(M) / 2));
			CASE(subtask = 3, N = MXN3, M = MXM, T = segmentedArray({{0, M - M / 3}, {1, M / 3}}));
			CASE(subtask = 3, N = MXN3, M = MXM, T = segmentedArray({{1, M / 2}, {0, M / 2}}));
			
			// Case (11) 4 segments
			CASE(subtask = 3, N = MXN3, M = MXM, T = segmentedArray({{0, M / 4}, {1, M / 8}, {0, M / 4}, {1, M * 3 / 8}}));
			
			// Case (12-13) many segments
			CASE(subtask = 3, N = MXN3, M = MXM, T = alternatingArray(M, 1));
			CASE(subtask = 3, N = MXN3, M = MXM, T = alternatingArray(M, 2));
			
			// Case (14-16) small M
			CASE(subtask = 3, N = rnd.nextInt(1, MXN3), M = 1, T = {0});
			CASE(subtask = 3, N = rnd.nextInt(1, MXN3), M = 1, T = {1});
			CASE(subtask = 3, N = rnd.nextInt(1, MXN3), M = 2, T = {0, 1});
			
			// Case (17-19) many segments turns into few segments
			CASE(subtask = 3, N = MXN3, M = MXM, T = rndArray(M / 3, 0, 0), T = addMaxSegments(M, T));
			CASE(subtask = 3, N = MXN3, M = MXM, T = rndArray(M / 3, 1, 1), T = addMaxSegments(M, T));
			CASE(subtask = 3, N = MXN3, M = MXM, T = segmentedArray({{0, M / 6}, {1, M / 6}}), T = addMaxSegments(M, T));
			
			// Case (20-23) smart (T generated from number of decrements in each iteration)
			CASE(subtask = 3, N = MXN3, M = MXM, decrements = fixedSumArray(10, M / 2 - M / 20), T = smartArray(decrements, M / 20, M / 20));
			CASE(subtask = 3, N = MXN3, M = MXM, decrements = fixedSumArray(sqrt(M), M / 2 - (int)sqrt(M) / 2), T = smartArray(decrements, (int)sqrt(M) / 2, (int)sqrt(M) / 2));
			CASE(subtask = 3, N = MXN3, M = MXM, decrements = fixedSumArray(M / 10, M / 2 - (int)sqrt(M) / 2), T = smartArray(decrements, (int)sqrt(M) / 2, (int)sqrt(M) / 2));
			CASE(subtask = 3, N = MXN3, M = MXM, decrements = fixedSumArray(M / 4, M / 2 - (int)sqrt(M) / 2), T = smartArray(decrements, (int)sqrt(M) / 2, (int)sqrt(M) / 2));
			
			// Case (24) smart with limiting N
			CASE(subtask = 3, N = 8, M = MXM, decrements = fixedSumArray(10, M / 2 - M / 20), T = smartArray(decrements, M / 20, M / 20));
			
			// Case (25) smart with unbalanced decrements
			CASE(subtask = 3, N = MXN3, M = MXM, decrements = fixedSumArray(M / 4, M / 4 + (int)sqrt(M) * 5), decrements[0] += M / 4 - (int)sqrt(M) * 5, T = smartArray(decrements, 0, 0));
		}
		void TestGroup4()
		{
			Subtasks({4});
			
			// Case (1-2) rand
			CASE(subtask = 4, N = rnd.nextInt(1, MXN), M = rnd.nextInt(1, MXM), T = rndArray(M, 0, 1));
			CASE(subtask = 4, N = rnd.nextInt(1, MXN), M = MXM, T = rndArray(M, 0, 1));
			
			// Case (3-4) small N
			CASE(subtask = 4, N = 200, M = MXM, T = rndArray(M, 0, 1));
			CASE(subtask = 4, N = 1, M = MXM, T = rndArray(M, 0, 1));
			
			// Case (5-6) 1 segment
			CASE(subtask = 4, N = MXN, M = MXM, T = rndArray(M, 0, 0));
			CASE(subtask = 4, N = MXN, M = MXM, T = rndArray(M, 1, 1));
			
			// Case (7-10) 2 segments
			CASE(subtask = 4, N = MXN, M = MXM, T = segmentedArray({{0, M / 2}, {1, M / 2}}));
			CASE(subtask = 4, N = MXM / 2 - sqrt(MXM) * 3, M = MXM, T = segmentedArray({{0, M / 2 - (int)sqrt(M) / 2}, {1, M / 2 + (int)sqrt(M) / 2}}), T = rndFlip(T, sqrt(M) / 2));
			CASE(subtask = 4, N = MXN, M = MXM, T = segmentedArray({{0, M - M / 3}, {1, M / 3}}));
			CASE(subtask = 4, N = MXN, M = MXM, T = segmentedArray({{1, M / 2}, {0, M / 2}}));
			
			// Case (11) 4 segments
			CASE(subtask = 4, N = MXM * 3 / 8 - sqrt(MXM), M = MXM, T = segmentedArray({{0, M / 4}, {1, M / 8}, {0, M / 4}, {1, M * 3 / 8}}));
			
			// Case (12-13) many segments
			CASE(subtask = 4, N = MXN, M = MXM, T = alternatingArray(M, 1));
			CASE(subtask = 4, N = MXN, M = MXM, T = alternatingArray(M, 2));
			
			// Case (14-16) small M
			CASE(subtask = 4, N = rnd.nextInt(1, MXN), M = 1, T = {0});
			CASE(subtask = 4, N = rnd.nextInt(1, MXN), M = 1, T = {1});
			CASE(subtask = 4, N = rnd.nextInt(1, MXN), M = 2, T = {0, 1});
			
			// Case (17-19) many segments turns into few segments
			CASE(subtask = 4, N = MXN, M = MXM, T = rndArray(M / 3, 0, 0), T = addMaxSegments(M, T));
			CASE(subtask = 4, N = MXN, M = MXM, T = rndArray(M / 3, 1, 1), T = addMaxSegments(M, T));
			CASE(subtask = 4, N = MXN, M = MXM, T = segmentedArray({{0, M / 6}, {1, M / 6}}), T = addMaxSegments(M, T));
			
			// Case (20-23) smart (T generated from number of decrements in each iteration)
			CASE(subtask = 4, N = MXN, M = MXM, decrements = fixedSumArray(10, M / 2 - M / 20), T = smartArray(decrements, M / 20, M / 20));
			CASE(subtask = 4, N = MXN, M = MXM, decrements = fixedSumArray(sqrt(M), M / 2 - (int)sqrt(M) / 2), T = smartArray(decrements, (int)sqrt(M) / 2, (int)sqrt(M) / 2));
			CASE(subtask = 4, N = MXN, M = MXM, decrements = fixedSumArray(M / 10, M / 2 - (int)sqrt(M) / 2), T = smartArray(decrements, (int)sqrt(M) / 2, (int)sqrt(M) / 2));
			CASE(subtask = 4, N = MXN, M = MXM, decrements = fixedSumArray(M / 4, M / 2 - (int)sqrt(M) / 2), T = smartArray(decrements, (int)sqrt(M) / 2, (int)sqrt(M) / 2));
			
			// Case (24-27) smart with limiting N
			CASE(subtask = 4, N = 8, M = MXM, decrements = fixedSumArray(10, M / 2 - M / 20), T = smartArray(decrements, M / 20, M / 20));
			CASE(subtask = 4, N = sqrt(MXM) - sqrt(sqrt(MXM)), M = MXM, decrements = fixedSumArray(sqrt(M), M / 2 - (int)sqrt(M) / 2), T = smartArray(decrements, (int)sqrt(M) / 2, (int)sqrt(M) / 2));
			CASE(subtask = 4, N = M / 10 - sqrt(MXM), M = MXM, decrements = fixedSumArray(M / 10, M / 2 - (int)sqrt(M) / 2), T = smartArray(decrements, (int)sqrt(M) / 2, (int)sqrt(M) / 2));
			CASE(subtask = 4, N = M / 4 - sqrt(MXM), M = MXM, decrements = fixedSumArray(M / 4, M / 2 - (int)sqrt(M) / 2), T = smartArray(decrements, (int)sqrt(M) / 2, (int)sqrt(M) / 2));
			
			// Case (28) smart with unbalanced decrements
			CASE(subtask = 4, N = MXN, M = MXM, decrements = fixedSumArray(M / 4, M / 4 + (int)sqrt(M) * 5), decrements[0] += M / 4 - (int)sqrt(M) * 5, T = smartArray(decrements, 0, 0));
		}

	private:
		vector<int> rndArray(int sz, int lo, int hi)
		{
			vector<int> ret;
			
			for(int i = 0; i < sz; i++)
			{
				ret.push_back(rnd.nextInt(lo, hi));
			}
			return ret;
		}
		vector<int> segmentedArray(vector<pair<int, int>> segments)
		{
			vector<int> ret;
			
			for(int i = 0; i < segments.size(); i++)
			{
				int val, cnt;
				
				val = segments[i].fr;
				cnt = segments[i].sc;
				ret.insert(ret.end(), cnt, val);
			}
			return ret;
		}
		vector<int> alternatingArray(int sz, int cnt)
		{
			vector<int> ret;
			
			for(int i = 0; i < sz; i++)
			{
				if(i % (cnt * 2) < cnt)
				{
					ret.push_back(0);
				}
				else
				{
					ret.push_back(1);
				}
			}
			return ret;
		}
		vector<int> increasingArray(int sz, int lo, int hi)
		{
			vector<int> ret;
			
			for(int i = 0; i < sz; i++)
			{
				ret.push_back(rnd.nextInt(lo, hi - (sz - 1)));
			}
			sort(ret.begin(), ret.end());
			for(int i = 0; i < sz; i++)
			{
				ret[i] += i;
			}
			return ret;
		}
		vector<int> fixedSumArray(int sz, int sum)
		{
			vector<int> vals, ret;
			
			vals = increasingArray(sz - 1, 1, sum - 1);
			vals.push_back(0);
			vals.push_back(sum);
			sort(vals.begin(), vals.end());
			for(int i = 0; i < sz; i++)
			{
				ret.push_back(vals[i + 1] - vals[i]);
			}
			return ret;
		}
		vector<int> rndFlip(vector<int> curV, int cnt)
		{
			for(int i = 0; i < cnt; i++)
			{
				int p = rnd.nextInt(0, curV.size() - 1);
				
				curV[p] ^= 1;
			}
			return curV;
		}
		vector<int> addMaxSegments(int sz, vector<int> curV)
		{
			vector<int> ret;
			
			for(int i = 0; i < sz; i++)
			{
				if(i % 3 == 0)
				{
					ret.push_back(0);
				}
				else if(i % 3 == 1)
				{
					ret.push_back(1);
				}
				else
				{
					ret.push_back(curV[i / 3]);
				}
			}
			return ret;
		}
		vector<int> smartArray(vector<int> curV, int cnt1, int cnt0)
		{
			int len, last;
			vector<pair<int, int>> segments;
			
			// initialize final irreducible array
			len = cnt1 + cnt0;
			if(cnt1 > 0)
			{
				segments.push_back({1, cnt1});
			}
			if(cnt0 > 0)
			{
				segments.push_back({0, cnt0});
			}
			
			// do increments to get initial array (reversed solution)
			sort(curV.begin(), curV.end());
			last = 0;
			for(int i = 0; i < curV.size(); i++)
			{
				int j, dec, p, p2, offset;
				vector<int> pos, tmp;
				vector<pair<int, int>> tmp2;
				
				// generate which positions to insert [0, 1] pairs equiprobably
				// also handles necessary positions (insertion positions from last increment)
				dec = curV[i] - last;
				pos = increasingArray(dec, 0, len - last);
				j = 0;
				p = 0;
				offset = 0;
				for(int i = 0; i < pos.size(); i++)
				{
					while(j < segments.size() && p + segments[j].sc <= pos[i] + offset)
					{
						p += segments[j].sc;
						if(segments[j].fr == 0 && j + 1 < segments.size())
						{
							tmp.push_back(p);
							offset++;
						}
						j++;
					}
					tmp.push_back(pos[i] + offset);
				}
				while(j < segments.size())
				{
					p += segments[j].sc;
					if(segments[j].fr == 0 && j + 1 < segments.size())
					{
						tmp.push_back(p);
						offset++;
					}
					j++;
				}
				pos = tmp;
				
				// insert [0, 1] into appropriate positions
				j = 0;
				p = 0;
				p2 = 0;
				for(int i = 0; i < pos.size(); i++)
				{
					while(j < segments.size() && p + segments[j].sc <= pos[i])
					{
						p += segments[j].sc;
						if(p > p2)
						{
							tmp2.push_back({segments[j].fr, p - p2});
							p2 = p;
						}
						j++;
					}
					if(pos[i] > p2)
					{
						tmp2.push_back({segments[j].fr, pos[i] - p2});
						p2 = pos[i];
					}
					tmp2.push_back({0, 1});
					tmp2.push_back({1, 1});
				}
				while(j < segments.size())
				{
					p += segments[j].sc;
					if(p > p2)
					{
						tmp2.push_back({segments[j].fr, p - p2});
						p2 = p;
					}
					j++;
				}
				segments = tmp2;
				
				// merge consecutive segments with the same value
				tmp2.clear();
				for(int i = 0; i < segments.size(); i++)
				{
					if(tmp2.empty() || segments[i].fr != tmp2.back().fr)
					{
						tmp2.push_back(segments[i]);
					}
					else
					{
						tmp2.back().sc += segments[i].sc;
					}
				}
				segments = tmp2;
				
				len += curV[i] * 2;
				last = curV[i];
			}
			
			return segmentedArray(segments);
		}
};
