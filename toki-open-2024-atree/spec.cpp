#include <bits/stdc++.h>

#include <tcframe/spec.hpp>
#include <tcrand/tree.hpp>

using namespace std;
using namespace tcrand;
using namespace tcframe;

const int MAXN = 100000;
const int MAXM = 200000;
const int MAXL = 100000;
const int MAXN_4000 = 4000;

class ProblemSpec : public BaseProblemSpec {
 protected:
  int N, M, L;
  vector<int> U, V;
  vector<int> A, S;
  long long ans;

  // For convenience
  bool doShuffle;
  vector<int> P;

  void InputFormat() {
    LINE(N, M, L);
    LINES(U, V) % SIZE(N - 1);
    LINE(A % SIZE(M));
    LINE(S % SIZE(L));
  }

  void OutputFormat() { LINE(ans); }

  void GradingConfig() {
    TimeLimit(2);
    MemoryLimit(256);
  }

  void Constraints() {
    CONS(2 <= N && N <= MAXN);
    CONS(N <= M && M <= MAXM);
    CONS(1 <= L && L <= MAXL);
    CONS(isTree(N, U, V));
    CONS(isButtons(N, M, A));
    CONS(isSequence(N, L, S));
  }

  void Subtask1() {
    Points(1);
    CONS(N == M);
    CONS(isLineTree(N, U, V));
  }

  void Subtask2() {
    Points(2);
    CONS(N == M);
  }

  void Subtask3() {
    Points(3);
    CONS(N <= MAXN_4000);
    CONS(M <= MAXN_4000);
    CONS(L <= MAXN_4000);
  }

  void Subtask4() {
    Points(4);
    CONS(isLineTree(N, U, V));
  }

  void Subtask5() { Points(5); }

 private:
  bool isTree(int N, const vector<int> &U, const vector<int> &V) {
    vector<vector<int>> adj(N);
    if (int(U.size()) != N - 1) return false;
    if (int(V.size()) != N - 1) return false;
    for (int i = 0; i < N - 1; i++) {
      if (!(0 <= U[i] && U[i] < V[i] && V[i] < N)) return false;
      adj[U[i]].push_back(V[i]);
      adj[V[i]].push_back(U[i]);
    }
    vector<int> vis(N);
    vector<int> que = {0};
    vis[0] = 1;
    for (int q = 0; q < int(que.size()); q++) {
      int u = que[q];
      for (auto v : adj[u]) {
        if (!vis[v]) {
          vis[v] = 1;
          que.push_back(v);
        }
      }
    }
    return int(que.size()) == N;
  }

  bool isButtons(int N, int M, const vector<int> &A) {
    if (int(A.size()) != M) return false;
    if (A[0] != 0) return false;
    vector<bool> appeared(N, false);
    for (int i = 0; i < M; i++) {
      if (!(0 <= A[i] && A[i] < N)) return false;
      appeared[A[i]] = true;
    }
    for (int i = 0; i < N; i++) {
      if (!appeared[i]) return false;
    }
    return true;
  }

  bool isSequence(int N, int L, const vector<int> &S) {
    if (int(S.size()) != L) return false;
    for (int i = 0; i < L; i++) {
      if (!(0 <= S[i] && S[i] < N)) return false;
      if (i > 0 && S[i - 1] == S[i]) return false;
    }
    return true;
  }

  bool isLineTree(int N, const vector<int> &U, const vector<int> &V) {
    if (int(U.size()) != N - 1) return false;
    if (int(V.size()) != N - 1) return false;
    for (int i = 0; i < N - 1; i++) {
      if (U[i] != i) return false;
      if (V[i] != i + 1) return false;
    }
    return true;
  }
};

class TestSpec : public BaseTestSpec<ProblemSpec> {
 protected:
  void SampleTestCase1() {
    Subtasks({3, 5});
    Input({
        "4 6 3",
        "1 2",
        "0 1",
        "1 3",
        "0 3 2 1 3 1",
        "0 2 3",
    });
    Output({
        "1",
    });
  }

  void SampleTestCase2() {
    Subtasks({3, 4, 5});
    Input({
        "5 9 2",
        "0 1",
        "1 2",
        "2 3",
        "3 4",
        "0 2 4 1 3 1 4 2 0",
        "4 2",
    });
    Output({
        "2",
    });
  }

  void BeforeTestCase() {
    N = M = L = ans = 0;
    U.clear();
    V.clear();
    A.clear();
    S.clear();

    doShuffle = false;
    P.clear();
  }

  void AfterTestCase() {
    U.clear();
    V.clear();
    vector<int> edgePermutation(N - 1);
    iota(begin(edgePermutation), end(edgePermutation), 0);
    if (doShuffle) {
      rnd.shuffle(begin(edgePermutation), end(edgePermutation));
    }
    for (auto i : edgePermutation) {
      U.push_back(P[i]);
      V.push_back(i + 1);
    }
    if (doShuffle) {
      vector<int> permutation(N);
      iota(begin(permutation), end(permutation), 0);
      rnd.shuffle(begin(permutation), end(permutation));
      if (permutation[A[0]] != 0) {
        int whoGot0 = -1;
        for (int i = 0; i < N; i++) {
          if (permutation[i] == 0) {
            whoGot0 = i;
            break;
          }
        }
        assert(whoGot0 != -1);
        swap(permutation[A[0]], permutation[whoGot0]);
      }
      for (int i = 0; i < N - 1; i++) {
        U[i] = permutation[U[i]];
        V[i] = permutation[V[i]];
        if (U[i] > V[i]) swap(U[i], V[i]);
      }
      for (int i = 0; i < M; i++) {
        A[i] = permutation[A[i]];
      }
      for (int i = 0; i < L; i++) {
        S[i] = permutation[S[i]];
      }
    }
    doShuffle = false;
    P.clear();
  }

  void TestGroup1() {
    // N, M, L <= MAXN_4000
    // M = N
    // U[i] = i, V[i] = i + 1

    Subtasks({1, 2, 3, 4, 5});

    const int max_n = MAXN_4000;
    const int max_n_diff = 50;

    CASE(N = M = 2; pathGraph(N); A = {0, 1}; L = 1; S = {0});
    CASE(N = M = 2; pathGraph(N); A = {0, 1}; L = 1; S = {1});
    CASE(N = M = 2; pathGraph(N); A = {0, 1}; randomSequence(max_n));
    CASE(N = M = 4; pathGraph(N); A = {0, 1, 2, 3}; randomSequence(max_n));

    for (int i = 0; i < 3; i++) {
      CASE(N = max_n; pathGraph(N); randomButtons(N);
           nearLeafSequence(N, i, 1.0 - i * 0.2));  // maximum total distance
      CASE(N = rnd.nextInt(max_n - max_n_diff, max_n); pathGraph(N);
           randomButtons(N); randomSequence(max_n));
      CASE(N = rnd.nextInt(max_n - max_n_diff, max_n); pathGraph(N);
           randomButtons(N); topKmostFrequentSequence(max_n, 2, 0.9));
      CASE(N = rnd.nextInt(max_n - max_n_diff, max_n); pathGraph(N);
           randomButtons(N); topKmostFrequentSequence(max_n, 10, 0.9));
      CASE(N = rnd.nextInt(max_n - max_n_diff, max_n); pathGraph(N);
           randomButtons(N); topKmostFrequentSequence(max_n, 100, 0.9));
    }
  }

  void TestGroup2() {
    // M = N
    // U[i] = i, V[i] = i + 1
    Subtasks({1, 2, 4, 5});

    const int max_n = MAXN;
    const int max_l = MAXL;
    const int max_n_diff = 200;

    CASE(N = max_n; pathGraph(N); randomButtons(N);
         nearLeafSequence(max_l, 0, 1.0));  // maximum total distance
    CASE(N = max_n; pathGraph(N); randomButtons(N);
         nearLeafSequence(max_l, 10, 0.8));
    CASE(N = rnd.nextInt(max_n - max_n_diff, max_n); pathGraph(N);
         randomButtons(N); randomSequence(max_l));
    CASE(N = rnd.nextInt(max_n - max_n_diff, max_n); pathGraph(N);
         randomButtons(N); topKmostFrequentSequence(max_l, 2, 0.9));
    CASE(N = rnd.nextInt(max_n - max_n_diff, max_n); pathGraph(N);
         randomButtons(N); topKmostFrequentSequence(max_l, 10, 0.9));
    CASE(N = rnd.nextInt(max_n - max_n_diff, max_n); pathGraph(N);
         randomButtons(N); topKmostFrequentSequence(max_l, 100, 0.9));
  }

  void TestGroup3() {
    // N, M, L <= MAXN_4000
    // U[i] = i, V[i] = i + 1
    Subtasks({3, 4, 5});

    const int max_n = MAXN_4000;
    const int max_n_diff = max_n / 2;

    CASE(N = 2; pathGraph(N); randomButtons(4); randomSequence(max_n));
    CASE(N = 2; pathGraph(N); randomButtons(max_n); randomSequence(max_n));
    CASE(N = rnd.nextInt(max_n - max_n_diff, max_n); pathGraph(N);
         randomButtons(max_n); nearLeafSequence(max_n, 0, 1.0));
    CASE(N = 2; pathGraph(N); kBiasedButtons(max_n, 2, 1);
         topKmostFrequentSequence(max_n, 2, 1.0));
    CASE(N = max_n / 2; pathGraph(N); M = max_n; A = vector<int>(max_n, 0);
         iota(begin(A), end(A), 0);
         for (int i = N; i < M; i++) A[i] = rnd.nextInt(N / 2, N / 2 + 1);
         rnd.shuffle(begin(A) + 1, end(A)); nearLeafSequence(max_n, 0, 1.0));

    for (int i = 0; i < 3; i++) {
      auto generateButtons = [&]() {
        if (i == 0) return randomButtons(max_n);
        if (i == 1) return nearLeafButtons(max_n, rnd.nextInt(1, 5), 0.9);
        if (i == 2) return kBiasedButtons(max_n, rnd.nextInt(2, 5), 0.9);
        return randomButtons(max_n);
      };
      CASE(N = rnd.nextInt(max_n - max_n_diff, max_n); pathGraph(N);
           generateButtons(); randomSequence(max_n));
      CASE(N = rnd.nextInt(max_n - max_n_diff, max_n); pathGraph(N);
           generateButtons(); topKmostFrequentSequence(max_n, 5, 0.9));
      CASE(N = rnd.nextInt(max_n - max_n_diff, max_n); pathGraph(N);
           generateButtons(); topKmostFrequentSequence(
               max_n, static_cast<int>(ceil(sqrt(max_n))), 0.9));
      CASE(N = rnd.nextInt(max_n - max_n_diff, max_n); pathGraph(N);
           generateButtons(); importanceSampledSequence(max_n));
      CASE(N = rnd.nextInt(max_n - max_n_diff, max_n); pathGraph(N);
           generateButtons(); nearLeafSequence(max_n, rnd.nextInt(1, 5), 0.9));
      CASE(N = rnd.nextInt(max_n - max_n_diff, max_n); pathGraph(N);
           generateButtons(); nearLeafSequence(
               max_n,
               rnd.nextInt(static_cast<int>(ceil(sqrt(max_n) / 2) / 2),
                           static_cast<int>(ceil(sqrt(max_n)))),
               0.9));
    }
  }

  void TestGroup4() {
    // U[i] = i, V[i] = i + 1
    Subtasks({4, 5});

    const int max_n = MAXN;
    const int max_m = MAXM;
    const int max_l = MAXL;
    const int max_n_diff = 200;

    CASE(N = 2; pathGraph(N); randomButtons(3); randomSequence(max_l));
    CASE(N = 2; pathGraph(N); randomButtons(max_m); randomSequence(max_l));
    CASE(N = rnd.nextInt(max_n - max_n_diff, max_n); pathGraph(N);
         randomButtons(max_m); nearLeafSequence(max_l, 0, 1.0));
    CASE(N = 2; pathGraph(N); kBiasedButtons(max_m, 2, 1);
         topKmostFrequentSequence(max_l, 2, 1.0));
    CASE(N = max_n / 2; pathGraph(N); M = max_m; A = vector<int>(M, 0);
         iota(begin(A), end(A), 0);
         for (int i = N; i < M; i++) A[i] = rnd.nextInt(N / 2, N / 2 + 1);
         rnd.shuffle(begin(A) + 1, end(A)); nearLeafSequence(max_l, 0, 1.0));

    CASE(N = rnd.nextInt(max_n - max_n_diff, max_n); pathGraph(N);
         randomButtons(max_m); randomSequence(max_l));
    CASE(N = rnd.nextInt(max_n - max_n_diff, max_n); pathGraph(N);
         nearLeafButtons(max_m, rnd.nextInt(0, 4), 0.9);
         importanceSampledSequence(max_l));

    CASE(N = rnd.nextInt(max_n - max_n_diff, max_n); pathGraph(N);
         kBiasedButtons(max_m, rnd.nextInt(1, 20), 0.9);
         topKmostFrequentSequence(max_l, 20, 0.9));
    CASE(N = rnd.nextInt(max_n - max_n_diff, max_n); pathGraph(N);
         kBiasedButtons(max_m, rnd.nextInt(1, 20), 0.9);
         topKmostFrequentSequence(max_l, static_cast<int>(ceil(sqrt(max_n))),
                                  0.9));

    CASE(N = rnd.nextInt(max_n - max_n_diff, max_n); pathGraph(N);
         kBiasedButtons(max_m, rnd.nextInt(1, 20), 0.9);
         nearLeafSequence(max_l, rnd.nextInt(1, 20), 0.9));
    CASE(N = rnd.nextInt(max_n - max_n_diff, max_n); pathGraph(N);
         kBiasedButtons(max_m, rnd.nextInt(1, 20), 0.9); nearLeafSequence(
             max_l,
             rnd.nextInt(static_cast<int>(ceil(sqrt(max_n) / 2) / 2),
                         static_cast<int>(ceil(sqrt(max_n)))),
             0.9));
  }

  void TestGroup5() {
    // N, M, L <= MAXN_4000
    // M = N
    Subtasks({2, 3, 5});

    const int max_n = MAXN_4000;
    const int max_n_diff = 50;

    CASE(doShuffle = true; N = 4; starGraph(N); randomButtons(N);
         randomSequence(max_n));
    CASE(doShuffle = true; N = 7; randomTree(N); randomButtons(N);
         randomSequence(max_n));
    CASE(doShuffle = true; N = 14; randomTree(N); randomButtons(N);
         randomSequence(max_n));
    CASE(doShuffle = true; N = 20; randomTree(N); randomButtons(N);
         randomSequence(max_n));

    auto generate = [&]() {
      randomButtons(N);
      nearLeafSequence(max_n, rnd.nextInt(0, 4), 0.75);
    };

    CASE(doShuffle = true; N = max_n; starGraph(N); generate());
    CASE(doShuffle = true; N = rnd.nextInt(max_n - max_n_diff, max_n);
         starGraph(N); generate());
    CASE(doShuffle = true; N = max_n; broomGraph(N); generate());
    CASE(doShuffle = true; N = max_n; broomGraph(N, N / 4 * 3); generate());
    CASE(doShuffle = true; N = max_n; caterpillarGraph(N); generate());
    CASE(doShuffle = true; N = max_n; pathRandomTree(N); generate());
    CASE(doShuffle = true; N = max_n; pathRandomTree(N, N / 2 + 2); generate());
    CASE(doShuffle = true; N = max_n; pathRandomTree(N, N / 4 * 3); generate());
    CASE(doShuffle = true; N = 2047; binaryTree(N); generate());
    CASE(doShuffle = true; N = max_n; binaryTree(N); generate());
    CASE(doShuffle = true; N = max_n; randomBinaryTree(N); generate());
    CASE(doShuffle = true; N = max_n; longBroomGraph(N); generate());
    CASE(doShuffle = true; N = rnd.nextInt(max_n - max_n_diff, max_n);
         makeTreeDep(100); generate());
    CASE(doShuffle = true; N = rnd.nextInt(max_n - max_n_diff, max_n);
         makeTreeDep(1000); generate());
    CASE(doShuffle = true; N = rnd.nextInt(max_n - max_n_diff, max_n);
         makeTreeLeaf(100); generate());
    CASE(doShuffle = true; N = rnd.nextInt(max_n - max_n_diff, max_n);
         makeTreeLeaf(1000); generate());
    CASE(doShuffle = true; N = rnd.nextInt(max_n - max_n_diff, max_n);
         makeTreeDeg(sqrt(N) - 5, sqrt(N) + 5); generate());
    CASE(doShuffle = true; N = rnd.nextInt(max_n - max_n_diff, max_n);
         makeTreeDeg(N / 10 - 10, N / 5); generate());
    CASE(doShuffle = true; N = max_n; randomTree(N); generate());
    CASE(doShuffle = true; N = max_n; randomTree(N); generate());
  }

  void TestGroup6() {
    // M = N
    Subtasks({2, 5});

    const int max_n = MAXN;
    const int max_l = MAXL;

    const int max_n_diff = 200;

    auto generate = [&]() {
      randomButtons(N);
      return nearLeafSequence(max_l, rnd.nextInt(0, 4),
                              rnd.nextDouble(0.3, 0.7));
    };

    CASE(doShuffle = true; N = max_n; starGraph(N); generate());
    CASE(doShuffle = true; N = max_n; broomGraph(N); generate());
    CASE(doShuffle = true; N = max_n; caterpillarGraph(N); generate());
    CASE(doShuffle = true; N = max_n; pathRandomTree(N); generate());
    CASE(doShuffle = true; N = (1 << 16) - 1; binaryTree(N); generate());
    CASE(doShuffle = true; N = max_n; randomBinaryTree(N); generate());
    CASE(doShuffle = true; N = max_n; longBroomGraph(N); generate());
    CASE(doShuffle = true; N = rnd.nextInt(max_n - max_n_diff, max_n);
         makeTreeDep(N / 20); generate());
    CASE(doShuffle = true; N = rnd.nextInt(max_n - max_n_diff, max_n);
         makeTreeLeaf(N / 20); generate());
    CASE(doShuffle = true; N = rnd.nextInt(max_n - max_n_diff, max_n);
         makeTreeDeg(sqrt(N) - 5, sqrt(N) + 5); generate());
    CASE(doShuffle = true; N = max_n; randomTree(N); generate());
  }

  void TestGroup7() {
    // N, M, L <= MAXN_4000
    Subtasks({3, 5});

    const int max_n = MAXN_4000;
    const int max_n_diff = max_n / 2;

    CASE(doShuffle = true; N = 4; starGraph(N); randomButtons(N * 5);
         randomSequence(max_n));
    CASE(doShuffle = true; N = 7; randomTree(N); randomButtons(N * 5);
         randomSequence(max_n));
    CASE(doShuffle = true; N = 14; randomTree(N); randomButtons(N * 5);
         randomSequence(max_n));
    CASE(doShuffle = true; N = 20; randomTree(N); randomButtons(N * 5);
         randomSequence(max_n));

    for (int i = 0; i < 2; i++) {
      auto generateSequence = [&]() {
        if (i == 0) {
          return nearLeafSequence(max_n, rnd.nextInt(0, 4), 0.75);
        } else if (i == 1) {
          return topKmostFrequentSequence(max_n, rnd.nextInt(2, 20), 0.75);
        } else {
          // never happen
          return randomSequence(max_n);
        }
      };

      CASE(doShuffle = true; N = rnd.nextInt(max_n - max_n_diff, max_n);
           starGraph(N); randomButtons(max_n); generateSequence());
      CASE(doShuffle = true; N = rnd.nextInt(max_n - max_n_diff, max_n);
           broomGraph(N);
           nearLeafButtons(max_n, rnd.nextInt(0, 4), rnd.nextDouble(0.3, 0.7));
           generateSequence());
      CASE(doShuffle = true; N = rnd.nextInt(max_n - max_n_diff, max_n);
           broomGraph(N, N / 4 * 3);
           kBiasedButtons(max_n, rnd.nextInt(1, 20), rnd.nextDouble(0.3, 0.7));
           generateSequence());

      CASE(doShuffle = true; N = rnd.nextInt(max_n - max_n_diff, max_n);
           caterpillarGraph(N);
           nearLeafButtons(max_n, rnd.nextInt(0, 4), rnd.nextDouble(0.3, 0.7));
           generateSequence());
      CASE(doShuffle = true; N = rnd.nextInt(max_n - max_n_diff, max_n);
           pathRandomTree(N);
           nearLeafButtons(max_n, rnd.nextInt(0, 4), rnd.nextDouble(0.3, 0.7));
           generateSequence());
      CASE(doShuffle = true; N = rnd.nextInt(max_n - max_n_diff, max_n);
           pathRandomTree(N, N / 4 * 3);
           kBiasedButtons(max_n, rnd.nextInt(1, 20), rnd.nextDouble(0.3, 0.7));
           generateSequence());

      CASE(doShuffle = true; N = 2047; binaryTree(N);
           nearLeafButtons(max_n, rnd.nextInt(0, 4), rnd.nextDouble(0.3, 0.7));
           generateSequence());
      CASE(doShuffle = true; N = rnd.nextInt(max_n - max_n_diff, max_n);
           binaryTree(N);
           kBiasedButtons(max_n, rnd.nextInt(1, 20), rnd.nextDouble(0.3, 0.7));
           generateSequence());
      CASE(doShuffle = true; N = rnd.nextInt(max_n - max_n_diff, max_n);
           randomBinaryTree(N);
           kBiasedButtons(max_n, rnd.nextInt(1, 20), rnd.nextDouble(0.3, 0.7));
           generateSequence());
      CASE(doShuffle = true; N = rnd.nextInt(max_n - max_n_diff, max_n);
           longBroomGraph(N);
           nearLeafButtons(max_n, rnd.nextInt(0, 4), rnd.nextDouble(0.3, 0.7));
           generateSequence());

      CASE(doShuffle = true; N = rnd.nextInt(max_n - max_n_diff, max_n);
           makeTreeDep(100);
           kBiasedButtons(max_n, rnd.nextInt(1, 20), rnd.nextDouble(0.3, 0.7));
           generateSequence());
      CASE(doShuffle = true; N = rnd.nextInt(max_n - max_n_diff, max_n);
           makeTreeDep(777);
           nearLeafButtons(max_n, rnd.nextInt(0, 4), rnd.nextDouble(0.3, 0.7));
           generateSequence());
      CASE(doShuffle = true; N = rnd.nextInt(max_n - max_n_diff, max_n);
           makeTreeLeaf(100);
           kBiasedButtons(max_n, rnd.nextInt(1, 20), rnd.nextDouble(0.3, 0.7));
           generateSequence());
      CASE(doShuffle = true; N = rnd.nextInt(max_n - max_n_diff, max_n);
           makeTreeLeaf(777);
           nearLeafButtons(max_n, rnd.nextInt(0, 4), rnd.nextDouble(0.3, 0.7));
           generateSequence());

      CASE(doShuffle = true; N = rnd.nextInt(max_n - max_n_diff, max_n);
           makeTreeDeg(sqrt(N) - 5, sqrt(N) + 5);
           kBiasedButtons(max_n, rnd.nextInt(1, 20), rnd.nextDouble(0.3, 0.7));
           generateSequence());
      CASE(doShuffle = true; N = rnd.nextInt(max_n - max_n_diff, max_n);
           randomTree(N);
           kBiasedButtons(max_n, rnd.nextInt(1, 20), rnd.nextDouble(0.3, 0.7));
           generateSequence());
      CASE(doShuffle = true; N = rnd.nextInt(max_n - max_n_diff, max_n);
           randomTree(N);
           nearLeafButtons(max_n, rnd.nextInt(0, 4), rnd.nextDouble(0.3, 0.7));
           generateSequence());
    }
  }

  void TestGroup8() {
    Subtasks({5});

    const int max_n = MAXN;
    const int max_m = MAXM;
    const int max_l = MAXL;
    const int max_n_diff = 200;

    CASE(doShuffle = true; N = rnd.nextInt(max_n - max_n_diff, max_n);
         starGraph(N); randomButtons(max_m); randomSequence(max_l));
    CASE(doShuffle = true; N = rnd.nextInt(max_n - max_n_diff, max_n);
         broomGraph(N);
         nearLeafButtons(max_m, rnd.nextInt(0, 4), rnd.nextDouble(0.3, 0.7));
         topKmostFrequentSequence(max_l, rnd.nextInt(2, 20),
                                  rnd.nextDouble(0.3, 0.7)));
    CASE(doShuffle = true; N = rnd.nextInt(max_n - max_n_diff, max_n);
         caterpillarGraph(N);
         nearLeafButtons(max_m, rnd.nextInt(0, 4), rnd.nextDouble(0.3, 0.7));
         topKmostFrequentSequence(max_l, rnd.nextInt(2, 20),
                                  rnd.nextDouble(0.3, 0.7)));
    CASE(doShuffle = true; N = rnd.nextInt(max_n - max_n_diff, max_n);
         pathRandomTree(N);
         nearLeafButtons(max_m, rnd.nextInt(0, 4), rnd.nextDouble(0.3, 0.7));
         topKmostFrequentSequence(max_l, rnd.nextInt(2, 20),
                                  rnd.nextDouble(0.3, 0.7)));

    CASE(doShuffle = true; N = rnd.nextInt(max_n - max_n_diff, max_n);
         binaryTree(N);
         kBiasedButtons(max_m, rnd.nextInt(1, 20), rnd.nextDouble(0.3, 0.7));
         topKmostFrequentSequence(max_l, rnd.nextInt(2, 20),
                                  rnd.nextDouble(0.3, 0.7)));
    CASE(doShuffle = true; N = rnd.nextInt(max_n - max_n_diff, max_n);
         randomBinaryTree(N);
         kBiasedButtons(max_m, rnd.nextInt(1, 20), rnd.nextDouble(0.3, 0.7));
         topKmostFrequentSequence(max_l, rnd.nextInt(2, 20),
                                  rnd.nextDouble(0.3, 0.7)));
    CASE(doShuffle = true; N = rnd.nextInt(max_n - max_n_diff, max_n);
         longBroomGraph(N);
         nearLeafButtons(max_m, rnd.nextInt(0, 4), rnd.nextDouble(0.3, 0.7));
         topKmostFrequentSequence(max_l, rnd.nextInt(2, 20),
                                  rnd.nextDouble(0.3, 0.7)));

    CASE(doShuffle = true; N = rnd.nextInt(max_n - max_n_diff, max_n);
         makeTreeDep(N / 5);
         nearLeafButtons(max_m, rnd.nextInt(0, 2), rnd.nextDouble(0.3, 0.7));
         topKmostFrequentSequence(max_l, rnd.nextInt(2, 20),
                                  rnd.nextDouble(0.3, 0.7)));
    CASE(doShuffle = true; N = rnd.nextInt(max_n - max_n_diff, max_n);
         makeTreeLeaf(N / 5);
         nearLeafButtons(max_m, rnd.nextInt(0, 4), rnd.nextDouble(0.3, 0.7));
         topKmostFrequentSequence(max_l, rnd.nextInt(2, 20),
                                  rnd.nextDouble(0.3, 0.7)));
    CASE(doShuffle = true; N = rnd.nextInt(max_n - max_n_diff, max_n);
         makeTreeDeg(sqrt(N) - 5, sqrt(N) + 5);
         kBiasedButtons(max_m, rnd.nextInt(1, 20), rnd.nextDouble(0.3, 0.7));
         topKmostFrequentSequence(max_l, rnd.nextInt(2, 20),
                                  rnd.nextDouble(0.3, 0.7)));

    CASE(doShuffle = true; N = rnd.nextInt(max_n - max_n_diff, max_n);
         randomTree(N);
         kBiasedButtons(max_m, rnd.nextInt(1, 20), rnd.nextDouble(0.3, 0.7));
         topKmostFrequentSequence(max_l, rnd.nextInt(2, 20),
                                  rnd.nextDouble(0.3, 0.7)));
    CASE(doShuffle = true; N = rnd.nextInt(max_n - max_n_diff, max_n);
         randomTree(N);
         nearLeafButtons(max_m, rnd.nextInt(0, 4), rnd.nextDouble(0.3, 0.7));
         topKmostFrequentSequence(max_l, rnd.nextInt(2, 20),
                                  rnd.nextDouble(0.3, 0.7)));
  }

 private:
  void randomSequence(int length) {
    L = length;
    S.resize(L);
    for (int i = 0; i < L; i++) {
      S[i] = rnd.nextInt(0, N - 1);
      while (i > 0 && S[i] == S[i - 1]) {
        S[i] = rnd.nextInt(0, N - 1);
      }
    }
  }

  void topKmostFrequentSequence(int length, int K, double prob_biased) {
    assert(K >= 2);

    L = length;
    S.resize(L);

    vector<int> freq(N, 0);
    for (auto a : A) freq[a]++;

    vector<int> topK(N);
    iota(begin(topK), end(topK), 0);
    sort(begin(topK), end(topK),
         [&](int i, int j) { return freq[i] > freq[j]; });

    topK.resize(min(K, N));

    for (int i = 0; i < L; i++) {
      if (rnd.nextDouble(1) < prob_biased) {
        S[i] = topK[rnd.nextInt(0, int(topK.size()) - 1)];
      } else {
        S[i] = rnd.nextInt(0, N - 1);
      }
      while (i > 0 && S[i] == S[i - 1]) {
        if (rnd.nextDouble(1) < prob_biased) {
          S[i] = topK[rnd.nextInt(0, int(topK.size()) - 1)];
        } else {
          S[i] = rnd.nextInt(0, N - 1);
        }
      }
    }
  }

  void importanceSampledSequence(int length) {
    L = length;
    S.resize(L);

    vector<int> freq(N, 0);
    for (auto a : A) freq[a]++;

    int total_freq = accumulate(begin(freq), end(freq), 0);
    for (int i = 1; i < N; i++) {
      freq[i] += freq[i - 1];
    }

    auto Sample = [&]() {
      int x = rnd.nextInt(0, total_freq - 1);
      return int(upper_bound(begin(freq), end(freq), x) - begin(freq));
    };

    for (int i = 0; i < L; i++) {
      S[i] = Sample();
      while (i > 0 && S[i] == S[i - 1]) {
        S[i] = Sample();
      }
    }
  }

  void nearLeafSequence(int length, int distance_threshold,
                        double prob_near_leaf) {
    vector<int> que;
    vector<int> distance(N, -1);
    vector<vector<int>> adj(N);
    for (int i = 0; i < N - 1; i++) {
      adj[P[i]].push_back(i + 1);
      adj[i + 1].push_back(P[i]);
    }
    for (int i = 0; i < N; i++) {
      if (int(adj[i].size()) == 1) {
        que.push_back(i);
        distance[i] = 0;
      }
    }
    for (int q = 0; q < int(que.size()); q++) {
      int u = que[q];
      for (auto v : adj[u]) {
        if (distance[v] == -1) {
          distance[v] = distance[u] + 1;
          que.push_back(v);
        }
      }
    }

    vector<int> near_leaf;
    for (int i = 0; i < N; i++) {
      if (distance[i] <= distance_threshold) {
        near_leaf.push_back(i);
      }
    }
    assert(!near_leaf.empty());

    L = length;
    S.resize(L);
    for (int i = 0; i < L; i++) {
      if (rnd.nextDouble(1) < prob_near_leaf) {
        S[i] = near_leaf[rnd.nextInt(0, int(near_leaf.size()) - 1)];
      } else {
        S[i] = rnd.nextInt(0, N - 1);
      }
      while (i > 0 && S[i] == S[i - 1]) {
        if (rnd.nextDouble(1) < prob_near_leaf) {
          S[i] = near_leaf[rnd.nextInt(0, int(near_leaf.size()) - 1)];
        } else {
          S[i] = rnd.nextInt(0, N - 1);
        }
      }
    }
  }

  void randomButtons(int length) {
    M = length;
    assert(M >= N);
    vector<int> buttons(M);
    iota(begin(buttons), end(buttons), 0);
    for (int i = N; i < M; i++) {
      buttons[i] = rnd.nextInt(0, N - 1);
    }
    rnd.shuffle(begin(buttons) + (doShuffle ? 0 : 1), end(buttons));
    A = buttons;
  }

  void nearLeafButtons(int length, int distance_threshold,
                       double prob_near_leaf) {
    vector<int> que;
    vector<int> distance(N, -1);
    vector<vector<int>> adj(N);
    for (int i = 0; i < N - 1; i++) {
      adj[P[i]].push_back(i + 1);
      adj[i + 1].push_back(P[i]);
    }
    for (int i = 0; i < N; i++) {
      if (int(adj[i].size()) == 1) {
        que.push_back(i);
        distance[i] = 0;
      }
    }
    for (int q = 0; q < int(que.size()); q++) {
      int u = que[q];
      for (auto v : adj[u]) {
        if (distance[v] == -1) {
          distance[v] = distance[u] + 1;
          que.push_back(v);
        }
      }
    }

    vector<int> near_leaf;
    for (int i = 0; i < N; i++) {
      if (distance[i] <= distance_threshold) {
        near_leaf.push_back(i);
      }
    }
    assert(!near_leaf.empty());

    M = length;
    assert(M >= N);
    vector<int> buttons(M);
    iota(begin(buttons), end(buttons), 0);
    for (int i = N; i < M; i++) {
      if (rnd.nextDouble(1) < prob_near_leaf) {
        buttons[i] = near_leaf[rnd.nextInt(0, int(near_leaf.size()) - 1)];
      } else {
        buttons[i] = rnd.nextInt(0, N - 1);
      }
    }
    rnd.shuffle(begin(buttons) + (doShuffle ? 0 : 1), end(buttons));
    A = buttons;
  }

  void kBiasedButtons(int length, int K, double prob_biased) {
    M = length;
    assert(M >= N);
    vector<int> buttons(M);
    iota(begin(buttons), end(buttons), 0);

    set<int> k_biased_set;
    while (int(k_biased_set.size()) < K) {
      k_biased_set.insert(rnd.nextInt(0, N - 1));
    }
    vector<int> k_biased(begin(k_biased_set), end(k_biased_set));

    for (int i = N; i < M; i++) {
      if (rnd.nextDouble(1) < prob_biased) {
        buttons[i] = k_biased[rnd.nextInt(0, K - 1)];
      } else {
        buttons[i] = rnd.nextInt(0, N - 1);
      }
    }
    rnd.shuffle(begin(buttons) + (doShuffle ? 0 : 1), end(buttons));
    A = buttons;
  }

  void pathGraph(int N) {
    P.assign(N - 1, 0);
    iota(P.begin(), P.end(), 0);
  }

  void starGraph(int N) { P.assign(N - 1, 0); }

  void broomGraph(int N, int height = -1) {
    if (height == -1) height = N / 2;
    for (int i = 1; i < height; ++i) {
      P.push_back(i - 1);
    }
    for (int i = height; i < N; ++i) {
      P.push_back(height - 1);
    }
  }

  void caterpillarGraph(int N) {
    int body = N - N / 4;
    for (int i = 1; i < body; ++i) {
      P.push_back(i - 1);
    }

    // feet
    for (int i = body; i < N; ++i) {
      P.push_back(rnd.nextInt(0, P.size()));
    }
  }

  void pathRandomTree(int N, int pathLength = -1) {
    if (pathLength == -1) pathLength = N / 2;
    for (int i = 1; i < pathLength; ++i) {
      P.push_back(i - 1);
    }
    for (int i = pathLength; i < N; ++i) {
      P.push_back(rnd.nextInt(pathLength - 1, i - 1));
    }
  }

  void binaryTree(int N) {
    for (int i = 1; i < N; ++i) {
      P.push_back(((i + 1) >> 1) - 1);
    }
  }

  void randomBinaryTree(int N) {
    vector<int> degree(N + 1, 0);
    P.push_back(0);
    P.push_back(0);
    degree[0] = 2;

    for (int i = 3; i < N; ++i) {
      int p;
      do {
        p = rnd.nextInt(0, i - 1);
      } while (degree[p] == 2);

      P.push_back(p);
      ++degree[p];
    }
  }

  void longBroomGraph(int N, int deg = 10) {
    assert(N >= deg * 2);
    for (int i = 1; i < deg; ++i) {
      P.push_back(i - 1);
    }

    for (int i = 0; i < deg; ++i) {
      P.push_back(deg - 1);
    }

    for (int i = deg * 2; i < N; ++i) {
      P.push_back(i - deg);
    }
  }

  void makeTreeLeaf(int leafCount) {
    auto [U, V] = TreeRandomizer()
                      .node_count(N)
                      .index_base(0)
                      .root(rnd.nextInt(0, N - 1))
                      .leaf_count(leafCount)
                      .next()
                      .edges();
    P = transformTree(U, V);
    changeRoot();
  }

  void makeTreeDep(int depth) {
    auto [U, V] = TreeRandomizer()
                      .node_count(N)
                      .index_base(0)
                      .root(rnd.nextInt(0, N - 1))
                      .depth(depth)
                      .next()
                      .edges();
    P = transformTree(U, V);
    changeRoot();
  }

  void randomTree(int N) {
    auto [U, V] = TreeRandomizer().node_count(N).index_base(0).next().edges();
    P = transformTree(U, V);
    changeRoot();
  }

  void makeTreeDeg(int loDeg, int hiDeg) {
    tie(U, V) = TreeRandomizer()
                    .node_count(N)
                    .index_base(0)
                    .root(rnd.nextInt(0, N - 1))
                    .child_count(loDeg, hiDeg)
                    .next()
                    .edges();
    P = transformTree(U, V);
    changeRoot();
  }

  vector<int> transformTree(vector<int> U, vector<int> V) {
    vector<vector<int>> adj(N);
    for (int i = 0; i < N - 1; i++) {
      adj[U[i]].push_back(V[i]);
      adj[V[i]].push_back(U[i]);
    }
    vector<int> ord;
    vector<int> que;
    vector<int> vis(N);
    int root = rnd.nextInt(N);
    vis[root] = 1;
    que.push_back(root);
    while (!que.empty()) {
      int r = rnd.nextInt(0, int(que.size()) - 1);
      int u = que[r];
      ord.push_back(u);
      swap(que[r], que.back());
      que.pop_back();
      for (auto v : adj[u]) {
        if (!vis[v]) {
          vis[v] = 1;
          que.push_back(v);
        }
      }
    }
    vector<int> pos(N);
    for (int i = 0; i < N; i++) {
      pos[ord[i]] = i;
    }
    vector<int> p(N - 1);
    for (int i = 0; i < N - 1; i++) {
      p[max(pos[U[i]], pos[V[i]]) - 1] = min(pos[U[i]], pos[V[i]]);
    }
    return p;
  }

  void changeRoot() {
    vector<int> U, V;
    for (int i = 0; i < N - 1; i++) {
      U.push_back(P[i]);
      V.push_back(i + 1);
    }
    P = transformTree(U, V);
  }
};
