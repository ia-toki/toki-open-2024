#include <tcframe/spec.hpp>
#include <bits/stdc++.h>

using namespace tcframe;
using namespace std;

const int MAXN = 500;

class ProblemSpec : public BaseProblemSpec {
 protected:
  int subtask;
  int N, R;
  vector<int> U, V;

  string queryString;
  string outSecret;
  int guessedRoot;
  int Q;

  void InputFormat() {
    LINE(subtask);
    LINE(N, R);
    LINES(U, V) % SIZE(N - 1);
  }

  void OutputFormat() {
    LINE(queryString);
    LINE(outSecret);
    LINE(guessedRoot);
    LINE(Q);
  }

  void GradingConfig() {
    TimeLimit(1);
    MemoryLimit(256);
  }

  void StyleConfig() {
    CustomScorer();
  }

  void Constraints() {
    CONS(4 <= N && N <= 500);
    CONS(0 <= R && R < N);
    CONS(isTree(U, V));
    CONS(leavesCount(U, V) >= 3);
  }

  void Subtask1() {
    Points(12);
    CONS(subtask == 1);
    for (int i = 0; i < N - 1; ++i) {
      CONS(U[i] == 0);
      CONS(V[i] == i + 1);
    }
  }

  void Subtask2() {
    Points(18);
    CONS(subtask == 2);
    for (int i = 0; i < N - 2; ++i) {
      CONS(U[i] == i);
      CONS(V[i] == i + 1);
    }
    CONS(U[N - 2] == N - 3);
    CONS(V[N - 2] == N - 1);
  }

  void Subtask3() {
    Points(70);
    CONS(subtask == 3);
  }

  bool isTree(const vector<int> &U, const vector<int> &V) {
    int N = U.size() + 1;
    for (int i = 0; i < N - 1; ++i) {
      if (U[i] >= V[i]) return false;
      if (U[i] < 0) return false;
      if (V[i] >= N) return false;
    }

    vector<int> par(N);
    iota(par.begin(), par.end(), 0);
    function<int(int)> root = [&](int u) {
      if (par[u] == u) return u;
      return par[u] = root(par[u]);
    };
    for (int i = 0; i < N - 1; ++i) {
      if (root(U[i]) == root(V[i])) return false;
      par[par[U[i]]] = par[V[i]];
    }
    return true;
  }

  int leavesCount(const vector<int> &U, const vector<int> &V) {
    vector<int> deg(N);
    for (int i = 0; i < U.size(); ++i) {
      ++deg[U[i]];
      ++deg[V[i]];
    }
    return count(deg.begin(), deg.end(), 1);
  }

};

class TestSpec : public BaseTestSpec<ProblemSpec> {
 protected:

  void SampleTestCase1() {
    Subtasks({3});
    Input({
      "3",
      "7 3",
      "0 1",
      "1 5",
      "2 6",
      "2 3",
      "0 3",
      "3 4",
    });
    Output({
      "QQQQQ",
      "toki-open-2024-zebra_secret-output",
      "3",
      "5",
    });
  }

  void BeforeTestCase() {
    subtask = 3;
    U.clear(); V.clear();
  }

  void AfterTestCase() {
    if (subtask == 3) {
      vector<int> order(N);
      iota(order.begin(), order.end(), 0);
      rnd.shuffle(order.begin() + 1, order.end());
      R = order[R];
      for (int i = 0; i < N - 1; ++i) {
        U[i] = order[U[i]];
        V[i] = order[V[i]];
        if (U[i] > V[i]) {
          swap(U[i], V[i]);
        }
      }
    }
  }

  void TestGroup1() {
    Subtasks({1});

    starCases(1);
  }

  void TestGroup2() {
    Subtasks({2});

    almostLineCases(2);
  }

  void TestGroup3() {
    Subtasks({3});

    starCases();
    almostLineCases();

    CASE(N = MAXN; R = rnd.nextInt(N); randomTree(N));

    for (int r : {0, 1, MAXN - 2, MAXN - 1}) {
      CASE(N = MAXN; R = r;
           for (int i = 0; i < N - 2; ++i) addEdge(0, i + 1);
           addEdge(N - 2, N - 1));
      CASE(N = MAXN; R = r;
           addEdge(0, 1);
           for (int i = 0; i < N - 2; ++i) addEdge(1, i + 2));
      CASE(N = MAXN; R = r;
           for (int i = 0; i < (N - 2) / 2; ++i) addEdge(0, i + 1);
           for (int i = 0; i < (N - 2) / 2; ++i) addEdge(N - 1, N - 2 - i);
           addEdge(1, N - 1));
      CASE(N = MAXN; R = r;
           for (int i = 0; i < (N - 2) / 2; ++i) addEdge(0, i + 1);
           for (int i = 0; i < (N - 2) / 2; ++i) addEdge(N - 1, N - 2 - i);
           addEdge(0, N - 1));
      CASE(N = MAXN; R = r;
           for (int i = 0; i < (N - 2) / 2; ++i) addEdge(0, i + 2);
           for (int i = 0; i < (N - 2) / 2; ++i) addEdge(1, N - 1 - i);
           addEdge(0, 1));
      CASE(N = MAXN; R = r;
           for (int i = 0; i < (N - 1) / 2; ++i) addEdge(0, i + 1);
           for (int i = (N - 1) / 2; i < N - 1; ++i) addEdge(i, i + 1));
      CASE(N = MAXN; R = r;
           for (int i = 0; i < (N - 2) / 2; ++i) addEdge(0, i + 1);
           for (int i = 0; i < (N - 2) / 2; ++i) addEdge(i + 1, (N - 2) / 2 + 1 + i);
           addEdge(0, N - 1));
      CASE(N = MAXN; R = r;
           for (int i = 0; i < N - 1; ++i) addEdge(i / 2, i + 1));
    }

    for (int r : {0, 1, 497, 498/3*2, 498/3*2 - 1}) {
      CASE(N = 498; R = r;
           for (int i = 1; i < N / 3 * 2; ++i) addEdge(i - 1, i);
           for (int i = 0; i < N / 3; ++i) addEdge(i * 2, N / 3 * 2 + i));
    }

    // Three legs
    CASE(threeLegs(1, 2, MAXN - 4); R = 0);
    CASE(threeLegs(1, 2, MAXN - 4); R = 1);
    CASE(threeLegs(1, 2, MAXN - 4); R = 2);
    CASE(threeLegs(1, 2, MAXN - 4); R = N - 2);
    CASE(threeLegs(1, 2, MAXN - 4); R = N - 1);
    CASE(threeLegs(MAXN / 3, MAXN / 3, MAXN / 3); R = 0);
    CASE(threeLegs(MAXN / 3, MAXN / 3, MAXN / 3); R = 1);
    CASE(threeLegs(MAXN / 3, MAXN / 3, MAXN / 3); R = N - 1);
    CASE(threeLegs(100, 100, 299); R = 0);
    CASE(threeLegs(100, 100, 299); R = N - 1);
  }

  void addEdge(int u, int v) {
    U.push_back(u);
    V.push_back(v);
  }

  void starGraph(int N) {
    U = vector<int>(N - 1, 0);
    V = vector<int>(N - 1);
    iota(V.begin(), V.end(), 1);
  }

  void almostLineGraph(int N) {
    U.resize(N - 1);
    V.resize(N - 1);
    for (int i = 0; i < N - 2; ++i) {
      U[i] = i;
      V[i] = i + 1;
    }
    U[N - 2] = N - 3;
    V[N - 2] = N - 1;
  }

  void randomTree(int N) {
    U.resize(N - 1);
    V.resize(N - 1);
    for (int i = 0; i < N - 1; ++i) {
      U[i] = rnd.nextInt(i + 1);
      V[i] = i + 1;
    }
  }

  void threeLegs(int a, int b, int c) {
    addEdge(0, 1);
    for (int i = 1; i < a; ++i) addEdge(i, i + 1);
    addEdge(0, a + 1);
    for (int i = 1; i < b; ++i) addEdge(a + i, a + i + 1);
    addEdge(0, a + b + 1);
    for (int i = 1; i < c; ++i) addEdge(a + b + i, a + b + i + 1);
    N = a + b + c + 1;
  }

  void starCases(int sub = 3) {
    CASE(subtask = sub; N = 4; R = 0; starGraph(N));
    CASE(subtask = sub; N = 4; R = 1; starGraph(N));
    CASE(subtask = sub; N = 4; R = 2; starGraph(N));
    CASE(subtask = sub; N = 4; R = 3; starGraph(N));

    CASE(subtask = sub; N = MAXN - 1; R = 0; starGraph(N));
    CASE(subtask = sub; N = MAXN - 1; R = N - 1; starGraph(N));
    CASE(subtask = sub; N = MAXN - 1; R = N / 2; starGraph(N));
    CASE(subtask = sub; N = MAXN - 1; R = N / 2 + 1; starGraph(N));

    CASE(subtask = sub; N = MAXN; R = 0; starGraph(N));
    CASE(subtask = sub; N = MAXN; R = N - 1; starGraph(N));
    CASE(subtask = sub; N = MAXN; R = N / 2; starGraph(N));
    CASE(subtask = sub; N = MAXN; R = N / 2 + 1; starGraph(N));

    CASE(subtask = sub; N = rnd.nextInt(MAXN / 10 * 9, MAXN); R = rnd.nextInt(N); starGraph(N));
    CASE(subtask = sub; N = rnd.nextInt(MAXN / 10 * 9, MAXN); R = rnd.nextInt(N); starGraph(N));
    CASE(subtask = sub; N = rnd.nextInt(MAXN / 10 * 9, MAXN); R = rnd.nextInt(N); starGraph(N));
    CASE(subtask = sub; N = rnd.nextInt(MAXN / 10 * 9, MAXN); R = rnd.nextInt(N); starGraph(N));
  }

  void almostLineCases(int sub = 3) {
    CASE(subtask = sub; N = 4; R = 0; almostLineGraph(N));
    CASE(subtask = sub; N = 4; R = 1; almostLineGraph(N));
    CASE(subtask = sub; N = 4; R = 2; almostLineGraph(N));
    CASE(subtask = sub; N = 4; R = 3; almostLineGraph(N));

    CASE(subtask = sub; N = 5; R = 0; almostLineGraph(N));
    CASE(subtask = sub; N = 5; R = 1; almostLineGraph(N));
    CASE(subtask = sub; N = 5; R = 2; almostLineGraph(N));
    CASE(subtask = sub; N = 5; R = 3; almostLineGraph(N));
    CASE(subtask = sub; N = 5; R = 4; almostLineGraph(N));

    CASE(subtask = sub; N = MAXN; R = 0; almostLineGraph(N));
    CASE(subtask = sub; N = MAXN; R = N - 1; almostLineGraph(N));
    CASE(subtask = sub; N = MAXN; R = N - 2; almostLineGraph(N));
    CASE(subtask = sub; N = MAXN; R = N - 3; almostLineGraph(N));
    CASE(subtask = sub; N = MAXN; R = N / 2; almostLineGraph(N));
    CASE(subtask = sub; N = MAXN; R = N / 2 + 1; almostLineGraph(N));

    CASE(subtask = sub; N = MAXN - 1; R = 0; almostLineGraph(N));
    CASE(subtask = sub; N = MAXN - 1; R = N - 1; almostLineGraph(N));
    CASE(subtask = sub; N = MAXN - 1; R = N - 2; almostLineGraph(N));
    CASE(subtask = sub; N = MAXN - 1; R = N - 3; almostLineGraph(N));
    CASE(subtask = sub; N = MAXN - 1; R = N / 2; almostLineGraph(N));
    CASE(subtask = sub; N = MAXN - 1; R = N / 2 + 1; almostLineGraph(N));

    CASE(subtask = sub; N = rnd.nextInt(MAXN / 10 * 9, MAXN); R = rnd.nextInt(N); almostLineGraph(N));
    CASE(subtask = sub; N = rnd.nextInt(MAXN / 10 * 9, MAXN); R = rnd.nextInt(N); almostLineGraph(N));
    CASE(subtask = sub; N = rnd.nextInt(MAXN / 10 * 9, MAXN); R = rnd.nextInt(N); almostLineGraph(N));
  }
};
