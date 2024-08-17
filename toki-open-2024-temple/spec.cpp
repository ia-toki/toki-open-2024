#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
#include <tcframe/spec.hpp>
#include <tcrand/graph.hpp>
#include <tcrand/tree.hpp>

using namespace std;
using namespace __gnu_pbds;
using namespace tcframe;
using namespace tcrand;

#define mp make_pair
#define fr first
#define sc second

template <class T>
using ordered_set = tree<T, null_type, less<T>, rb_tree_tag, tree_order_statistics_node_update>;

const int MXN = 1e5, MXM = 2e5, MXW = 1e9;
const int MXN1 = 1e3;
const int MXN2 = 1e3, MXM2 = 2e3, CNTW2 = 2;
const int MXN3 = 1e3, MXM3 = 2e3, CNTW3 = 10;
const int MXN4 = 1e3, MXM4 = 2e3;

class ProblemSpec : public BaseProblemSpec
{
	protected:
		int N, M, K;
		vector<int> U, V, W, P;
		long long ans;
		
		void InputFormat()
		{
			LINE(N, M, K);
			LINES(U, V, W) % SIZE(M);
			LINE(P % SIZE(K));
		}
		void OutputFormat()
		{
			LINE(ans);
		}
		void GradingConfig()
		{
			TimeLimit(2);
			MemoryLimit(512);
		}
		void Constraints()
		{
			CONS(2 <= N && N <= MXN);
			CONS(N - 1 <= M && M <= min((long long)MXM, (long long)N * (N - 1) / 2));
			CONS(1 <= K && K <= N - 1);
			CONS(eachElementBetween(U, 0, N - 1));
			CONS(eachElementBetween(V, 0, N - 1));
			CONS(noSelfLoopsOrMultiEdges(U, V));
			CONS(isConnected(N, U, V));
			CONS(eachElementBetween(W, 1, MXW));
			CONS(eachElementBetween(P, 1, N - 1));
			CONS(isDistinct(P));
		}
		void Subtask1()
		{
			Points(6);
			CONS(N <= MXN1);
			CONS(M == N - 1);
			CONS(isLineGraph(U, V));
			CONS(K == 1);
			CONS(P[0] == N - 1);
		}
		void Subtask2()
		{
			Points(10);
			CONS(N <= MXN2);
			CONS(M <= MXM2);
			CONS(cntDifferentVals(W, CNTW2));
		}
		void Subtask3()
		{
			Points(14);
			CONS(N <= MXN3);
			CONS(M <= MXM3);
			CONS(cntDifferentVals(W, CNTW3));
		}
		void Subtask4()
		{
			Points(27);
			CONS(N <= MXN4);
			CONS(M <= MXM4);
		}
		void Subtask5()
		{
			Points(29);
			CONS(K == 1);
			CONS(P[0] == N - 1);
		}
		void Subtask6()
		{
			Points(14);
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
		bool noSelfLoopsOrMultiEdges(vector<int> &curU, vector<int> &curV)
		{
			vector<pair<int, int>> edges;
			
			for(int i = 0; i < curU.size(); i++)
			{
				if(!(curU[i] < curV[i]))
				{
					return false;
				}
				edges.push_back({curU[i], curV[i]});
			}
			sort(edges.begin(), edges.end());
			for(int i = 0; i < edges.size() - 1; i++)
			{
				if(edges[i] == edges[i + 1])
				{
					return false;
				}
			}
			return true;
		}
		
		int dsu[MXN + 69];
		int unionFind(int x)
		{
			if(dsu[x] != x)
			{
				dsu[x] = unionFind(dsu[x]);
			}
			return dsu[x];
		}
		bool isConnected(int curN, vector<int> &curU, vector<int> &curV)
		{
			int cnt = 0;
			
			for(int i = 0; i < curN; i++)
			{
				dsu[i] = i;
			}
			for(int i = 0; i < curU.size(); i++)
			{
				int k, l;

				k = curU[i];
				l = curV[i];
				if(unionFind(k) != unionFind(l))
				{
					dsu[unionFind(l)] = unionFind(k);
					cnt++;
				}
			}
			return cnt == curN - 1;
		}
		
		bool isDistinct(vector<int> &curV)
		{
			vector<int> tmp = curV;
			
			sort(tmp.begin(), tmp.end());
			for(int i = 0; i < tmp.size() - 1; i++)
			{
				if(tmp[i] == tmp[i + 1])
				{
					return false;
				}
			}
			return true;
		}
		bool isLineGraph(vector<int> &curU, vector<int> &curV)
		{
			for(int i = 0; i < curU.size(); i++)
			{
				if(!(curU[i] == i && curV[i] == i + 1))
				{
					return false;
				}
			}
			return true;
		}
		bool cntDifferentVals(vector<int> &curV, int hi)
		{
			vector<int> tmp = curV;
			
			sort(tmp.begin(), tmp.end());
			tmp.erase(unique(tmp.begin(), tmp.end()), tmp.end());
			return tmp.size() <= hi;
		}
};

class TestSpec : public BaseTestSpec<ProblemSpec>
{
	protected:
		void SampleTestCase1()
		{
			Subtasks({3, 4, 6});
			Input
			({
				"7 8 2",
				"0 1 6",
				"1 2 1",
				"2 5 1",
				"3 5 3",
				"3 4 3",
				"0 4 4",
				"1 3 5",
				"3 6 2",
				"5 6",
			});
			Output
			({
				"6",
			});
		}
		
		bool doShuffle, lastNodeExit;
		int cntV, par[MXN * 2 + 69];
		vector<int> adj[MXN * 2 + 69], exits;
		void BeforeTestCase()
		{
			U.clear();
			V.clear();
			W.clear();
			P.clear();
			
			doShuffle = true;
			lastNodeExit = false;
			
			cntV = 0;
			for(int i = 0; i < MXN * 2; i++)
			{
				adj[i].clear();
			}
			exits.clear();
		}
		void AfterTestCase()
		{
			for(int i = 0; i < M; i++)
			{
				if(U[i] > V[i])
				{
					swap(U[i], V[i]);
				}
			}
			if(doShuffle)
			{
				int perm[N + 69];
				
				for(int i = 0; i < N; i++)
				{
					perm[i] = i;
				}
				if(!lastNodeExit)
				{
					rnd.shuffle(perm + 1, perm + N);
				}
				else
				{
					rnd.shuffle(perm + 1, perm + N - 1);
					swap(perm[P[0]], perm[N - 1]);
				}
				for(int i = 0; i < M; i++)
				{
					int p;
					
					U[i] = perm[U[i]];
					V[i] = perm[V[i]];
					if(U[i] > V[i])
					{
						swap(U[i], V[i]);
					}
					
					p = rnd.nextInt(0, i);
					swap(U[i], U[p]);
					swap(V[i], V[p]);
					swap(W[i], W[p]);
				}
				for(int i = 0; i < K; i++)
				{
					P[i] = perm[P[i]];
				}
				rnd.shuffle(P.begin(), P.end());
			}
		}
		
		void TestGroup1()
		{
			// small tests for all subs
			Subtasks({1, 2, 3, 4, 5, 6});
			
			// Case (1-2)
			CASE(N = 2, M = 1, K = 1, U = {0}, V = {1}, W = {MXW}, P = {1}, doShuffle = false);
			CASE(N = 3, M = 2, K = 1, U = {0, 1}, V = {1, 2}, W = {MXW, MXW}, P = {2}, doShuffle = false);
			CASE(N = 3, M = 2, K = 1, U = {0, 1}, V = {1, 2}, W = {MXW, MXW - 1}, P = {2}, doShuffle = false);
			CASE(N = 3, M = 2, K = 1, U = {0, 1}, V = {1, 2}, W = {MXW - 1, MXW}, P = {2}, doShuffle = false);

			// Case (3) all same W test for sub 1
			CASE(N = MXN1, M = N - 1, K = 1, lineGraph(N), W = rndArray(M, MXW, MXW), P = {N - 1}, doShuffle = false);
		}
		void TestGroup2()
		{
			// sub 1
			Subtasks({1, 4, 5, 6});

			// Case (1-2) rand values
			CASE(N = rnd.nextInt(1, MXN1), M = N - 1, K = 1, lineGraph(N), W = rndArray(M, 1, MXW), P = {N - 1}, doShuffle = false);
			CASE(N = MXN1, M = N - 1, K = 1, lineGraph(N), W = rndArray(M, 1, MXW), P = {N - 1}, doShuffle = false);

			// Case (3) max ans
			CASE(N = MXN1, M = N - 1, K = 1, maxAnsGraph(N, MXW), P = {N - 1}, doShuffle = false);

			// Case (4) increasing
			CASE(N = MXN1, M = N - 1, K = 1, lineGraph(N), W = increasingArray(M, 1, MXW), P = {N - 1}, doShuffle = false);

			// Case (5) decreasing
			CASE(N = MXN1, M = N - 1, K = 1, lineGraph(N), W = decreasingArray(M, 1, MXW), P = {N - 1}, doShuffle = false);

			// Case (6-7) alternating decreasing
			CASE(N = MXN1, M = N - 1, K = 1, lineGraph(N), W = alternatingDecreasingArray(M, 1, MXW), P = {N - 1}, doShuffle = false);
			CASE(N = MXN1, M = N - 1, K = 1, lineGraph(N), W = alternatingDecreasingArray2(M, 1, MXW), P = {N - 1}, doShuffle = false);

			// Case (8-10) half half
			CASE(N = MXN1, M = N - 1, K = 1, lineGraph(N), W = decreasingIncreasingArray(M, 1, MXW), P = {N - 1}, doShuffle = false);
			CASE(N = MXN1, M = N - 1, K = 1, lineGraph(N), W = decreasingIncreasingDisjointArray(M, 1, MXW), P = {N - 1}, doShuffle = false);
			CASE(N = MXN1, M = N - 1, K = 1, lineGraph(N), W = decreasingDecreasingDisjointArray(M, 1, MXW), P = {N - 1}, doShuffle = false);
		}
		void TestGroup3()
		{
			// sub 2
			Subtasks({2, 3, 4, 6});
			
			// Case (1-4)
			CASE(N = 3, M = 3, K = 1, U = {0, 0, 1}, V = {1, 2, 2}, W = {MXW, MXW, MXW - 1}, P = {1}, doShuffle = false);
			CASE(N = 3, M = 3, K = 1, U = {0, 0, 1}, V = {1, 2, 2}, W = {MXW, MXW - 1, MXW - 1}, P = {1}, doShuffle = false);
			CASE(N = 3, M = 3, K = 2, U = {0, 0, 1}, V = {1, 2, 2}, W = {MXW, MXW - 1, MXW}, P = {1, 2}, doShuffle = false);
			CASE(N = 3, M = 3, K = 2, U = {0, 0, 1}, V = {1, 2, 2}, W = {MXW - 1, MXW, MXW - 1}, P = {1, 2}, doShuffle = false);
			
			// Case (5-6) makes incorrect DSU TLE
			CASE(N = MXN2, M = MXM2, K = sqrt(N), antiWrongDSUCompressW(N, M, 1, MXW, CNTW2), P = distinctArray(K, N - K, N - 1), doShuffle = false);
			CASE(N = MXN2, M = MXM2, K = sqrt(N), antiWrongDSUCompressW(N, M, 1, MXW, CNTW2), reverseVertices(), reverseEdges(), P = distinctArray(K, N - K, N - 1), doShuffle = false);
			
			// Case (7-9) rand graph
			CASE(N = rnd.nextInt(MXN2 / 2, MXN2), M = rnd.nextInt(N - 1, MXM2), K = rnd.nextInt(1, N - 1), rndGraph(N, M), W = rndArray(M, 1, MXW), P = distinctArray(K, 1, N - 1), compressW(CNTW2, 1, MXW));
			CASE(N = MXN2, M = MXM2, K = sqrt(N), rndGraph(N, M), W = rndArray(M, 1, MXW), P = distinctArray(K, 1, N - 1), compressW(CNTW2, 1, MXW));
			CASE(N = MXN2, M = MXM2, K = N / 3, rndGraph(N, M), W = rndArray(M, 1, MXW), P = distinctArray(K, 1, N - 1), compressW(CNTW2, 1, MXW));
			
			// Case (10-11) rand graph with controlled weights to force many steps in the dijkstra
			CASE(N = MXN2, M = MXM2, K = 1, rndGraph(N, M), manyStepsW(N, M, MXW, 5), P = distinctArray(K, N - K, N - 1), compressW(CNTW2, 1, MXW));
			CASE(N = MXN2, M = MXM2, K = N - 1, rndGraph(N, M), manyStepsW(N, M, MXW, 5), P = distinctArray(K, N - K, N - 1), compressW(CNTW2, 1, MXW));
			
			// Case (12-14) line graph
			CASE(N = MXN2, M = N - 1, K = sqrt(N), lineGraph(N), W = rndArray(M, 1, MXW), P = distinctArray(K, N - K, N - 1), compressW(CNTW2, 1, MXW));
			CASE(N = MXN2, M = N - 1, K = N / 3, lineGraph(N), W = rndArray(M, 1, MXW), P = distinctArray(K, N - K, N - 1), compressW(CNTW2, 1, MXW));
			CASE(N = MXN2, M = N - 1, K = 1, maxAnsGraphCompressW(N, MXW, CNTW2), P = distinctArray(K, N - K, N - 1));
			
			// Case (15) line graph plus extra edges
			CASE(N = MXN2, M = MXM2, K = sqrt(N), lineGraphPlus(N, M), manyStepsW(N, M, MXW, 100), P = distinctArray(K, N - K, N - 1), compressW(CNTW2, 1, MXW));
			
			// Case (16) complete graph
			CASE(N = 63, M = N * (N - 1) / 2, K = sqrt(N), rndGraph(N, M), W = rndArray(M, 1, MXW), P = distinctArray(K, 1, N - 1), compressW(CNTW2, 1, MXW));
			
			// Case (17) star graph plus extra edges
			CASE(N = MXN2, M = MXM2, K = N / 3, starGraphPlus(N, M), manyStepsW(N, M, MXW, 5), P = distinctArray(K, N - K, N - 1), compressW(CNTW2, 1, MXW));
			
			// Case (18) star at vertex 0 and star at vertex N - 1
			CASE(N = MXN2, M = N * 2 - 4, K = 1, twoStarsGraphCompressW(N, 1, MXW, CNTW2), P = distinctArray(K, N - K, N - 1));
			
			// Case (19-22) needs two steps
			CASE(N = MXN2, M = MXM2, K = 1, twoStepsGraph(N, M, K, 1, MXW));
			CASE(N = MXN2, M = MXM2, K = sqrt(N), twoStepsGraph(N, M, K, 1, MXW));
			CASE(N = MXN2, M = MXM2, K = N / 3, twoStepsGraph(N, M, K, 1, MXW));
			CASE(N = MXN2, M = MXM2, K = N - 2, twoStepsGraph(N, M, K, 1, MXW));
			
			// Case (23-24) the optimal path skips an exit before exiting
			CASE(N = 4, M = 3, K = 1, U = {0, 1, 2}, V = {1, 2, 3}, W = {MXW, MXW - 1, MXW}, P = {2}, doShuffle = false);
			CASE(N = MXN2, M = MXM2, K = sqrt(N), skipExitGraphCompressW(N, M, 1, MXW, CNTW2), P = distinctArray(K, N - K, N - 1));
			
			// Case (25) all same W, smart
			CASE(N = MXN2, M = MXM2, K = sqrt(N), allSameWSmartGraph(N, M, K, MXW), P = distinctArray(K, N - K, N - 1));
		}
		void TestGroup4()
		{
			// small tests for sub 5
			Subtasks({3, 4, 5, 6});
			
			// Case (1-2)
			CASE(N = 3, M = 3, K = 1, U = {0, 0, 1}, V = {1, 2, 2}, W = {MXW - 1, MXW, MXW - 2}, P = {2}, doShuffle = false);
			CASE(N = 3, M = 3, K = 1, U = {0, 0, 1}, V = {1, 2, 2}, W = {MXW - 2, MXW, MXW - 1}, P = {2}, doShuffle = false);
			
			// Case (3) the optimal path skips an exit before exiting
			CASE(N = 4, M = 4, K = 1, U = {0, 0, 1, 2}, V = {1, 3, 3, 3}, W = {MXW - 1, MXW, MXW - 2, MXW - 1}, P = {3}, doShuffle = false);
		}
		void TestGroup5()
		{
			// sub 3
			Subtasks({3, 4, 6});
			
			// Case (1-4)
			CASE(N = 3, M = 3, K = 1, U = {0, 0, 1}, V = {1, 2, 2}, W = {MXW, MXW - 1, MXW - 2}, P = {1}, doShuffle = false);
			CASE(N = 3, M = 3, K = 1, U = {0, 0, 1}, V = {1, 2, 2}, W = {MXW, MXW - 2, MXW - 1}, P = {1}, doShuffle = false);
			CASE(N = 3, M = 3, K = 2, U = {0, 0, 1}, V = {1, 2, 2}, W = {MXW - 2, MXW - 1, MXW}, P = {1, 2}, doShuffle = false);
			CASE(N = 3, M = 3, K = 2, U = {0, 0, 1}, V = {1, 2, 2}, W = {MXW, MXW - 1, MXW - 2}, P = {1, 2}, doShuffle = false);
			
			// Case (5) counter for wrong extension of sub 5 solution
			CASE(N = 8, M = 12, K = 2, U = {0, 0, 0, 1, 1, 1, 2, 2, 3, 4, 5, 6}, V = {1, 4, 5, 2, 4, 5, 3, 6, 7, 5, 6, 7}, W = {60000000, 80000000, 90000000, 50000000, 90000000, 80000000, 40000000, 120000000, 140000000, 70000000, 30000000, 20000000}, P = {3, 7});
			
			// Case (6-7) makes incorrect DSU TLE
			CASE(N = MXN3, M = MXM3, K = sqrt(N), antiWrongDSUCompressW(N, M, 1, MXW, CNTW3), P = distinctArray(K, N - K, N - 1), doShuffle = false);
			CASE(N = MXN3, M = MXM3, K = sqrt(N), antiWrongDSUCompressW(N, M, 1, MXW, CNTW3), reverseVertices(), reverseEdges(), P = distinctArray(K, N - K, N - 1), doShuffle = false);
			
			// Case (8) predetermined reachability tree
			CASE(N = MXN3, M = MXM3, K = sqrt(N), tallReachabilityTree(N, K, sqrt(N)), reachibilityTreeGraph(N, M, 1, MXW), compressW(CNTW3, 1, MXW));
			
			// Case (9-11) predetermined reachability tree with max distances
			CASE(N = MXN3, M = N - 1, K = sqrt(N), rndReachabilityTree(N, K, sqrt(N)), reachibilityTreeGraphMaxDistance(N, M, 1, MXW, MXW), compressW(CNTW3, 1, MXW));
			CASE(N = MXN3, M = N - 1, K = sqrt(N), tallReachabilityTree(N, K, sqrt(N)), reachibilityTreeGraphMaxDistance(N, M, 1, MXW, MXW), compressW(CNTW3, 1, MXW));
			CASE(N = MXN3, M = N - 1, K = N / 3, tallReachabilityTree(N, K, sqrt(N)), reachibilityTreeGraphMaxDistance(N, M, 1, MXW, MXW), compressW(CNTW3, 1, MXW));
			
			// Case (12-14) rand graph
			CASE(N = rnd.nextInt(MXN3 / 2, MXN3), M = rnd.nextInt(N - 1, MXM3), K = rnd.nextInt(1, N - 1), rndGraph(N, M), W = rndArray(M, 1, MXW), P = distinctArray(K, 1, N - 1), compressW(CNTW3, 1, MXW));
			CASE(N = MXN3, M = MXM3, K = sqrt(N), rndGraph(N, M), W = rndArray(M, 1, MXW), P = distinctArray(K, 1, N - 1), compressW(CNTW3, 1, MXW));
			CASE(N = MXN3, M = MXM3, K = N / 3, rndGraph(N, M), W = rndArray(M, 1, MXW), P = distinctArray(K, 1, N - 1), compressW(CNTW3, 1, MXW));
			
			// Case (15-16) rand graph with controlled weights to force many steps in the dijkstra
			CASE(N = MXN3, M = MXM3, K = 1, rndGraph(N, M), manyStepsW(N, M, MXW, 5), P = distinctArray(K, N - K, N - 1), compressW(CNTW3, 1, MXW));
			CASE(N = MXN3, M = MXM3, K = N - 1, rndGraph(N, M), manyStepsW(N, M, MXW, 5), P = distinctArray(K, N - K, N - 1), compressW(CNTW3, 1, MXW));
			
			// Case (17-19) line graph
			CASE(N = MXN3, M = N - 1, K = sqrt(N), lineGraph(N), W = rndArray(M, 1, MXW), P = distinctArray(K, N - K, N - 1), compressW(CNTW3, 1, MXW));
			CASE(N = MXN3, M = N - 1, K = N / 3, lineGraph(N), W = rndArray(M, 1, MXW), P = distinctArray(K, N - K, N - 1), compressW(CNTW3, 1, MXW));
			CASE(N = MXN3, M = N - 1, K = 1, maxAnsGraphCompressW(N, MXW, CNTW3), P = distinctArray(K, N - K, N - 1));
			
			// Case (20-23) line graph plus extra edges
			CASE(N = MXN3, M = MXM3, K = sqrt(N), lineGraphPlus(N, M), manyStepsW(N, M, MXW, 100), P = distinctArray(K, N - K, N - 1), compressW(CNTW3, 1, MXW));
			CASE(N = MXN3, M = MXM3, K = sqrt(N), lineGraphPlusShort(N, M), manyStepsW(N, M, MXW, 100), P = distinctArray(K, N - K, N - 1), compressW(CNTW3, 1, MXW));
			CASE(N = MXN3, M = MXM3, K = N / 3, lineGraphPlusShort(N, M), manyStepsW(N, M, MXW, 100), P = distinctArray(K, N - K, N - 1), compressW(CNTW3, 1, MXW));
			CASE(N = MXN3, M = MXM3, K = 1, maxAnsGraphPlusShortCompressW(N, M, MXW, CNTW3), P = distinctArray(K, N - K, N - 1));
			
			// Case (24) complete graph
			CASE(N = 63, M = N * (N - 1) / 2, K = sqrt(N), rndGraph(N, M), W = rndArray(M, 1, MXW), P = distinctArray(K, 1, N - 1), compressW(CNTW3, 1, MXW));
			
			// Case (25-26) two lines
			CASE(N = MXN3, M = N, K = sqrt(N), twoLinesGraph(N), W = rndArray(M, 1, MXW), P = distinctArray(K, N - K, N - 1), compressW(CNTW3, 1, MXW));
			CASE(N = MXN3, M = N, K = N / 3, twoLinesGraph(N), W = rndArray(M, 1, MXW), P = distinctArray(K, N - K, N - 1), compressW(CNTW3, 1, MXW));
			
			// Case (27-28) two lines plus extra edges
			CASE(N = MXN3, M = MXM3, K = sqrt(N), twoLinesGraphPlusShort(N, M), manyStepsW(N, M, MXW, 100), P = distinctArray(K, N - K, N - 1), compressW(CNTW3, 1, MXW));
			CASE(N = MXN3, M = MXM3, K = N / 3, twoLinesGraphPlusShort(N, M), manyStepsW(N, M, MXW, 100), P = distinctArray(K, N - K, N - 1), compressW(CNTW3, 1, MXW));
			
			// Case (29) star graph plus extra edges
			CASE(N = MXN3, M = MXM3, K = N / 3, starGraphPlus(N, M), manyStepsW(N, M, MXW, 5), P = distinctArray(K, N - K, N - 1), compressW(CNTW3, 1, MXW));
			
			// Case (30) star at vertex 0 and star at vertex N - 1
			CASE(N = MXN3, M = N * 2 - 4, K = 1, twoStarsGraphCompressW(N, 1, MXW, CNTW3), P = distinctArray(K, N - K, N - 1));
			
			// Case (31) there is a spanning tree with the minimum weight
			CASE(N = MXN3, M = MXM3, K = sqrt(N), minSpanningTreeGraph(N, M, 1, MXW), P = distinctArray(K, 1, N - 1), compressW(CNTW3, MXW / 2, MXW));
			
			// Case (32-33) the optimal path uses many edges more than once
			CASE(N = (MXN3 - 1) / 3 * 3 + 1, M = N / 3 * 6, K = sqrt(N), notSimplePathGraphCompressW(N, 1, MXW, CNTW3), P = distinctArray(K, N - K, N - 1));
			CASE(N = (MXN3 - 1) / 3 * 3 + 1, M = N / 3 * 6, K = N / 3, notSimplePathGraphCompressW(N, 1, MXW, CNTW3), P = distinctArray(K, N - K, N - 1));
			
			// Case (34) line graph with complete graph at the end
			CASE(N = MXN3, M = N - 46 + 46 * 45 / 2, K = sqrt(46), lineCompleteGraph(N, 46, 1, MXW), P = distinctArray(K, N - K, N - 1), compressW(CNTW3, 1, MXW));
			
			// Case (35-36) the optimal path skips an exit before exiting
			CASE(N = 4, M = 4, K = 1, U = {0, 0, 1, 2}, V = {1, 2, 2, 3}, W = {MXW - 1, MXW, MXW - 2, MXW - 1}, P = {2}, doShuffle = false);
			CASE(N = MXN3, M = MXM3, K = sqrt(N), skipExitGraphCompressW(N, M, 1, MXW, CNTW3), P = distinctArray(K, N - K, N - 1));
		}
		void TestGroup6()
		{
			// sub 4
			Subtasks({4, 6});
			
			// Case (1-2) makes incorrect DSU TLE
			CASE(N = MXN4, M = MXM4, K = sqrt(N), antiWrongDSU(N, M, 1, MXW), P = distinctArray(K, N - K, N - 1), doShuffle = false);
			CASE(N = MXN4, M = MXM4, K = sqrt(N), antiWrongDSU(N, M, 1, MXW), reverseVertices(), reverseEdges(), P = distinctArray(K, N - K, N - 1), doShuffle = false);
			
			// Case (3) predetermined reachability tree
			CASE(N = MXN4, M = MXM4, K = sqrt(N), tallReachabilityTree(N, K, sqrt(N)), reachibilityTreeGraph(N, M, 1, MXW));
			
			// Case (4-8) predetermined reachability tree with max distances
			CASE(N = MXN4, M = N - 1, K = sqrt(N), rndReachabilityTree(N, K, sqrt(N)), reachibilityTreeGraphMaxDistance(N, M, 1, MXW, MXW));
			CASE(N = MXN4, M = N - 1, K = sqrt(N), tallReachabilityTree(N, K, sqrt(N)), reachibilityTreeGraphMaxDistance(N, M, 1, MXW, MXW));
			CASE(N = MXN4, M = N - 1, K = N / 3, tallReachabilityTree(N, K, sqrt(N)), reachibilityTreeGraphMaxDistance(N, M, 1, MXW, MXW));
			CASE(N = MXN4, M = MXM4, K = sqrt(N), rndReachabilityTree(N, K, sqrt(N)), reachibilityTreeGraphMaxDistance(N, M, 1, M, MXW));
			CASE(N = MXN4, M = MXM4, K = sqrt(N), tallReachabilityTree(N, K, sqrt(N)), reachibilityTreeGraphMaxDistance(N, M, 1, M, MXW));
			
			// Case (9-11) rand graph
			CASE(N = rnd.nextInt(MXN4 / 2, MXN4), M = rnd.nextInt(N - 1, MXM4), K = rnd.nextInt(1, N - 1), rndGraph(N, M), W = rndArray(M, 1, MXW), P = distinctArray(K, 1, N - 1));
			CASE(N = MXN4, M = MXM4, K = sqrt(N), rndGraph(N, M), W = rndArray(M, 1, MXW), P = distinctArray(K, 1, N - 1));
			CASE(N = MXN4, M = MXM4, K = N / 3, rndGraph(N, M), W = rndArray(M, 1, MXW), P = distinctArray(K, 1, N - 1));
			
			// Case (12-14) rand graph with controlled weights to force many steps in the dijkstra
			CASE(N = MXN4, M = MXM4, K = 1, rndGraph(N, M), manyStepsW(N, M, MXW, 5), P = distinctArray(K, N - K, N - 1));
			CASE(N = MXN4, M = MXM4, K = sqrt(N), rndGraph(N, M), manyStepsW(N, M, MXW, 5), P = distinctArray(K, N - K, N - 1));
			CASE(N = MXN4, M = MXM4, K = N - 1, rndGraph(N, M), manyStepsW(N, M, MXW, 5), P = distinctArray(K, N - K, N - 1));
			
			// Case (15-17) line graph
			CASE(N = MXN4, M = N - 1, K = sqrt(N), lineGraph(N), W = rndArray(M, 1, MXW), P = distinctArray(K, N - K, N - 1));
			CASE(N = MXN4, M = N - 1, K = N / 3, lineGraph(N), W = rndArray(M, 1, MXW), P = distinctArray(K, N - K, N - 1));
			CASE(N = MXN4, M = N - 1, K = 1, maxAnsGraph(N, MXW), P = distinctArray(K, N - K, N - 1));
			
			// Case (18-21) line graph plus extra edges
			CASE(N = MXN4, M = MXM4, K = sqrt(N), lineGraphPlus(N, M), manyStepsW(N, M, MXW, 100), P = distinctArray(K, N - K, N - 1));
			CASE(N = MXN4, M = MXM4, K = sqrt(N), lineGraphPlusShort(N, M), manyStepsW(N, M, MXW, 100), P = distinctArray(K, N - K, N - 1));
			CASE(N = MXN4, M = MXM4, K = N / 3, lineGraphPlusShort(N, M), manyStepsW(N, M, MXW, 100), P = distinctArray(K, N - K, N - 1));
			CASE(N = MXN4, M = MXM4, K = 1, maxAnsGraphPlusShort(N, M, MXW), P = distinctArray(K, N - K, N - 1));
			
			// Case (22) complete graph
			CASE(N = 63, M = N * (N - 1) / 2, K = sqrt(N), rndGraph(N, M), W = rndArray(M, 1, MXW), P = distinctArray(K, 1, N - 1));
			
			// Case (23-24) two lines
			CASE(N = MXN4, M = N, K = sqrt(N), twoLinesGraph(N), W = rndArray(M, 1, MXW), P = distinctArray(K, N - K, N - 1));
			CASE(N = MXN4, M = N, K = N / 3, twoLinesGraph(N), W = rndArray(M, 1, MXW), P = distinctArray(K, N - K, N - 1));
			
			// Case (25-26) two lines plus extra edges
			CASE(N = MXN4, M = MXM4, K = sqrt(N), twoLinesGraphPlusShort(N, M), manyStepsW(N, M, MXW, 100), P = distinctArray(K, N - K, N - 1));
			CASE(N = MXN4, M = MXM4, K = N / 3, twoLinesGraphPlusShort(N, M), manyStepsW(N, M, MXW, 100), P = distinctArray(K, N - K, N - 1));
			
			// Case (27) star graph plus extra edges
			CASE(N = MXN4, M = MXM4, K = N / 3, starGraphPlus(N, M), manyStepsW(N, M, MXW, 5), P = distinctArray(K, N - K, N - 1));
			
			// Case (28) star at vertex 0 and star at vertex N - 1
			CASE(N = MXN4, M = N * 2 - 4, K = 1, twoStarsGraph(N, 1, MXW), P = distinctArray(K, N - K, N - 1));
			
			// Case (29-30) star plus line
			CASE(N = MXN4, M = N * 2 - 3, K = sqrt(N), starLineGraph(N, 15e5 + 1, MXW, N / 5), P = distinctArray(K, N - K, N - 1));
			CASE(N = MXN4, M = N * 2 - 3, K = N / 3, starLineGraph(N, 15e5 + 1, MXW, N / 5), P = distinctArray(K, N - K, N - 1));
			
			// Case (31) there is a spanning tree with the minimum weight
			CASE(N = MXN4, M = MXM4, K = sqrt(N), minSpanningTreeGraph(N, M, 1, MXW), P = distinctArray(K, 1, N - 1));
			
			// Case (32) many distance array overwrites in naive dijkstra solution
			CASE(N = MXN4, K = sqrt(N), manyOverwritesGraph(N, N / 3, 1, MXW), P = distinctArray(K, N - K, N - 1));
			
			// Case (33-34) the optimal path uses many edges more than once
			CASE(N = (MXN4 - 1) / 3 * 3 + 1, M = N / 3 * 6, K = sqrt(N), notSimplePathGraph(N, 1, MXW), P = distinctArray(K, N - K, N - 1));
			CASE(N = (MXN4 - 1) / 3 * 3 + 1, M = N / 3 * 6, K = N / 3, notSimplePathGraph(N, 1, MXW), P = distinctArray(K, N - K, N - 1));
			
			// Case (35) line graph with complete graph at the end
			CASE(N = MXN4, M = N - 46 + 46 * 45 / 2, K = sqrt(46), lineCompleteGraph(N, 46, 1, MXW), P = distinctArray(K, N - K, N - 1));
			
			// Case (36) the optimal path skips an exit before exiting
			CASE(N = MXN4, M = MXM4, K = sqrt(N), skipExitGraph(N, M, 1, MXW), P = distinctArray(K, N - K, N - 1));
		}
		void TestGroup7()
		{
			// sub 5
			Subtasks({5, 6});
			
			// Case (1-2) makes incorrect DSU TLE
			CASE(N = MXN, M = MXM, K = 1, antiWrongDSU(N, M, 1, MXW), P = {N - 1}, doShuffle = false);
			CASE(N = MXN, M = MXM, K = 1, antiWrongDSU(N, M, 1, MXW), reverseVertices(), reverseEdges(), P = {N - 1}, doShuffle = false);
			
			// Case (3-4) predetermined reachability tree with max distances
			CASE(N = MXN, M = N - 1, K = 1, tallReachabilityTree(N, K, sqrt(N)), reachibilityTreeGraphMaxDistance(N, M, 1, MXW, MXW), lastNodeExit = true);
			CASE(N = MXN, M = MXM, K = 1, tallReachabilityTree(N, K, sqrt(N)), reachibilityTreeGraphMaxDistance(N, M, 1, M, MXW), lastNodeExit = true);
			
			// Case (5) rand graph with controlled weights to force many steps in the dijkstra
			CASE(N = MXN, M = MXM, K = 1, rndGraph(N, M), manyStepsW(N, M, MXW, 5), P = {N - 1}, lastNodeExit = true);
			
			// Case (6) line graph
			CASE(N = MXN, M = N - 1, K = 1, maxAnsGraph(N, MXW), P = {N - 1}, lastNodeExit = true);
			
			// Case (7-8) line graph plus extra edges
			CASE(N = MXN, M = MXM, K = 1, lineGraphPlusShort(N, M), manyStepsW(N, M, MXW, 100), P = {N - 1}, lastNodeExit = true);
			CASE(N = MXN, M = MXM, K = 1, maxAnsGraphPlusShort(N, M, MXW), P = {N - 1}, lastNodeExit = true);
			
			// Case (9) complete graph
			CASE(N = 632, M = N * (N - 1) / 2, K = 1, rndGraph(N, M), W = rndArray(M, 1, MXW), P = {N - 1}, lastNodeExit = true);
			
			// Case (10) star at vertex 0 and star at vertex N - 1
			CASE(N = MXN, M = N * 2 - 4, K = 1, twoStarsGraph(N, 1, MXW), P = {N - 1}, lastNodeExit = true);
			
			// Case (11) star plus line
			CASE(N = MXN, M = N * 2 - 3, K = 1, starLineGraph(N, 9e8 + 1, MXW, N / 20), P = {N - 1}, lastNodeExit = true);
			
			// Case (12) there is a spanning tree with the minimum weight
			CASE(N = MXN, M = MXM, K = 1, minSpanningTreeGraph(N, M, 1, MXW), P = {N - 1}, lastNodeExit = true);
			
			// Case (13) many distance array overwrites in naive dijkstra solution
			CASE(N = MXN, K = 1, manyOverwritesGraph(N, N / 3, 1, MXW), P = {N - 1}, lastNodeExit = true);
			
			// Case (14) the optimal path uses many edges more than once
			CASE(N = (MXN - 1) / 3 * 3 + 1, M = N / 3 * 6, K = 1, notSimplePathGraph(N, 1, MXW), P = {N - 1}, lastNodeExit = true);
			
			// Case (15) line graph with complete graph at the end
			CASE(N = MXN, M = N - 448 + 448 * 447 / 2, K = 1, lineCompleteGraph(N, 448, 1, MXW), P = {N - 1}, lastNodeExit = true);
		}
		void TestGroup8()
		{
			// sub 6
			Subtasks({6});
			
			// Case (1-2) makes incorrect DSU TLE
			CASE(N = MXN, M = MXM, K = sqrt(N), antiWrongDSU(N, M, 1, MXW), P = distinctArray(K, N - K, N - 1), doShuffle = false);
			CASE(N = MXN, M = MXM, K = sqrt(N), antiWrongDSU(N, M, 1, MXW), reverseVertices(), reverseEdges(), P = distinctArray(K, N - K, N - 1), doShuffle = false);
			
			// Case (3-5) predetermined reachability tree with max distances
			CASE(N = MXN, M = N - 1, K = sqrt(N), tallReachabilityTree(N, K, sqrt(N)), reachibilityTreeGraphMaxDistance(N, M, 1, MXW, MXW));
			CASE(N = MXN, M = N - 1, K = N / 3, tallReachabilityTree(N, K, sqrt(N)), reachibilityTreeGraphMaxDistance(N, M, 1, MXW, MXW));
			CASE(N = MXN, M = MXM, K = sqrt(N), tallReachabilityTree(N, K, sqrt(N)), reachibilityTreeGraphMaxDistance(N, M, 1, M, MXW));
			
			// Case (6-7) rand graph with controlled weights to force many steps in the dijkstra
			CASE(N = MXN, M = MXM, K = sqrt(N), rndGraph(N, M), manyStepsW(N, M, MXW, 5), P = distinctArray(K, N - K, N - 1));
			CASE(N = MXN, M = MXM, K = N - 1, rndGraph(N, M), manyStepsW(N, M, MXW, 5), P = distinctArray(K, N - K, N - 1));
			
			// Case (8-9) line graph plus extra edges
			CASE(N = MXN, M = MXM, K = sqrt(N), lineGraphPlusShort(N, M), manyStepsW(N, M, MXW, 100), P = distinctArray(K, N - K, N - 1));
			CASE(N = MXN, M = MXM, K = N / 3, lineGraphPlusShort(N, M), manyStepsW(N, M, MXW, 100), P = distinctArray(K, N - K, N - 1));
			
			// Case (10) complete graph
			CASE(N = 632, M = N * (N - 1) / 2, K = sqrt(N), rndGraph(N, M), W = rndArray(M, 1, MXW), P = distinctArray(K, 1, N - 1));
			
			// Case (11) star plus line
			CASE(N = MXN, M = N * 2 - 3, K = N / 3, starLineGraph(N, 9e8 + 1, MXW, N / 20), P = distinctArray(K, N - K, N - 1));
			
			// Case (12) there is a spanning tree with the minimum weight
			CASE(N = MXN, M = MXM, K = sqrt(N), minSpanningTreeGraph(N, M, 1, MXW), P = distinctArray(K, 1, N - 1));
			
			// Case (13) many distance array overwrites in naive dijkstra solution
			CASE(N = MXN, K = sqrt(N), manyOverwritesGraph(N, N / 3, 1, MXW), P = distinctArray(K, N - K, N - 1));
			
			// Case (14) the optimal path uses many edges more than once
			CASE(N = (MXN - 1) / 3 * 3 + 1, M = N / 3 * 6, K = N / 3, notSimplePathGraph(N, 1, MXW), P = distinctArray(K, N - K, N - 1));
			
			// Case (15) line graph with complete graph at the end
			CASE(N = MXN, M = N - 448 + 448 * 447 / 2, K = sqrt(448), lineCompleteGraph(N, 448, 1, MXW), P = distinctArray(K, N - K, N - 1));
		}
	
	private:
		template <class T>
		T getRandomElem(ordered_set<T>& seto)
		{
			int it = rnd.nextInt(0, (int)seto.size() - 1);
			return *seto.find_by_order(it);
		}

		vector<int> rndArray(int sz, int lo, int hi)
		{
			vector<int> ret;
			
			for(int i = 0; i < sz; i++)
			{
				ret.push_back(rnd.nextInt(lo, hi));
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
		vector<int> decreasingArray(int sz, int lo, int hi)
		{
			vector<int> ret = increasingArray(sz, lo, hi);
			
			reverse(ret.begin(), ret.end());
			return ret;
		}
		vector<int> distinctArray(int sz, int lo, int hi)
		{
			vector<int> ret = increasingArray(sz, lo, hi);
			
			rnd.shuffle(ret.begin(), ret.end());
			return ret;
		}
		vector<int> alternatingDecreasingArray(int sz, int lo, int hi)
		{
			int p = 0;
			vector<int> vals, ret;
			
			vals = increasingArray(sz, lo, hi);
			ret.resize(sz, 0);
			for(int i = sz - 2; i >= 0; i -= 2)
			{
				ret[i] = vals[p];
				p++;
			}
			for(int i = sz - 1; i >= 0; i -= 2)
			{
				ret[i] = vals[p];
				p++;
			}
			return ret;
		}
		vector<int> alternatingDecreasingArray2(int sz, int lo, int hi)
		{
			int p = 0;
			vector<int> vals, ret;
			
			vals = increasingArray(sz, lo, hi);
			ret.resize(sz, 0);
			for(int i = sz - 1; i >= 0; i -= 2)
			{
				if(i - 1 >= 0)
				{
					ret[i - 1] = vals[p];
					p++;
				}
				ret[i] = vals[p];
				p++;
			}
			return ret;
		}
		vector<int> decreasingIncreasingArray(int sz, int lo, int hi)
		{
			vector<int> tmp, ret;
			
			ret = decreasingArray(sz / 2, lo, hi);
			tmp = increasingArray(sz - sz / 2, lo, hi);
			ret.insert(ret.end(), tmp.begin(), tmp.end());
			return ret;
		}
		vector<int> decreasingIncreasingDisjointArray(int sz, int lo, int hi)
		{
			vector<int> ret = increasingArray(sz, lo, hi);
			
			reverse(ret.begin(), ret.begin() + sz / 2);
			return ret;
		}
		vector<int> decreasingDecreasingDisjointArray(int sz, int lo, int hi)
		{
			vector<int> ret = increasingArray(sz, lo, hi);
			
			reverse(ret.begin(), ret.begin() + sz / 2);
			reverse(ret.begin() + sz / 2, ret.end());
			return ret;
		}
		void rndGraph(int curN, int curM)
		{
			GraphRandomizer graph_rand;
			tie(U, V) = graph_rand.node_count(curN).edge_count(curM).index_base(0).next().edges();
		}
		void lineGraph(int curN)
		{
			for(int i = 0; i < curN - 1; i++)
			{
				U.push_back(i);
				V.push_back(i + 1);
			}
		}
		void maxAnsGraph(int curN, int hi)
		{
			lineGraph(curN);
			for(int i = 0; i < curN - 1; i++)
			{
				W.push_back(hi - i);
			}
		}
		void lineGraphPlus(int curN, int curM)
		{
			rndGraph(curN - 1, curM - (curN - 1));
			for(int i = 0; i < U.size(); i++)
			{
				if(U[i] > V[i])
				{
					swap(U[i], V[i]);
				}
				V[i]++;
			}
			for(int i = 0; i < curN - 1; i++)
			{
				U.push_back(i);
				V.push_back(i + 1);
			}
		}
		void lineGraphPlusShort(int curN, int curM)
		{
			int len = 1;
			
			while(U.size() < curM)
			{
				for(int i = 0; i < curN - len; i++)
				{
					U.push_back(i);
					V.push_back(i + len);
					if(U.size() >= curM)
					{
						break;
					}
				}
				len++;
			}
		}
		void maxAnsGraphPlusShort(int curN, int curM, int hi)
		{
			int len = 1;
			
			while(U.size() < curM)
			{
				for(int i = 0; i < curN - len; i++)
				{
					U.push_back(i);
					V.push_back(i + len);
					W.push_back(hi - i);
					if(U.size() >= curM)
					{
						break;
					}
				}
				len++;
			}
		}
		void twoLinesGraph(int curN)
		{
			for(int i = 0; i < curN; i++)
			{
				U.push_back(max(i - 1, 0));
				V.push_back(min(i + 1, curN - 1));
			}
		}
		void twoLinesGraphPlusShort(int curN, int curM)
		{
			int len = 3;
			
			twoLinesGraph(curN);
			while(U.size() < curM)
			{
				for(int i = 0; i < curN - len; i++)
				{
					U.push_back(i);
					V.push_back(i + len);
					if(U.size() >= curM)
					{
						break;
					}
				}
				len++;
			}
		}
		void starGraph(int curN)
		{
			for(int i = 0; i < curN - 1; i++)
			{
				U.push_back(i);
				V.push_back(curN - 1);
			}
		}
		void starGraphPlus(int curN, int curM)
		{
			rndGraph(curN - 1, curM - (curN - 1));
			for(int i = 0; i < curN - 1; i++)
			{
				U.push_back(i);
				V.push_back(curN - 1);
			}
		}
		void twoStarsGraph(int curN, int lo, int hi)
		{
			int mid = (lo + hi) / 2;
			
			for(int i = 1; i < curN - 1; i++)
			{
				U.push_back(0);
				V.push_back(i);
				W.push_back(rnd.nextInt(mid + 1, hi));
				U.push_back(i);
				V.push_back(curN - 1);
				W.push_back(rnd.nextInt(lo, mid));
			}
		}
		void starLineGraph(int curN, int lo, int hi, int cnt)
		{
			for(int i = 0; i < curN - 1; i++)
			{
				U.push_back(i);
				V.push_back(i + 1);
				W.push_back(curN - 1 - i);
				if(i + 1 != curN - 1)
				{
					U.push_back(i);
					V.push_back(curN - 1);
					if(i < cnt || rnd.nextInt(0, 1))
					{
						W.push_back(rnd.nextInt(lo, hi));
					}
					else
					{
						W.push_back(rnd.nextInt(1, curN - 1 - cnt));
					}
				}
			}
		}
		
		int dsu[MXN + 69];
		int unionFind(int x)
		{
			if(dsu[x] != x)
			{
				dsu[x] = unionFind(dsu[x]);
			}
			return dsu[x];
		}
		void minSpanningTreeGraph(int curN, int curM, int lo, int hi)
		{
			int mn = rnd.nextInt(lo, hi);
			
			rndGraph(curN, curM);
			for(int i = 0; i < curM; i++)
			{
				int p = rnd.nextInt(0, i);
				
				swap(U[i], U[p]);
				swap(V[i], V[p]);
			}
			for(int i = 0; i < curN; i++)
			{
				dsu[i] = i;
			}
			for(int i = 0; i < curM; i++)
			{
				if(unionFind(U[i]) != unionFind(V[i]))
				{
					W.push_back(mn);
					dsu[unionFind(V[i])] = unionFind(U[i]);
				}
				else
				{
					W.push_back(rnd.nextInt(mn + 1, hi));
				}
			}
		}
		void twoStepsGraph(int curN, int curM, int curK, int lo, int hi)
		{
			// determine the two weights
			int w, w2;
			w = rnd.nextInt(lo, hi);
			w2 = rnd.nextInt(lo, hi - 1);
			if(w2 >= w)
			{
				w2++;
			}
			else
			{
				swap(w, w2);
			}
			
			// determine exits
			bool isExit[curN + 69];
			P = distinctArray(curK, 1, curN - 1);
			for(int i = 0; i < curN; i++)
			{
				isExit[i] = false;
			}
			for(int i = 0; i < curK; i++)
			{
				isExit[P[i]] = true;
			}
			
			// determine vertices adjacent to vertex 0
			int deg0 = rnd.nextInt(1, min(10, curN - 1 - K));
			vector<int> nonExits;
			for(int i = 1; i < curN; i++)
			{
				if(!isExit[i])
				{
					nonExits.push_back(i);
				}
			}
			rnd.shuffle(nonExits.begin(), nonExits.end());
			nonExits.resize(deg0);
			
			// generate all edges
			rndGraph(curN - 1, curM - deg0);
			for(int i = 0; i < U.size(); i++)
			{
				U[i]++;
				V[i]++;
			}
			for(int i = 0; i < deg0; i++)
			{
				U.push_back(0);
				V.push_back(nonExits[i]);
			}
			for(int i = 0; i < curM; i++)
			{
				int p = rnd.nextInt(0, i);
				swap(U[i], U[p]);
				swap(V[i], V[p]);
			}
			
			// initialize DSU
			for(int i = 0; i < curN; i++)
			{
				dsu[i] = i;
			}
			
			// add edges that are adjacent to exits
			W.resize(curM, 0);
			for(int i = 0; i < curM; i++)
			{
				if(isExit[U[i]] || isExit[V[i]])
				{
					W[i] = w;
					if(!isExit[U[i]])
					{
						swap(U[i], V[i]);
					}
					dsu[unionFind(V[i])] = unionFind(U[i]);
				}
			}
			
			// add edges with small weight and big weight
			for(int i = 0; i < curM; i++)
			{
				if(W[i] == 0)
				{
					if(!(unionFind(U[i]) != unionFind(V[i]) && (unionFind(U[i]) == unionFind(0) || unionFind(V[i]) == unionFind(0)) && (isExit[unionFind(U[i])] || isExit[unionFind(V[i])])))
					{
						W[i] = w;
						if(!isExit[unionFind(U[i])])
						{
							swap(U[i], V[i]);
						}
						dsu[unionFind(V[i])] = unionFind(U[i]);
					}
					else
					{
						W[i] = w2;
					}
				}
			}
		}
		
		void manyOverwritesGraph(int curN, int cnt, int lo, int hi)
		{
			vector<int> vals = decreasingArray(curN - 1, lo, hi);
			
			for(int i = 0; i < cnt; i++)
			{
				int p = cnt + (cnt - 1 - i);
				
				if(p - 2 > i)
				{
					U.push_back(p - 2);
					V.push_back(p);
					W.push_back(vals[i]);
				}
				if(p - 1 > i)
				{
					U.push_back(i);
					V.push_back(p - 1);
					W.push_back(vals[i]);
				}
				if(p - 2 > i)
				{
					U.push_back(i);
					V.push_back(p - 2);
					W.push_back(vals[i]);
				}
				if(p - 3 > i)
				{
					U.push_back(i);
					V.push_back(p - 3);
					W.push_back(vals[i]);
				}
			}
			for(int i = cnt; i < curN - 1; i++)
			{
				U.push_back(i);
				V.push_back(i + 1);
				W.push_back(vals[i]);
			}
			M = U.size();
		}
		void notSimplePathGraph(int curN, int lo, int hi)
		{
			vector<int> vals = decreasingArray(curN / 3 * 4, lo, hi);
			
			for(int i = 0; i < curN / 3; i++)
			{
				U.push_back(i * 3 + 1);
				V.push_back(i * 3 + 3);
				W.push_back(vals[i * 4]);
				U.push_back(i * 3);
				V.push_back(i * 3 + 1);
				W.push_back(vals[i * 4 + 1]);
				U.push_back(i * 3 + 2);
				V.push_back(i * 3 + 3);
				W.push_back(vals[i * 4 + 2]);
				U.push_back(i * 3);
				V.push_back(i * 3 + 2);
				W.push_back(vals[i * 4 + 3]);
				U.push_back(i * 3);
				V.push_back(i * 3 + 3);
				W.push_back(vals[i * 4 + 3]);
				U.push_back(i * 3 + 1);
				V.push_back(i * 3 + 2);
				W.push_back(vals[i * 4 + 3]);
			}
		}
		void lineCompleteGraph(int curN, int cnt, int lo, int hi)
		{
			int p;
			vector<int> vals = decreasingArray(curN - cnt, lo, hi);
			
			for(int i = 0; i < curN - cnt; i++)
			{
				U.push_back(i);
				V.push_back(i + 1);
				W.push_back(vals[i]);
			}
			p = rnd.nextInt(0, curN - cnt);
			for(int i = curN - cnt; i < curN; i++)
			{
				for(int j = i + 1; j < curN; j++)
				{
					U.push_back(i);
					V.push_back(j);
					W.push_back(rnd.nextInt(lo, vals[p]));
				}
			}
		}
		void antiWrongDSU(int curN, int curM, int lo, int hi)
		{
			int p, deg[curN / 2 + 69];
			vector<int> vals = increasingArray(curM, lo, hi);
			
			p = 0;
			for(int i = curN - 4; i >= 0; i -= 2)
			{
				U.push_back(i);
				V.push_back(i + 2);
				W.push_back(vals[p]);
				p++;
			}
			
			for(int i = 0; i < curN / 2; i++)
			{
				deg[i] = 0;
			}
			for(int i = 0; i < curM - (curN - 1); i++)
			{
				int k = rnd.nextInt(0, curN / 2 - 1);
				
				deg[k]++;
			}
			
			int carry = 0;
			
			for(int i = 0; i < curN / 2; i++)
			{
				deg[i] += carry;
				carry = max(deg[i] - i * 2, 0);
				deg[i] = min(deg[i], i * 2);
			}
			for(int i = curN / 2 - 1; i >= 0; i--)
			{
				deg[i] += carry;
				carry = max(deg[i] - i * 2, 0);
				deg[i] = min(deg[i], i * 2);
			}
			
			for(int i = 0; i < curN; i += 2)
			{
				U.push_back(i);
				V.push_back(i + 1);
				W.push_back(vals[p]);
				p++;
				
				if(deg[i / 2] > 0)
				{
					vector<int> pos = distinctArray(deg[i / 2], 0, i - 1);
					
					for(int j = 0; j < deg[i / 2]; j++)
					{
						U.push_back(pos[j]);
						V.push_back(i + 1);
						W.push_back(vals[p]);
						p++;
					}
				}
			}
		}
		void manyStepsW(int curN, int curM, int hi, int c)
		{
			vector<int> diffs;
			vector<long long> vals;
			long long mx = 0;
			int div;
			
			diffs = rndArray(curN - 1, 1, hi);
			sort(diffs.begin(), diffs.end(), greater<int>());
			vals.push_back(0);
			for(int i = 0; i < curN - 1; i++)
			{
				vals.push_back(vals[i] + diffs[i]);
			}
			for(int i = 0; i < curM; i++)
			{
				long long w = abs(vals[U[i]] - vals[V[i]]) * ((abs(U[i] - V[i]) - 1) / c + 1);
				
				mx = max(mx, w);
			}
			div = (mx - 1) / hi + 1;
			W.clear();
			for(int i = 0; i < curM; i++)
			{
				long long w = abs(vals[U[i]] - vals[V[i]]) * ((abs(U[i] - V[i]) - 1) / c + 1);
				
				W.push_back(max(w / div, 1ll));
			}
		}
		void rndReachabilityTree(int curN, int curK, int extra)
		{
			// generate random reachability tree
			for(int i = 1; i < curN - extra; i++)
			{
				par[i] = rnd.nextInt(0, i - 1);
				adj[par[i]].push_back(i);
			}
			
			// find all leaves
			// for each non-leaf, add a leaf directly to it
			ordered_set<int> leaves;
			cntV = curN - extra;
			for(int i = 0; i < curN - extra; i++)
			{
				if(adj[i].empty())
				{
					exits.push_back(i);
				}
				else
				{
					par[cntV] = i;
					adj[i].push_back(cntV);
					leaves.insert(cntV);
					cntV++;
				}
			}
			
			// select exits from initial leaves
			rnd.shuffle(exits.begin(), exits.end());
			exits.resize(curK);
			
			// split up a few newly added leaves
			for(int i = 0; i < extra; i++)
			{
				int k = getRandomElem(leaves);
				leaves.erase(k);
				par[cntV] = k;
				adj[k].push_back(cntV);
				leaves.insert(cntV);
				cntV++;
				par[cntV] = k;
				adj[k].push_back(cntV);
				leaves.insert(cntV);
				cntV++;
			}
			
			// reverse adjacency list
			for(int i = 0; i < cntV; i++)
			{
				reverse(adj[i].begin(), adj[i].end());
			}
		}
		void tallReachabilityTree(int curN, int curK, int extra)
		{
			// determine which vertex will be branching
			bool isBranch[curN + 69];
			vector<int> branches;
			for(int i = 0; i < curN; i++)
			{
				isBranch[i] = false;
			}
			branches = distinctArray(curK - 1, 2, curN - extra - 1);
			for(int i = 0; i < branches.size(); i++)
			{
				isBranch[branches[i]] = true;
			}
			
			// generate reachibility tree based on isBranch array
			// branching vertices makes a new branch for a new leaf
			// non-branching vertices will only extend a leaf chain
			ordered_set<int> leaves, nonLeaves;
			leaves.insert(0);
			for(int i = 1; i < curN - extra; i++)
			{
				if(!isBranch[i])
				{
					par[i] = getRandomElem(leaves);
					adj[par[i]].push_back(i);
					leaves.erase(par[i]);
					nonLeaves.insert(par[i]);
					leaves.insert(i);
				}
				else
				{
					par[i] = getRandomElem(nonLeaves);
					adj[par[i]].push_back(i);
					leaves.insert(i);
				}
			}
			
			// select exits from initial leaves
			for(int i = 0; i < curK; i++)
			{
				int k = getRandomElem(leaves);
				exits.push_back(k);
				leaves.erase(k);
			}
			
			// for each non-leaf, add a leaf directly to it
			leaves.clear();
			cntV = curN - extra;
			for(int i = 0; i < curN - extra; i++)
			{
				if(!adj[i].empty())
				{
					par[cntV] = i;
					adj[i].push_back(cntV);
					leaves.insert(cntV);
					cntV++;
				}
			}
			
			// split up a few newly added leaves
			for(int i = 0; i < extra; i++)
			{
				int k = getRandomElem(leaves);
				leaves.erase(k);
				par[cntV] = k;
				adj[k].push_back(cntV);
				leaves.insert(cntV);
				cntV++;
				par[cntV] = k;
				adj[k].push_back(cntV);
				leaves.insert(cntV);
				cntV++;
			}
			
			// reverse adjacency list
			for(int i = 0; i < cntV; i++)
			{
				reverse(adj[i].begin(), adj[i].end());
			}
		}
		
		int cntEuler, eulerTour[MXN * 2 + 69], eulerPos[MXN * 2 + 69], subSize[MXN * 2 + 69];
		int weight[MXN * 2 + 69];
		void getEulerTour(int x)
		{
			subSize[x] = 0;
			for(int i = 0; i < adj[x].size(); i++)
			{
				int l = adj[x][i];
				getEulerTour(l);
				subSize[x] += subSize[l];
			}
			if(adj[x].empty())
			{
				eulerTour[cntEuler] = x;
				cntEuler++;
				subSize[x]++;
			}
			eulerPos[x] = cntEuler;
		}
		void reachibilityTreeGraph(int curN, int curM, int lo, int hi)
		{
			// euler tour preprocessing
			cntEuler = 0;
			getEulerTour(0);
			
			// get values for edge weights in descending order
			vector<int> vals = decreasingArray(curM, lo, hi);
			
			// iterate the tree randomly from ancestors to descendants to assign edge weights
			int p = 0;
			ordered_set<int> roots;
			roots.insert(0);
			while(!roots.empty())
			{
				// get random vertex with no parent
				int k, sz;
				k = getRandomElem(roots);
				sz = adj[k].size();
				roots.erase(k);
				
				if(sz > 0)
				{
					// generate spanning tree connecting the children of k
					vector<int> curU, curV;
					GraphRandomizer graph_rand;
					tie(curU, curV) = graph_rand.node_count(sz).edge_count(sz - 1).index_base(0).next().edges();
					
					// make edges based on the spanning tree above
					weight[k] = vals[p];
					p++;
					for(int i = 0; i < sz - 1; i++)
					{
						int u, v;
						u = adj[k][curU[i]];
						v = adj[k][curV[i]];
						
						U.push_back(rnd.nextInt(eulerPos[u] - subSize[u], eulerPos[u] - 1));
						V.push_back(rnd.nextInt(eulerPos[v] - subSize[v], eulerPos[v] - 1));
						W.push_back(weight[k]);
					}
					
					// add children of k into roots
					for(int i = 0; i < sz; i++)
					{
						roots.insert(adj[k][i]);
					}
				}
			}
			
			// mark which edges are there
			map<pair<int, int>, bool> isEdge;
			for(int i = 0; i < U.size(); i++)
			{
				if(U[i] > V[i])
				{
					swap(U[i], V[i]);
				}
				isEdge[{U[i], V[i]}] = true;
			}
			
			// add extra edges that do not contribute to the reachability tree
			while(U.size() < curM)
			{
				// get random vertex in the reachability tree
				int k = rnd.nextInt(0, cntV - 1);
				if(subSize[k] == 1)
				{
					continue;
				}
				
				// determine the edge weight
				int w, curHi;
				if(k == 0)
				{
					curHi = hi;
				}
				else
				{
					curHi = weight[par[k]];
				}
				w = rnd.nextInt(weight[k], curHi);
				
				// get the two vertices for the edge
				int u, v;
				u = rnd.nextInt(rnd.nextInt(eulerPos[k] - subSize[k], eulerPos[k] - 1));
				v = rnd.nextInt(rnd.nextInt(eulerPos[k] - subSize[k], eulerPos[k] - 2));
				if(v >= u)
				{
					v++;
				}
				else
				{
					swap(u, v);
				}
				
				// add edge
				if(!isEdge[{u, v}])
				{
					U.push_back(u);
					V.push_back(v);
					W.push_back(w);
					isEdge[{u, v}] = true;
				}
			}
			
			// get real labels of exits
			for(int i = 0; i < exits.size(); i++)
			{
				P.push_back(eulerPos[exits[i]] - 1);
			}
		}
		void reachibilityTreeGraphMaxDistance(int curN, int curM, int lo, int hi, int hi2)
		{
			// euler tour preprocessing
			cntEuler = 0;
			getEulerTour(0);
			
			// get values for edge weights in descending order
			vector<int> vals = decreasingArray(curM, lo, hi);
			
			// iterate the tree randomly from ancestors to descendants to assign edge weights
			int p = 0;
			ordered_set<int> roots;
			roots.insert(0);
			while(!roots.empty())
			{
				// get random vertex with no parent
				int k, sz;
				k = getRandomElem(roots);
				sz = adj[k].size();
				roots.erase(k);
				
				if(sz > 0)
				{
					// generate spanning tree connecting the children of k
					vector<int> curU, curV;
					GraphRandomizer graph_rand;
					tie(curU, curV) = graph_rand.node_count(sz).edge_count(sz - 1).index_base(0).next().edges();
					
					// make edges based on the spanning tree above
					weight[k] = vals[p];
					p++;
					for(int i = 0; i < sz - 1; i++)
					{
						int u, v;
						u = adj[k][curU[i]];
						v = adj[k][curV[i]];
						
						U.push_back(eulerPos[u] - subSize[u]);
						V.push_back(eulerPos[v] - subSize[v]);
						W.push_back(weight[k]);
					}
					
					// add children of k into roots
					for(int i = 0; i < sz; i++)
					{
						roots.insert(adj[k][i]);
					}
				}
			}
			
			// mark which edges are there
			map<pair<int, int>, bool> isEdge;
			for(int i = 0; i < U.size(); i++)
			{
				if(U[i] > V[i])
				{
					swap(U[i], V[i]);
				}
				isEdge[{U[i], V[i]}] = true;
			}
			
			// add extra edges that do not contribute to the reachability tree
			for(int i = 0; i < curN; i++)
			{
				for(int j = 0; j < i; j++)
				{
					if(!isEdge[{j, i}])
					{
						U.push_back(j);
						V.push_back(i);
						W.push_back(hi2);
					}
					if(U.size() >= curM)
					{
						break;
					}
				}
				if(U.size() >= curM)
				{
					break;
				}
			}
			
			// get real labels of exits
			for(int i = 0; i < exits.size(); i++)
			{
				P.push_back(eulerPos[exits[i]] - 1);
			}
		}
		
		void skipExitGraph(int curN, int curM, int lo, int hi)
		{
			int mid = ((long long)hi * 9 + lo) / 10;
			
			rndGraph(curN - 2, curM - 2);
			for(int i = 0; i < U.size(); i++)
			{
				int w;
				
				U[i] += 2;
				V[i] += 2;
				w = rnd.nextInt(lo, hi - 1);
				if(w >= mid)
				{
					w++;
				}
				W.push_back(w);
			}
			U.push_back(0);
			V.push_back(2);
			W.push_back(mid);
			U.push_back(1);
			V.push_back(N - 1);
			W.push_back(mid);
		}
		void allSameWSmartGraph(int curN, int curM, int curK, int w)
		{
			int deg0 = rnd.nextInt(1, min(10, curN - 1 - curK));
			vector<int> adj0 = distinctArray(deg0, 1, curN - 1 - curK);
			
			rndGraph(curN - 1, curM - deg0);
			for(int i = 0; i < U.size(); i++)
			{
				U[i]++;
				V[i]++;
			}
			for(int i = 0; i < deg0; i++)
			{
				U.push_back(0);
				V.push_back(adj0[i]);
			}
			
			W = rndArray(curM, w, w);
		}
		
		void reverseVertices()
		{
			for(int i = 0; i < M; i++)
			{
				U[i] = N - 1 - U[i];
				V[i] = N - 1 - V[i];
			}
		}
		void reverseEdges()
		{
			for(int i = 0; i < M - 1 - i; i++)
			{
				swap(U[i], U[M - 1 - i]);
				swap(V[i], V[M - 1 - i]);
				swap(W[i], W[M - 1 - i]);
			}
		}
		void maxAnsGraphCompressW(int curN, int hi, int cntW)
		{
			lineGraph(curN);
			for(int i = 0; i < curN - 1; i++)
			{
				W.push_back(hi - i / (curN / cntW + 1));
			}
		}
		void maxAnsGraphPlusShortCompressW(int curN, int curM, int hi, int cntW)
		{
			int len = 1;
			
			while(U.size() < curM)
			{
				for(int i = 0; i < curN - len; i++)
				{
					U.push_back(i);
					V.push_back(i + len);
					W.push_back(min(hi - i / (curN / (cntW - 1) + 1) - 2 + len, hi));
					if(U.size() >= curM)
					{
						break;
					}
				}
				len++;
			}
		}
		void twoStarsGraphCompressW(int curN, int lo, int hi, int cntW)
		{
			int mid = (cntW + 1) / 2 - 1;
			vector<int> vals = increasingArray(cntW, lo, hi);
			
			for(int i = 1; i < curN - 1; i++)
			{
				U.push_back(0);
				V.push_back(i);
				W.push_back(vals[rnd.nextInt(mid + 1, cntW - 1)]);
				U.push_back(i);
				V.push_back(curN - 1);
				W.push_back(vals[rnd.nextInt(0, mid)]);
			}
		}
		void notSimplePathGraphCompressW(int curN, int lo, int hi, int cntW)
		{
			int p;
			vector<int> vals, vals2;
			
			vals = decreasingArray(cntW, lo, hi);
			p = rnd.nextInt(0, curN / 3 * 4 - cntW);
			for(int i = 0; i < p; i++)
			{
				vals2.push_back(vals[0]);
			}
			for(int i = 0; i < cntW; i++)
			{
				vals2.push_back(vals[i]);
			}
			for(int i = p + cntW; i < curN / 3 * 4; i++)
			{
				vals2.push_back(vals[cntW - 1]);
			}
			for(int i = 0; i < curN / 3; i++)
			{
				U.push_back(i * 3 + 1);
				V.push_back(i * 3 + 3);
				W.push_back(vals2[i * 4]);
				U.push_back(i * 3);
				V.push_back(i * 3 + 1);
				W.push_back(vals2[i * 4 + 1]);
				U.push_back(i * 3 + 2);
				V.push_back(i * 3 + 3);
				W.push_back(vals2[i * 4 + 2]);
				U.push_back(i * 3);
				V.push_back(i * 3 + 2);
				W.push_back(vals2[i * 4 + 3]);
				U.push_back(i * 3);
				V.push_back(i * 3 + 3);
				W.push_back(vals2[i * 4 + 3]);
				U.push_back(i * 3 + 1);
				V.push_back(i * 3 + 2);
				W.push_back(vals2[i * 4 + 3]);
			}
		}
		void antiWrongDSUCompressW(int curN, int curM, int lo, int hi, int cntW)
		{
			int p, deg[curN / 2 + 69];
			vector<int> vals, vals2;
			
			vals = increasingArray(cntW, lo, hi);
			for(int i = 0; i < curN / 2 - 1; i++)
			{
				vals2.push_back(vals[rnd.nextInt(0, (cntW + 1) / 2 - 1)]);
			}
			for(int i = 0; i < curM - (curN / 2 - 1); i++)
			{
				vals2.push_back(vals[rnd.nextInt((cntW + 1) / 2, cntW - 1)]);
			}
			sort(vals2.begin(), vals2.end());
			p = 0;
			for(int i = curN - 4; i >= 0; i -= 2)
			{
				U.push_back(i);
				V.push_back(i + 2);
				W.push_back(vals2[p]);
				p++;
			}
			
			for(int i = 0; i < curN / 2; i++)
			{
				deg[i] = 0;
			}
			for(int i = 0; i < curM - (curN - 1); i++)
			{
				int k = rnd.nextInt(0, curN / 2 - 1);
				
				deg[k]++;
			}
			
			int carry = 0;
			
			for(int i = 0; i < curN / 2; i++)
			{
				deg[i] += carry;
				carry = max(deg[i] - i * 2, 0);
				deg[i] = min(deg[i], i * 2);
			}
			for(int i = curN / 2 - 1; i >= 0; i--)
			{
				deg[i] += carry;
				carry = max(deg[i] - i * 2, 0);
				deg[i] = min(deg[i], i * 2);
			}
			
			for(int i = 0; i < curN; i += 2)
			{
				U.push_back(i);
				V.push_back(i + 1);
				W.push_back(vals2[p]);
				p++;
				
				if(deg[i / 2] > 0)
				{
					vector<int> pos = distinctArray(deg[i / 2], 0, i - 1);
					
					for(int j = 0; j < deg[i / 2]; j++)
					{
						U.push_back(pos[j]);
						V.push_back(i + 1);
						W.push_back(vals2[p]);
						p++;
					}
				}
			}
		}
		void skipExitGraphCompressW(int curN, int curM, int lo, int hi, int cntW)
		{
			int mid = (cntW * 9 - 1) / 10;
			vector<int> vals = increasingArray(cntW, lo, hi);
			
			rndGraph(curN - 2, curM - 2);
			for(int i = 0; i < U.size(); i++)
			{
				int p;
				
				U[i] += 2;
				V[i] += 2;
				p = rnd.nextInt(0, cntW - 2);
				if(p >= mid)
				{
					p++;
				}
				W.push_back(vals[p]);
			}
			U.push_back(0);
			V.push_back(2);
			W.push_back(vals[mid]);
			U.push_back(1);
			V.push_back(N - 1);
			W.push_back(vals[mid]);
		}
		void compressW(int cntW, int lo, int hi)
		{
			vector<int> vals = increasingArray(cntW, lo, hi);
			
			for(int i = 0; i < W.size(); i++)
			{
				int mn, w;
				
				mn = hi;
				for(int j = 0; j < cntW; j++)
				{
					int diff = abs(W[i] - vals[j]);
					
					if(diff < mn)
					{
						mn = diff;
						w = vals[j];
					}
				}
				W[i] = w;
			}
		}
};

