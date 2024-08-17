#include <bits/extc++.h>
#include <bits/stdc++.h>

#include <tcframe/spec.hpp>

using namespace std;
using namespace tcframe;
using namespace __gnu_pbds;

template <typename T>
using ordered_set =
    tree<T, null_type, less<T>, rb_tree_tag, tree_order_statistics_node_update>;

using lint = long long;

const lint MAXN = 2e5;
const lint MAXA = 1e9;
const lint MAXN_2000 = 2000;
const lint MAXA_50 = 50;

using data_t = lint;

template <bool RIGHTMOST>
struct SegTree {
  int n;
  vector<pair<data_t, int>> tree;
  vector<data_t> lazy;

  SegTree(int n) : n(n), tree(2 * n), lazy(2 * n) {}

  void Apply(int n, data_t x) {
    tree[n].first += x;
    lazy[n] += x;
  }

  void Push(int n, int lc, int rc) {
    Apply(lc, lazy[n]);
    Apply(rc, lazy[n]);
    lazy[n] = 0;
  }

  void Pull(int n, int lc, int rc) { tree[n] = min(tree[lc], tree[rc]); }

  template <typename T>
  void Build(int n, int tl, int tr, const vector<T>& a) {
    if (tl == tr) {
      if constexpr (RIGHTMOST) {
        tree[n] = {static_cast<data_t>(a[tl]), -tl};
      } else {
        tree[n] = {static_cast<data_t>(a[tl]), tl};
      }
      return;
    }
    int tm = (tl + tr) / 2;
    int z = n + 2 * (tm - tl + 1);
    Build(n + 1, tl, tm, a);
    Build(z, tm + 1, tr, a);
    Pull(n, n + 1, z);
  }

  template <typename T>
  void Build(const vector<T>& a) {
    Build(1, 0, n - 1, a);
  }

  void RangeAdd(int n, int tl, int tr, int l, int r, data_t x) {
    if (r < tl || tr < l || r < l || tr < tl) return;
    if (l <= tl && tr <= r) {
      Apply(n, x);
      return;
    }
    int tm = (tl + tr) / 2;
    int z = n + 2 * (tm - tl + 1);
    Push(n, n + 1, z);
    RangeAdd(n + 1, tl, tm, l, r, x);
    RangeAdd(z, tm + 1, tr, l, r, x);
    Pull(n, n + 1, z);
  }

  void RangeAdd(int l, int r, data_t x) { RangeAdd(1, 0, n - 1, l, r, x); }

  pair<data_t, int> Query(int n, int tl, int tr, int l, int r) {
    if (r < tl || tr < l || r < l || tr < tl)
      return {numeric_limits<data_t>::max(), -1};
    if (l <= tl && tr <= r) return tree[n];
    int tm = (tl + tr) / 2;
    int z = n + 2 * (tm - tl + 1);
    Push(n, n + 1, z);
    return min(Query(n + 1, tl, tm, l, r), Query(z, tm + 1, tr, l, r));
  }

  pair<data_t, int> Query(int l, int r) {
    auto res = Query(1, 0, n - 1, l, r);
    if constexpr (RIGHTMOST) {
      res.second = -res.second;
    }
    return res;
  }
};

class ProblemSpec : public BaseProblemSpec {
 protected:
  lint N;
  vector<lint> A;

  long long ans;

  void InputFormat() {
    LINE(N);
    LINE(A % SIZE(N));
  }

  void OutputFormat() { LINE(ans); }

  void GradingConfig() {
    TimeLimit(1);
    MemoryLimit(256);
  }

  void Constraints() {
    CONS(1 <= N && N <= MAXN);
    CONS(N == lint(A.size()));
    CONS(eachElementBetween(A, 0ll, MAXA));
  }

  void Subtask1() {
    Points(4);
    CONS(N <= MAXN_2000);
    CONS(A[0] == 0);
    CONS(A[N - 1] == 0);
  }

  void Subtask2() {
    Points(15);
    CONS(N <= MAXN_2000);
    CONS(eachElementBetween(A, 0ll, MAXA_50));
  }

  void Subtask3() {
    Points(10);
    CONS(N <= MAXN_2000);
    CONS(A[N - 1] == 0);
  }

  void Subtask4() {
    Points(16);
    CONS(N <= MAXN_2000);
    CONS(isBitonic(A));
  }

  void Subtask5() {
    Points(24);
    CONS(N <= MAXN_2000);
    CONS(A[0] == A[N - 1]);
    CONS(eachElementBetween(A, A[0], MAXA));
  }

  void Subtask6() {
    Points(17);
    CONS(N <= MAXN_2000);
  }

  void Subtask7() { Points(14); }

 private:
  template <typename T>
  bool eachElementBetween(const vector<T>& A, const T& minA, const T& maxA) {
    for (const T& a : A) {
      if (!(minA <= a && a <= maxA)) {
        return false;
      }
    }
    return true;
  }

  template <typename T>
  bool isBitonic(const vector<T>& A) {
    lint k = 1;
    while (k < lint(A.size()) && A[k - 1] <= A[k]) {
      k += 1;
    }
    for (lint i = k + 1; i < lint(A.size()); i++) {
      if (A[i - 1] < A[i]) {
        return false;
      }
    }
    return true;
  }
};

class TestSpec : public BaseTestSpec<ProblemSpec> {
 protected:
  void SampleTestCase1() {
    Subtasks({2, 6, 7});
    Input({
        "5",
        "2 1 3 4 1",
    });
    Output({
        "3",
    });
  }

  void SampleTestCase2() {
    Subtasks({2, 4, 5, 6, 7});
    Input({
        "6",
        "1 1 2 2 1 1",
    });
    Output({
        "0",
    });
  }

  void BeforeTestCase() {
    N = ans = 0;
    A.clear();
  }

  void TestGroup1() {
    Subtasks({1, 2, 3, 4, 5, 6, 7});
    CASE(N = 1; A = {0});
    CASE(N = 2; A = {0, 0});
    CASE(N = MAXN_2000; A = randomArray(N, MAXA_50, MAXA_50);
         A[0] = A[N - 1] = 0);
    CASE(N = MAXN_2000; A = randomBitonicArray(N, 0, MAXA_50);
         A[0] = A[N - 1] = 0);
  }

  void TestGroup2() {
    Subtasks({1, 2, 3, 5, 6, 7});
    CASE(N = rnd.nextLongLong(MAXN_2000 - 100, MAXN_2000);
         A = randomArray(N, 0, MAXA_50); A[0] = A[N - 1] = 0);
    CASE(N = rnd.nextLongLong(MAXN_2000 - 10, MAXN_2000);
         A = randomArray(N, 0, MAXA_50); A[0] = A[N - 1] = 0);
    CASE(N = MAXN_2000; A = randomArray(N, 0, MAXA_50); A[0] = A[N - 1] = 0);
    CASE(N = MAXN_2000; A = randomArray(N, 0, MAXA_50); A[0] = A[N - 1] = 0);
  }

  void TestGroup3() {
    Subtasks({1, 3, 4, 5, 6, 7});
    CASE(N = MAXN_2000; A = randomArray(N, MAXA, MAXA); A[0] = A[N - 1] = 0);
    CASE(N = MAXN_2000; A = randomBitonicArray(N, 0, MAXA);
         A[0] = A[N - 1] = 0);
    CASE(N = MAXN_2000; A = randomBitonicArray(N, 0, MAXA);
         A[0] = A[N - 1] = 0);
  }

  void TestGroup4() {
    Subtasks({1, 3, 5, 6, 7});
    for (lint i = 0; i < 2; i++) {
      auto genA = [&]() {
        if (i == 0) {
          return randomArray(N, 0, MAXA);
        } else {
          return randomArray(N, MAXN_2000 / 10, 0, MAXA);
        }
      };
      CASE(N = rnd.nextLongLong(MAXN_2000 - 100, MAXN_2000); A = genA();
           A[0] = A[N - 1] = 0);
      CASE(N = rnd.nextLongLong(MAXN_2000 - 10, MAXN_2000); A = genA();
           A[0] = A[N - 1] = 0);
      CASE(N = MAXN_2000; A = genA(); A[0] = A[N - 1] = 0);
    }
  }

  void TestGroup5() {
    Subtasks({2, 3, 4, 6, 7});
    CASE(N = 2; A = {1, 0});
    CASE(N = 2; A = {MAXA_50, 0});
    CASE(N = MAXN_2000; A = randomBitonicArray(N, 1, MAXA_50); A[N - 1] = 0);
    CASE(N = MAXN_2000; A = randomBitonicArray(N, 1, MAXA_50); A[N - 1] = 0);
  }

  void TestGroup6() {
    Subtasks({2, 3, 6, 7});
    CASE(N = rnd.nextLongLong(MAXN_2000 - 100, MAXN_2000);
         A = randomArray(N, 0, MAXA_50); A[N - 1] = 0; while (A[0] == 0) {
           A = randomArray(N, 0, MAXA_50);
           A[N - 1] = 0;
         });
    CASE(N = rnd.nextLongLong(MAXN_2000 - 10, MAXN_2000);
         A = randomArray(N, 0, MAXA_50); A[N - 1] = 0; while (A[0] == 0) {
           A = randomArray(N, 0, MAXA_50);
           A[N - 1] = 0;
         });
    CASE(N = MAXN_2000; A = randomArray(N, 0, MAXA_50); A[N - 1] = 0;
         while (A[0] == 0) {
           A = randomArray(N, 0, MAXA_50);
           A[N - 1] = 0;
         });
  }

  void TestGroup7() {
    Subtasks({3, 6, 7});
    for (lint i = 0; i < 2; i++) {
      auto genA = [&]() {
        auto gen = [&]() {
          vector<lint> ret;
          if (i == 0) {
            ret = randomArray(N, 0, MAXA);
          } else {
            ret = randomArray(N, MAXN_2000 / 10, 0, MAXA);
          }
          ret[N - 1] = 0;
          return ret;
        };
        return repeatUntil(gen, [](auto a) { return a[0] != 0; });
      };
      CASE(N = rnd.nextLongLong(MAXN_2000 - 100, MAXN_2000); A = genA());
      CASE(N = rnd.nextLongLong(MAXN_2000 - 10, MAXN_2000); A = genA());
      CASE(N = MAXN_2000; A = genA());
    }
  }

  void TestGroup8() {
    Subtasks({2, 4, 5, 6, 7});

    const lint maximum = MAXA_50;
    const auto ensure = [&]() { A[0] = A[N - 1] = min(A[0], A[N - 1]); };

    CASE(lint minimum = rnd.nextLongLong(10, maximum * 2 / 5); N = MAXN_2000;
         A = randomBitonicArray(N, minimum, maximum); ensure());
    CASE(lint minimum = rnd.nextLongLong(10, maximum * 2 / 5); N = MAXN_2000;
         A = randomBitonicArray(N, N, minimum, maximum); ensure());
    CASE(lint minimum = rnd.nextLongLong(10, maximum * 2 / 5); N = MAXN_2000;
         A = randomBitonicArray(N, N / 2, minimum, maximum); ensure());
    CASE(lint minimum = rnd.nextLongLong(10, maximum * 2 / 5); N = MAXN_2000;
         A = randomBitonicArray(N, floor(sqrt(N)), minimum, maximum); ensure());

    CASE(lint minimum = rnd.nextLongLong(10, maximum * 2 / 5);
         N = rnd.nextLongLong(MAXN_2000 - 10, MAXN_2000);
         A = randomBitonicArray(N, minimum, maximum); ensure());
    CASE(lint minimum = rnd.nextLongLong(10, maximum * 2 / 5);
         N = rnd.nextLongLong(MAXN_2000 - 10, MAXN_2000);
         A = randomBitonicArray(N, N, minimum, maximum); ensure());
    CASE(lint minimum = rnd.nextLongLong(10, maximum * 2 / 5);
         N = rnd.nextLongLong(MAXN_2000 - 10, MAXN_2000);
         A = randomBitonicArray(N, N / 2, minimum, maximum); ensure());
    CASE(lint minimum = rnd.nextLongLong(10, maximum * 2 / 5);
         N = rnd.nextLongLong(MAXN_2000 - 10, MAXN_2000);
         A = randomBitonicArray(N, floor(sqrt(N)), minimum, maximum); ensure());

    CASE(lint minimum = rnd.nextLongLong(10, maximum * 2 / 5);
         N = rnd.nextLongLong(MAXN_2000 - 10, MAXN_2000);
         A = groupedBitonicArray(N, floor(sqrt(N)), minimum, maximum);
         ensure());
    CASE(lint minimum = rnd.nextLongLong(10, maximum * 2 / 5);
         N = rnd.nextLongLong(MAXN_2000 - 10, MAXN_2000);
         A = groupedBitonicArray(N, floor(sqrt(N)), N, minimum, maximum);
         ensure());
    CASE(lint minimum = rnd.nextLongLong(10, maximum * 2 / 5);
         N = rnd.nextLongLong(MAXN_2000 - 10, MAXN_2000);
         A = groupedBitonicArray(N, floor(sqrt(N)), floor(sqrt(N)), minimum,
                                 maximum);
         ensure());

    CASE(lint minimum = rnd.nextLongLong(10, maximum * 2 / 5);
         N = rnd.nextLongLong(MAXN_2000 - 10, MAXN_2000);
         A = groupedBitonicArray(N, 10, minimum, maximum); ensure());
    CASE(lint minimum = rnd.nextLongLong(10, maximum * 2 / 5);
         N = rnd.nextLongLong(MAXN_2000 - 10, MAXN_2000);
         A = groupedBitonicArray(N, 10, N, minimum, maximum); ensure());
    CASE(lint minimum = rnd.nextLongLong(10, maximum * 2 / 5);
         N = rnd.nextLongLong(MAXN_2000 - 10, MAXN_2000);
         A = groupedBitonicArray(N, 10, floor(sqrt(N)), minimum, maximum);
         ensure());

    CASE(lint minimum = rnd.nextLongLong(10, maximum * 2 / 5);
         N = rnd.nextLongLong(MAXN_2000 - 10, MAXN_2000);
         A = groupedBitonicArray(N, N / 10, minimum, maximum); ensure());
    CASE(lint minimum = rnd.nextLongLong(10, maximum * 2 / 5);
         N = rnd.nextLongLong(MAXN_2000 - 10, MAXN_2000);
         A = groupedBitonicArray(N, N / 10, N, minimum, maximum); ensure());
    CASE(lint minimum = rnd.nextLongLong(10, maximum * 2 / 5);
         N = rnd.nextLongLong(MAXN_2000 - 10, MAXN_2000);
         A = groupedBitonicArray(N, N / 10, floor(sqrt(N)), minimum, maximum);
         ensure());
  }

  void TestGroup9() {
    Subtasks({2, 4, 6, 7});
    const lint maximum = MAXA_50;
    const auto ensure = [&]() {
      if (A[0] == A[N - 1]) {
        lint f, s;
        if (rnd.nextLongLong(2) == 0) {
          f = 0, s = N - 1;
        } else {
          f = N - 1, s = 0;
        }
        if (A[f] > 0) {
          A[f]--;
        } else if (A[s] > 0) {
          A[s]--;
        } else {
          // Hopefully doesn't trigger
          A[f]++;
        }
      }
    };

    CASE(lint minimum = rnd.nextLongLong(10, maximum * 2 / 5); N = MAXN_2000;
         A = randomBitonicArray(N, minimum, maximum); ensure());
    CASE(lint minimum = rnd.nextLongLong(10, maximum * 2 / 5); N = MAXN_2000;
         A = randomBitonicArray(N, N, minimum, maximum); ensure());
    CASE(lint minimum = rnd.nextLongLong(10, maximum * 2 / 5); N = MAXN_2000;
         A = randomBitonicArray(N, N / 2, minimum, maximum); ensure());
    CASE(lint minimum = rnd.nextLongLong(10, maximum * 2 / 5); N = MAXN_2000;
         A = randomBitonicArray(N, floor(sqrt(N)), minimum, maximum); ensure());

    CASE(lint minimum = rnd.nextLongLong(10, maximum * 2 / 5);
         N = rnd.nextLongLong(MAXN_2000 - 10, MAXN_2000);
         A = randomBitonicArray(N, minimum, maximum); ensure());
    CASE(lint minimum = rnd.nextLongLong(10, maximum * 2 / 5);
         N = rnd.nextLongLong(MAXN_2000 - 10, MAXN_2000);
         A = randomBitonicArray(N, N, minimum, maximum); ensure());
    CASE(lint minimum = rnd.nextLongLong(10, maximum * 2 / 5);
         N = rnd.nextLongLong(MAXN_2000 - 10, MAXN_2000);
         A = randomBitonicArray(N, N / 2, minimum, maximum); ensure());
    CASE(lint minimum = rnd.nextLongLong(10, maximum * 2 / 5);
         N = rnd.nextLongLong(MAXN_2000 - 10, MAXN_2000);
         A = randomBitonicArray(N, floor(sqrt(N)), minimum, maximum); ensure());

    CASE(lint minimum = rnd.nextLongLong(10, maximum * 2 / 5);
         N = rnd.nextLongLong(MAXN_2000 - 10, MAXN_2000);
         A = groupedBitonicArray(N, floor(sqrt(N)), minimum, maximum);
         ensure());
    CASE(lint minimum = rnd.nextLongLong(10, maximum * 2 / 5);
         N = rnd.nextLongLong(MAXN_2000 - 10, MAXN_2000);
         A = groupedBitonicArray(N, floor(sqrt(N)), N, minimum, maximum);
         ensure());
    CASE(lint minimum = rnd.nextLongLong(10, maximum * 2 / 5);
         N = rnd.nextLongLong(MAXN_2000 - 10, MAXN_2000);
         A = groupedBitonicArray(N, floor(sqrt(N)), floor(sqrt(N)), minimum,
                                 maximum);
         ensure());

    CASE(lint minimum = rnd.nextLongLong(10, maximum * 2 / 5);
         N = rnd.nextLongLong(MAXN_2000 - 10, MAXN_2000);
         A = groupedBitonicArray(N, 10, minimum, maximum); ensure());
    CASE(lint minimum = rnd.nextLongLong(10, maximum * 2 / 5);
         N = rnd.nextLongLong(MAXN_2000 - 10, MAXN_2000);
         A = groupedBitonicArray(N, 10, N, minimum, maximum); ensure());
    CASE(lint minimum = rnd.nextLongLong(10, maximum * 2 / 5);
         N = rnd.nextLongLong(MAXN_2000 - 10, MAXN_2000);
         A = groupedBitonicArray(N, 10, floor(sqrt(N)), minimum, maximum);
         ensure());

    CASE(lint minimum = rnd.nextLongLong(10, maximum * 2 / 5);
         N = rnd.nextLongLong(MAXN_2000 - 10, MAXN_2000);
         A = groupedBitonicArray(N, N / 10, minimum, maximum); ensure());
    CASE(lint minimum = rnd.nextLongLong(10, maximum * 2 / 5);
         N = rnd.nextLongLong(MAXN_2000 - 10, MAXN_2000);
         A = groupedBitonicArray(N, N / 10, N, minimum, maximum); ensure());
    CASE(lint minimum = rnd.nextLongLong(10, maximum * 2 / 5);
         N = rnd.nextLongLong(MAXN_2000 - 10, MAXN_2000);
         A = groupedBitonicArray(N, N / 10, floor(sqrt(N)), minimum, maximum);
         ensure());
  }

  void TestGroup10() {
    Subtasks({4, 5, 6, 7});
    // same setting as TestGroup8
    const lint maximum = MAXA;
    const auto ensure = [&]() { A[0] = A[N - 1] = min(A[0], A[N - 1]); };

    CASE(lint minimum = rnd.nextLongLong(10, maximum * 2 / 5); N = MAXN_2000;
         A = randomBitonicArray(N, minimum, maximum); ensure());
    CASE(lint minimum = rnd.nextLongLong(10, maximum * 2 / 5); N = MAXN_2000;
         A = randomBitonicArray(N, N, minimum, maximum); ensure());
    CASE(lint minimum = rnd.nextLongLong(10, maximum * 2 / 5); N = MAXN_2000;
         A = randomBitonicArray(N, N / 2, minimum, maximum); ensure());
    CASE(lint minimum = rnd.nextLongLong(10, maximum * 2 / 5); N = MAXN_2000;
         A = randomBitonicArray(N, floor(sqrt(N)), minimum, maximum); ensure());

    CASE(lint minimum = rnd.nextLongLong(10, maximum * 2 / 5);
         N = rnd.nextLongLong(MAXN_2000 - 10, MAXN_2000);
         A = randomBitonicArray(N, minimum, maximum); ensure());
    CASE(lint minimum = rnd.nextLongLong(10, maximum * 2 / 5);
         N = rnd.nextLongLong(MAXN_2000 - 10, MAXN_2000);
         A = randomBitonicArray(N, N, minimum, maximum); ensure());
    CASE(lint minimum = rnd.nextLongLong(10, maximum * 2 / 5);
         N = rnd.nextLongLong(MAXN_2000 - 10, MAXN_2000);
         A = randomBitonicArray(N, N / 2, minimum, maximum); ensure());
    CASE(lint minimum = rnd.nextLongLong(10, maximum * 2 / 5);
         N = rnd.nextLongLong(MAXN_2000 - 10, MAXN_2000);
         A = randomBitonicArray(N, floor(sqrt(N)), minimum, maximum); ensure());

    CASE(lint minimum = rnd.nextLongLong(10, maximum * 2 / 5);
         N = rnd.nextLongLong(MAXN_2000 - 10, MAXN_2000);
         A = groupedBitonicArray(N, floor(sqrt(N)), minimum, maximum);
         ensure());
    CASE(lint minimum = rnd.nextLongLong(10, maximum * 2 / 5);
         N = rnd.nextLongLong(MAXN_2000 - 10, MAXN_2000);
         A = groupedBitonicArray(N, floor(sqrt(N)), N, minimum, maximum);
         ensure());
    CASE(lint minimum = rnd.nextLongLong(10, maximum * 2 / 5);
         N = rnd.nextLongLong(MAXN_2000 - 10, MAXN_2000);
         A = groupedBitonicArray(N, floor(sqrt(N)), floor(sqrt(N)), minimum,
                                 maximum);
         ensure());

    CASE(lint minimum = rnd.nextLongLong(10, maximum * 2 / 5);
         N = rnd.nextLongLong(MAXN_2000 - 10, MAXN_2000);
         A = groupedBitonicArray(N, 10, minimum, maximum); ensure());
    CASE(lint minimum = rnd.nextLongLong(10, maximum * 2 / 5);
         N = rnd.nextLongLong(MAXN_2000 - 10, MAXN_2000);
         A = groupedBitonicArray(N, 10, N, minimum, maximum); ensure());
    CASE(lint minimum = rnd.nextLongLong(10, maximum * 2 / 5);
         N = rnd.nextLongLong(MAXN_2000 - 10, MAXN_2000);
         A = groupedBitonicArray(N, 10, floor(sqrt(N)), minimum, maximum);
         ensure());

    CASE(lint minimum = rnd.nextLongLong(10, maximum * 2 / 5);
         N = rnd.nextLongLong(MAXN_2000 - 10, MAXN_2000);
         A = groupedBitonicArray(N, N / 10, minimum, maximum); ensure());
    CASE(lint minimum = rnd.nextLongLong(10, maximum * 2 / 5);
         N = rnd.nextLongLong(MAXN_2000 - 10, MAXN_2000);
         A = groupedBitonicArray(N, N / 10, N, minimum, maximum); ensure());
    CASE(lint minimum = rnd.nextLongLong(10, maximum * 2 / 5);
         N = rnd.nextLongLong(MAXN_2000 - 10, MAXN_2000);
         A = groupedBitonicArray(N, N / 10, floor(sqrt(N)), minimum, maximum);
         ensure());
  }

  void TestGroup11() {
    Subtasks({4, 6, 7});
    // same setting as TestGroup9
    const lint maximum = MAXA;
    const auto ensure = [&]() {
      if (A[0] == A[N - 1]) {
        lint f, s;
        if (rnd.nextLongLong(2) == 0) {
          f = 0, s = N - 1;
        } else {
          f = N - 1, s = 0;
        }
        if (A[f] > 0) {
          A[f]--;
        } else if (A[s] > 0) {
          A[s]--;
        } else {
          // Hopefully doesn't trigger
          A[f]++;
        }
      }
    };

    CASE(lint minimum = rnd.nextLongLong(10, maximum * 2 / 5); N = MAXN_2000;
         A = randomBitonicArray(N, minimum, maximum); ensure());
    CASE(lint minimum = rnd.nextLongLong(10, maximum * 2 / 5); N = MAXN_2000;
         A = randomBitonicArray(N, N, minimum, maximum); ensure());
    CASE(lint minimum = rnd.nextLongLong(10, maximum * 2 / 5); N = MAXN_2000;
         A = randomBitonicArray(N, N / 2, minimum, maximum); ensure());
    CASE(lint minimum = rnd.nextLongLong(10, maximum * 2 / 5); N = MAXN_2000;
         A = randomBitonicArray(N, floor(sqrt(N)), minimum, maximum); ensure());

    CASE(lint minimum = rnd.nextLongLong(10, maximum * 2 / 5);
         N = rnd.nextLongLong(MAXN_2000 - 10, MAXN_2000);
         A = randomBitonicArray(N, minimum, maximum); ensure());
    CASE(lint minimum = rnd.nextLongLong(10, maximum * 2 / 5);
         N = rnd.nextLongLong(MAXN_2000 - 10, MAXN_2000);
         A = randomBitonicArray(N, N, minimum, maximum); ensure());
    CASE(lint minimum = rnd.nextLongLong(10, maximum * 2 / 5);
         N = rnd.nextLongLong(MAXN_2000 - 10, MAXN_2000);
         A = randomBitonicArray(N, N / 2, minimum, maximum); ensure());
    CASE(lint minimum = rnd.nextLongLong(10, maximum * 2 / 5);
         N = rnd.nextLongLong(MAXN_2000 - 10, MAXN_2000);
         A = randomBitonicArray(N, floor(sqrt(N)), minimum, maximum); ensure());

    CASE(lint minimum = rnd.nextLongLong(10, maximum * 2 / 5);
         N = rnd.nextLongLong(MAXN_2000 - 10, MAXN_2000);
         A = groupedBitonicArray(N, floor(sqrt(N)), minimum, maximum);
         ensure());
    CASE(lint minimum = rnd.nextLongLong(10, maximum * 2 / 5);
         N = rnd.nextLongLong(MAXN_2000 - 10, MAXN_2000);
         A = groupedBitonicArray(N, floor(sqrt(N)), N, minimum, maximum);
         ensure());
    CASE(lint minimum = rnd.nextLongLong(10, maximum * 2 / 5);
         N = rnd.nextLongLong(MAXN_2000 - 10, MAXN_2000);
         A = groupedBitonicArray(N, floor(sqrt(N)), floor(sqrt(N)), minimum,
                                 maximum);
         ensure());

    CASE(lint minimum = rnd.nextLongLong(10, maximum * 2 / 5);
         N = rnd.nextLongLong(MAXN_2000 - 10, MAXN_2000);
         A = groupedBitonicArray(N, 10, minimum, maximum); ensure());
    CASE(lint minimum = rnd.nextLongLong(10, maximum * 2 / 5);
         N = rnd.nextLongLong(MAXN_2000 - 10, MAXN_2000);
         A = groupedBitonicArray(N, 10, N, minimum, maximum); ensure());
    CASE(lint minimum = rnd.nextLongLong(10, maximum * 2 / 5);
         N = rnd.nextLongLong(MAXN_2000 - 10, MAXN_2000);
         A = groupedBitonicArray(N, 10, floor(sqrt(N)), minimum, maximum);
         ensure());

    CASE(lint minimum = rnd.nextLongLong(10, maximum * 2 / 5);
         N = rnd.nextLongLong(MAXN_2000 - 10, MAXN_2000);
         A = groupedBitonicArray(N, N / 10, minimum, maximum); ensure());
    CASE(lint minimum = rnd.nextLongLong(10, maximum * 2 / 5);
         N = rnd.nextLongLong(MAXN_2000 - 10, MAXN_2000);
         A = groupedBitonicArray(N, N / 10, N, minimum, maximum); ensure());
    CASE(lint minimum = rnd.nextLongLong(10, maximum * 2 / 5);
         N = rnd.nextLongLong(MAXN_2000 - 10, MAXN_2000);
         A = groupedBitonicArray(N, N / 10, floor(sqrt(N)), minimum, maximum);
         ensure());
  }

  void TestGroup12() {
    Subtasks({2, 5, 6, 7});
    const bool do_obfuscate = false;
    const bool same_start_end = true;
    const int maximum = MAXA_50;
    const double longChainFraction = 0.01;

    CASE(N = MAXN_2000;
         A = repeatUntil(
             [&]() { return antiMagicArray(N, 1, maximum); },
             [&](const vector<lint>& a) { return a[0] == a[N - 1]; }));

    const vector<lint> partition_sizes = {5, lint(floor(sqrt(MAXN_2000))),
                                          MAXN_2000 / 10};
    for (const lint num_partition : partition_sizes) {
      for (const bool isBiased : {false, true}) {
        CASE(N = rnd.nextLongLong(MAXN_2000 - 10, MAXN_2000);
             A = gen_test(N, -1, 0, 1, 0, 0, 1, maximum, longChainFraction, 15,
                          1e-2, num_partition, do_obfuscate, same_start_end,
                          isBiased ? rnd.nextInt(1, 5) : -1));
        CASE(N = rnd.nextLongLong(MAXN_2000 - 10, MAXN_2000);
             A = gen_test(N, N, 0, 1, 0, 0, 1, maximum, longChainFraction, 15,
                          1e-2, num_partition, do_obfuscate, same_start_end,
                          isBiased ? rnd.nextInt(1, 5) : -1));
        CASE(N = rnd.nextLongLong(MAXN_2000 - 10, MAXN_2000);
             A = gen_test(N, -1, 0, 1, 0, 0, 1, maximum, longChainFraction, 15,
                          1e-2, num_partition, do_obfuscate, same_start_end,
                          isBiased ? rnd.nextInt(1, 5) : -1));
        CASE(N = rnd.nextLongLong(MAXN_2000 - 10, MAXN_2000);
             A = gen_test(N, N, 0, 1, 0, 0, 1, maximum, longChainFraction, 15,
                          1e-2, num_partition, do_obfuscate, same_start_end,
                          isBiased ? rnd.nextInt(1, 5) : -1));
      }
    }
  }

  void TestGroup13() {
    Subtasks({2, 6, 7});
    const bool do_obfuscate = true;
    const bool same_start_end = false;
    const int maximum = MAXA_50;
    const double longChainFraction = 0.01;

    CASE(N = MAXN_2000; A = randomArray(N, 0, maximum));
    CASE(N = MAXN_2000; A = randomArray(N, floor(sqrt(N)), 0, maximum));
    CASE(N = MAXN_2000; A = randomArray(N, N / 2, 0, maximum));
    CASE(N = MAXN_2000; A = randomArray(N, N, 0, maximum));

    CASE(N = MAXN_2000; A = swapArray(sortedArray(N, 1, maximum), N / 10));
    CASE(N = MAXN_2000; A = swapArray(sortedArray(N, N, 1, maximum), N / 10));
    CASE(N = MAXN_2000;
         A = swapArray(groupedBitonicArray(N, 10, 1, maximum), N / 10));
    CASE(N = MAXN_2000;
         A = swapArray(groupedBitonicArray(N, 10, N, 1, maximum), N / 10));

    const vector<lint> partition_sizes = {5, lint(floor(sqrt(MAXN_2000))),
                                          MAXN_2000 / 10};
    for (const lint num_partition : partition_sizes) {
      for (const bool isBiased : {false, true}) {
        CASE(N = rnd.nextLongLong(MAXN_2000 - 10, MAXN_2000);
             A = repeatUntil(
                 [&]() {
                   return gen_test(N, -1, 0, 1, 0, 0, 1, maximum,
                                   longChainFraction, 15, 1e-2, num_partition,
                                   do_obfuscate, same_start_end,
                                   isBiased ? rnd.nextInt(1, 5) : -1);
                 },
                 [&](const vector<lint>& a) {
                   return a[0] != a[N - 1] ||
                          *min_element(a.begin(), a.end()) < a[0];
                 }));
        CASE(N = rnd.nextLongLong(MAXN_2000 - 10, MAXN_2000);
             A = repeatUntil(
                 [&]() {
                   return gen_test(N, N, 0, 1, 0, 0, 1, maximum,
                                   longChainFraction, 15, 1e-2, num_partition,
                                   do_obfuscate, same_start_end,
                                   isBiased ? rnd.nextInt(1, 5) : -1);
                 },
                 [&](const vector<lint>& a) {
                   return a[0] != a[N - 1] ||
                          *min_element(a.begin(), a.end()) < a[0];
                 }));
        CASE(N = rnd.nextLongLong(MAXN_2000 - 10, MAXN_2000);
             A = repeatUntil(
                 [&]() {
                   return gen_test(N, -1, 0, 1, 0, 0, 1, maximum,
                                   longChainFraction, 15, 1e-2, num_partition,
                                   do_obfuscate, same_start_end,
                                   isBiased ? rnd.nextInt(1, 5) : -1);
                 },
                 [&](const vector<lint>& a) {
                   return a[0] != a[N - 1] ||
                          *min_element(a.begin(), a.end()) < a[0];
                 }));
        CASE(N = rnd.nextLongLong(MAXN_2000 - 10, MAXN_2000);
             A = repeatUntil(
                 [&]() {
                   return gen_test(N, N, 0, 1, 0, 0, 1, maximum,
                                   longChainFraction, 15, 1e-2, num_partition,
                                   do_obfuscate, same_start_end,
                                   isBiased ? rnd.nextInt(1, 5) : -1);
                 },
                 [&](const vector<lint>& a) {
                   return a[0] != a[N - 1] ||
                          *min_element(a.begin(), a.end()) < a[0];
                 }));
      }
    }
  }

  void TestGroup14() {
    Subtasks({5, 6, 7});

    const bool do_obfuscate = false;
    const bool same_start_end = true;
    const double longChainFraction = 0.01;

    const vector<lint> partition_sizes = {5, lint(floor(sqrt(MAXN_2000))),
                                          MAXN_2000 / 10};
    for (const lint num_partition : partition_sizes) {
      for (const bool isBiased : {false, true}) {
        CASE(N = rnd.nextLongLong(MAXN_2000 - 10, MAXN_2000);
             A = gen_test(N, -1, 3, 1000, 14, 30, 5, MAXA, longChainFraction,
                          15, 1e-2, num_partition, do_obfuscate, same_start_end,
                          isBiased ? rnd.nextInt(1, 5) : -1));
        CASE(N = rnd.nextLongLong(MAXN_2000 - 10, MAXN_2000);
             A = gen_test(N, N, 3, 1000, 14, 30, 5, MAXA, longChainFraction, 15,
                          1e-2, num_partition, do_obfuscate, same_start_end,
                          isBiased ? rnd.nextInt(1, 5) : -1));
        CASE(N = rnd.nextLongLong(MAXN_2000 - 10, MAXN_2000);
             A = gen_test(N, -1, 0, 1, 0, 0, 1, MAXA, longChainFraction, 15,
                          1e-2, num_partition, do_obfuscate, same_start_end,
                          isBiased ? rnd.nextInt(1, 5) : -1));
        CASE(N = rnd.nextLongLong(MAXN_2000 - 10, MAXN_2000);
             A = gen_test(N, N, 0, 1, 0, 0, 1, MAXA, longChainFraction, 15,
                          1e-2, num_partition, do_obfuscate, same_start_end,
                          isBiased ? rnd.nextInt(1, 5) : -1));
        CASE(N = rnd.nextLongLong(MAXN_2000 - 10, MAXN_2000);
             A = gen_test(N, -1, 0, 1, 0, 0, 1, MAXA, longChainFraction, 15,
                          1e-2, num_partition, do_obfuscate, same_start_end,
                          isBiased ? rnd.nextInt(1, 5) : -1));
        CASE(N = rnd.nextLongLong(MAXN_2000 - 10, MAXN_2000);
             A = gen_test(N, N, 0, 1, 0, 0, 1, MAXA, longChainFraction, 15,
                          1e-2, num_partition, do_obfuscate, same_start_end,
                          isBiased ? rnd.nextInt(1, 5) : -1));
      }
    }
  }

  void TestGroup15() {
    Subtasks({6, 7});

    const bool do_obfuscate = true;
    const bool same_start_end = false;
    const double longChainFraction = 0.01;

    CASE(N = MAXN_2000; A = antiMagicArray(N, 1, MAXA));

    CASE(N = MAXN_2000; A = randomArray(N, 0, MAXA));
    CASE(N = MAXN_2000; A = randomArray(N, floor(sqrt(N)), 0, MAXA));
    CASE(N = MAXN_2000; A = randomArray(N, N / 2, 0, MAXA));
    CASE(N = MAXN_2000; A = randomArray(N, N, 0, MAXA));

    CASE(N = MAXN_2000; A = swapArray(sortedArray(N, 0, MAXA), N / 10));
    CASE(N = MAXN_2000; A = swapArray(sortedArray(N, N, 0, MAXA), N / 10));
    CASE(N = MAXN_2000;
         A = swapArray(groupedBitonicArray(N, 10, 0, MAXA), N / 10));
    CASE(N = MAXN_2000;
         A = swapArray(groupedBitonicArray(N, 10, N, 0, MAXA), N / 10));

    const vector<lint> partition_sizes = {5, lint(floor(sqrt(MAXN_2000))),
                                          MAXN_2000 / 10};
    for (const lint num_partition : partition_sizes) {
      for (const bool isBiased : {false, true}) {
        CASE(N = rnd.nextLongLong(MAXN_2000 - 10, MAXN_2000);
             A = repeatUntil(
                 [&]() {
                   return gen_test(N, -1, 3, 1000, 14, 30, 5, MAXA,
                                   longChainFraction, 15, 1e-2, num_partition,
                                   do_obfuscate, same_start_end,
                                   isBiased ? rnd.nextInt(1, 5) : -1);
                 },
                 [&](const vector<lint>& a) {
                   return a[0] != a[N - 1] ||
                          *min_element(a.begin(), a.end()) < a[0];
                 }));
        CASE(N = rnd.nextLongLong(MAXN_2000 - 10, MAXN_2000);
             A = repeatUntil(
                 [&]() {
                   return gen_test(N, N, 3, 1000, 14, 30, 5, MAXA,
                                   longChainFraction, 15, 1e-2, num_partition,
                                   do_obfuscate, same_start_end,
                                   isBiased ? rnd.nextInt(1, 5) : -1);
                 },
                 [&](const vector<lint>& a) {
                   return a[0] != a[N - 1] ||
                          *min_element(a.begin(), a.end()) < a[0];
                 }));
        CASE(N = rnd.nextLongLong(MAXN_2000 - 10, MAXN_2000);
             A = repeatUntil(
                 [&]() {
                   return gen_test(N, -1, 0, 1, 0, 0, 1, MAXA,
                                   longChainFraction, 15, 1e-2, num_partition,
                                   do_obfuscate, same_start_end,
                                   isBiased ? rnd.nextInt(1, 5) : -1);
                 },
                 [&](const vector<lint>& a) {
                   return a[0] != a[N - 1] ||
                          *min_element(a.begin(), a.end()) < a[0];
                 }));
        CASE(N = rnd.nextLongLong(MAXN_2000 - 10, MAXN_2000);
             A = repeatUntil(
                 [&]() {
                   return gen_test(N, N, 0, 1, 0, 0, 1, MAXA, longChainFraction,
                                   15, 1e-2, num_partition, do_obfuscate,
                                   same_start_end,
                                   isBiased ? rnd.nextInt(1, 5) : -1);
                 },
                 [&](const vector<lint>& a) {
                   return a[0] != a[N - 1] ||
                          *min_element(a.begin(), a.end()) < a[0];
                 }));
      }
    }
  }

  void TestGroup16() {
    Subtasks({7});

    const bool do_obfuscate = true;
    const bool same_start_end = false;
    const double longChainFraction = 5e-4;

    CASE(N = MAXN; A = antiMagicArray(N, 1, MAXA));
    CASE(N = MAXN; A = randomAntiBitonicArray(N, 1, MAXA));
    CASE(N = MAXN; A = groupedAntiBitonicArray(N, 10, 1, MAXA));
    CASE(N = MAXN; A = sortedArray(N, 0, MAXA));
    CASE(N = MAXN; A = sortedArray(N, 0, MAXA); reverse(begin(A), end(A)));

    CASE(N = MAXN; A = randomArray(N, 0, MAXA));
    CASE(N = MAXN; A = randomArray(N, floor(sqrt(N)), 0, MAXA));
    CASE(N = MAXN; A = randomArray(N, N / 2, 0, MAXA));
    CASE(N = MAXN; A = randomArray(N, N, 0, MAXA));

    CASE(N = MAXN; A = swapArray(sortedArray(N, 0, MAXA), N / 10));
    CASE(N = MAXN; A = swapArray(sortedArray(N, N, 0, MAXA), N / 10));
    CASE(N = MAXN; A = swapArray(groupedBitonicArray(N, 10, 0, MAXA), N / 10));
    CASE(N = MAXN;
         A = swapArray(groupedBitonicArray(N, 10, N, 0, MAXA), N / 10));
    CASE(N = MAXN;
         A = swapArray(groupedAntiBitonicArray(N, 10, 0, MAXA), N / 10));
    CASE(N = MAXN;
         A = swapArray(groupedAntiBitonicArray(N, 10, N, 0, MAXA), N / 10));

    const vector<lint> partition_sizes = {5, lint(floor(sqrt(MAXN))),
                                          MAXN / 10};
    for (const lint num_partition : partition_sizes) {
      for (const bool isBiased : {false, true}) {
        CASE(N = rnd.nextLongLong(MAXN - 100, MAXN);
             A = repeatUntil(
                 [&]() {
                   return gen_test(N, ceil(sqrt(N)), 3, 1000, 14, 30, 5, MAXA,
                                   longChainFraction, 100, 1e-3,
                                   rnd.nextInt(3 * num_partition / 5,
                                               7 * num_partition / 5),
                                   do_obfuscate, same_start_end,
                                   isBiased ? rnd.nextInt(1, 5) : -1);
                 },
                 [&](const vector<lint>& a) {
                   return a[0] != a[N - 1] ||
                          *min_element(a.begin(), a.end()) < a[0];
                 }));
        CASE(N = rnd.nextLongLong(MAXN - 100, MAXN);
             A = repeatUntil(
                 [&]() {
                   return gen_test(N, N, 3, 1000, 14, 30, 5, MAXA,
                                   longChainFraction, 100, 1e-3,
                                   rnd.nextInt(3 * num_partition / 5,
                                               7 * num_partition / 5),
                                   do_obfuscate, same_start_end,
                                   isBiased ? rnd.nextInt(1, 5) : -1);
                 },
                 [&](const vector<lint>& a) {
                   return a[0] != a[N - 1] ||
                          *min_element(a.begin(), a.end()) < a[0];
                 }));
        CASE(N = rnd.nextLongLong(MAXN - 100, MAXN);
             A = repeatUntil(
                 [&]() {
                   return gen_test(N, ceil(sqrt(N)), 0, 1, 0, 0, 1, MAXA,
                                   longChainFraction, 100, 1e-3,
                                   rnd.nextInt(3 * num_partition / 5,
                                               7 * num_partition / 5),
                                   do_obfuscate, same_start_end,
                                   isBiased ? rnd.nextInt(1, 5) : -1);
                 },
                 [&](const vector<lint>& a) {
                   return a[0] != a[N - 1] ||
                          *min_element(a.begin(), a.end()) < a[0];
                 }));
        CASE(N = rnd.nextLongLong(MAXN - 100, MAXN);
             A = repeatUntil(
                 [&]() {
                   return gen_test(N, N, 0, 1, 0, 0, 1, MAXA, longChainFraction,
                                   100, 1e-3,
                                   rnd.nextInt(3 * num_partition / 5,
                                               7 * num_partition / 5),
                                   do_obfuscate, same_start_end,
                                   isBiased ? rnd.nextInt(1, 5) : -1);
                 },
                 [&](const vector<lint>& a) {
                   return a[0] != a[N - 1] ||
                          *min_element(a.begin(), a.end()) < a[0];
                 }));
      }
    }
  }

 private:
  void expandCoordinate(vector<lint>& A, lint K, lint loC, lint hiC) {
    vector<lint> coords(K);
    for (lint i = 0; i < K; i++) {
      coords[i] = rnd.nextLongLong(loC, hiC);
    }
    sort(begin(coords), end(coords));
    for (lint i = 0; i < lint(A.size()); i++) {
      assert(0 <= A[i] && A[i] < K);
      A[i] = coords[A[i]];
    }
  }

  vector<lint> randomArray(lint N, lint loC, lint hiC) {
    vector<lint> ret(N);
    for (lint i = 0; i < N; i++) {
      ret[i] = rnd.nextLongLong(loC, hiC);
    }
    return ret;
  }

  vector<lint> randomArray(lint N, lint K, lint loC, lint hiC) {
    vector<lint> ret = randomArray(N, 0, K - 1);
    expandCoordinate(ret, K, loC, hiC);
    return ret;
  }

  vector<lint> sortedArray(lint N, lint loC, lint hiC) {
    vector<lint> ret = randomArray(N, loC, hiC);
    sort(begin(ret), end(ret));
    return ret;
  }

  vector<lint> sortedArray(lint N, lint K, lint loC, lint hiC) {
    vector<lint> ret = randomArray(N, K, loC, hiC);
    sort(begin(ret), end(ret));
    return ret;
  }

  vector<lint> randomBitonicArray(lint N, lint loC, lint hiC) {
    vector<lint> coords(N);
    for (lint i = 0; i < N; i++) {
      coords[i] = rnd.nextLongLong(loC, hiC);
    }
    sort(begin(coords), end(coords), greater<>());
    deque<lint> dq;
    for (auto c : coords) {
      if (rnd.nextLongLong(0, 1) == 0) {
        dq.emplace_back(c);
      } else {
        dq.emplace_front(c);
      }
    }
    vector<lint> ret;
    for (auto i : dq) {
      ret.emplace_back(i);
    }
    if (is_sorted(begin(ret), end(ret))) {
      ret = randomBitonicArray(N, loC, hiC);
    }
    return ret;
  }

  vector<lint> randomBitonicArray(lint N, lint K, lint loC, lint hiC) {
    vector<lint> ret = randomBitonicArray(N, 0, K - 1);
    expandCoordinate(ret, K, loC, hiC);
    return ret;
  }

  vector<lint> randomAntiBitonicArray(lint N, lint loC, lint hiC) {
    vector<lint> coords(N);
    for (lint i = 0; i < N; i++) {
      coords[i] = rnd.nextLongLong(loC, hiC);
    }
    sort(begin(coords), end(coords));
    deque<lint> dq;
    for (auto c : coords) {
      if (rnd.nextLongLong(0, 1) == 0) {
        dq.emplace_back(c);
      } else {
        dq.emplace_front(c);
      }
    }
    vector<lint> ret;
    for (auto i : dq) {
      ret.emplace_back(i);
    }
    if (is_sorted(begin(ret), end(ret))) {
      ret = randomAntiBitonicArray(N, loC, hiC);
    }
    return ret;
  }

  vector<lint> randomAntiBitonicArray(lint N, lint K, lint loC, lint hiC) {
    vector<lint> ret = randomAntiBitonicArray(N, 0, K - 1);
    expandCoordinate(ret, K, loC, hiC);
    return ret;
  }

  vector<lint> groupedBitonicArray(lint N, lint B, lint loC, lint hiC) {
    vector<lint> coords(N);
    for (lint i = 0; i < N; i++) {
      coords[i] = rnd.nextLongLong(loC, hiC);
    }
    sort(begin(coords), end(coords), greater<>());
    vector<lint> color;
    for (lint i = 0; i < N; i++) {
      color.emplace_back(rnd.nextLongLong(0, B - 1));
    }
    sort(begin(color), end(color));
    deque<lint> dq;
    for (lint i = 0; i < N; i++) {
      if (color[i] & 1) {
        dq.emplace_back(coords[i]);
      } else {
        dq.emplace_front(coords[i]);
      }
    }
    vector<lint> ret;
    for (auto i : dq) {
      ret.emplace_back(i);
    }
    return ret;
  }

  vector<lint> groupedBitonicArray(lint N, lint B, lint K, lint loC, lint hiC) {
    vector<lint> ret = groupedBitonicArray(N, B, 0, K - 1);
    expandCoordinate(ret, K, loC, hiC);
    return ret;
  }

  vector<lint> groupedAntiBitonicArray(lint N, lint B, lint loC, lint hiC) {
    vector<lint> coords(N);
    for (lint i = 0; i < N; i++) {
      coords[i] = rnd.nextLongLong(loC, hiC);
    }
    sort(begin(coords), end(coords));
    vector<lint> color;
    for (lint i = 0; i < N; i++) {
      color.emplace_back(rnd.nextLongLong(0, B - 1));
    }
    sort(begin(color), end(color));
    deque<lint> dq;
    for (lint i = 0; i < N; i++) {
      if (color[i] & 1) {
        dq.emplace_back(coords[i]);
      } else {
        dq.emplace_front(coords[i]);
      }
    }
    vector<lint> ret;
    for (auto i : dq) {
      ret.emplace_back(i);
    }
    return ret;
  }

  vector<lint> groupedAntiBitonicArray(lint N, lint B, lint K, lint loC,
                                       lint hiC) {
    vector<lint> ret = groupedAntiBitonicArray(N, B, 0, K - 1);
    expandCoordinate(ret, K, loC, hiC);
    return ret;
  }

  vector<lint> antiMagicArray(lint N, lint loC, lint hiC) {
    assert(N % 2 == 0);
    vector<lint> coords(N);
    for (lint i = 0; i < N; i++) {
      coords[i] = rnd.nextLongLong(loC, hiC / 2);
    }
    vector<lint> ret;
    sort(begin(coords), end(coords), greater<>());
    coords.pop_back();
    coords.emplace_back(loC);
    for (lint i = 0; i < N; i++) {
      if (i + 1 < N) {
        if (i == 0) {
          coords[i + 1] =
              rnd.nextLongLong((coords[i + 2] + coords[i]) / 2, coords[i]);
          ret.emplace_back(coords[i + 1]);
          ret.emplace_back(coords[i]);
          ret.emplace_back(coords[i + 2]);
          i += 1;
        } else {
          ret.emplace_back(coords[i + 1]);
          ret.emplace_back(coords[i]);
        }
        i += 1;
      } else {
        ret.insert(begin(ret), coords[i]);
      }
    }
    assert(ret.size() == N);
    return ret;
  }

  vector<lint> customCartesianTree(lint N, lint K, lint antiNum, lint antiC,
                                   lint antiSizeL, lint antiSizeR, lint loC,
                                   lint hiC, double longChainFraction,
                                   lint numLongChain, double probChangeDir,
                                   vector<lint>& coordsK) {
    vector<lint> ret(N, -1);
    vector<pair<lint, lint>> ranges;
    lint ID = 0;
    vector<lint> placeMid(N);
    for (lint i = 0; i < numLongChain && i < N; i++) {
      placeMid[i] = 1;
    }
    rnd.shuffle(begin(placeMid), end(placeMid));
    const auto Dfs = [&](const auto& self, lint L, lint R, lint lastdir,
                         ordered_set<pair<lint, lint>>& oset) -> void {
      if (L > R) return;
      lint id = ID++;
      lint len = R - L + 1;
      assert(oset.size() == len);
      if (N >= 20 && antiSizeL <= len && len <= antiSizeR) {
        if (len % 2 == 0) {
          ranges.emplace_back(L, R);
        } else {
          lint p = rnd.nextLongLong(2);
          if (p == 0) {
            ret[L] = prev(end(oset))->first;
            oset.erase(prev(end(oset)));
            self(self, L + 1, R, 0, oset);
          } else {
            ret[R] = prev(end(oset))->first;
            oset.erase(prev(end(oset)));
            self(self, L, R - 1, 1, oset);
          }
        }
        return;
      }
      lint M = -1;
      if (placeMid[id]) {
        M = rnd.nextLongLong(L, R);
      } else {
        if (lastdir == -1 || rnd.nextDouble(1) < probChangeDir) {
          lastdir = rnd.nextLongLong(2);
        }
        const lint longChain =
            min(20ll, static_cast<lint>(floor(len * longChainFraction)));
        if (lastdir == 0) {
          M = rnd.nextLongLong(L, min(R, L + longChain));
        } else {
          M = rnd.nextLongLong(max(L, R - longChain), R);
        }
      }
      ret[M] = prev(end(oset))->first;
      oset.erase(prev(end(oset)));
      lint cnt = min(M - L, R - M);
      ordered_set<pair<lint, lint>> other;
      while (cnt--) {
        lint id = rnd.nextLongLong(oset.size());
        auto it = oset.find_by_order(id);
        other.insert(*it);
        oset.erase(it);
      }
      if (M - L <= R - M) {
        self(self, L, M - 1, lastdir == 0 ? -1 : lastdir, other);
        self(self, M + 1, R, lastdir == 1 ? -1 : lastdir, oset);
      } else {
        self(self, L, M - 1, lastdir == 0 ? -1 : lastdir, oset);
        self(self, M + 1, R, lastdir == 1 ? -1 : lastdir, other);
      }
    };
    ordered_set<pair<lint, lint>> oset;
    while (oset.size() < N) {
      oset.insert({K == -1 ? rnd.nextLongLong(loC, hiC / antiC)
                           : rnd.nextLongLong(0, K - 1),
                   rnd.nextLongLong(MAXA)});
    }
    Dfs(Dfs, 0, N - 1, -1, oset);
    if (K != -1) {
      if (coordsK.size() != K) {
        // cerr << "gen coordsK: " << K << '\n';
        vector<lint> coords(K);
        for (lint i = 0; i < K; i++) {
          coords[i] = rnd.nextLongLong(loC, hiC / antiC);
        }
        coordsK = coords;
        sort(begin(coordsK), end(coordsK), greater<>());
      }
      for (lint i = 0; i < N; i++) {
        if (ret[i] != -1) {
          assert(0 <= ret[i] && ret[i] < K);
          ret[i] = coordsK[ret[i]];
        }
      }
    } else {
      assert(false);
    }
    for (lint i = 0; i < N; i++) {
      if (ret[i] != -1) {
        ret[i] *= antiC;
      }
    }
    if (N >= 20) {
      rnd.shuffle(begin(ranges), end(ranges));
      for (lint i = 0; i < lint(ranges.size()); i++) {
        auto [l, r] = ranges[i];
        vector<lint> anti = antiMagicArray(
            r - l + 1, 1, rnd.nextLongLong(2 * antiC / 3, antiC));
        for (lint j = l; j <= r; j++) {
          ret[j] = anti[j - l];
        }
      }
    }
    return ret;
  }

  vector<lint> swapArray(vector<lint> A, lint swaps) {
    while (swaps--) {
      lint i = rnd.nextLongLong(A.size());
      lint j = rnd.nextLongLong(A.size());
      swap(A[i], A[j]);
    }
    return A;
  }

  vector<lint> gen_test(lint N, lint K, lint antiNum, lint antiC,
                        lint antiSizeL, lint antiSizeR, lint loC, lint hiC,
                        double longChain, lint numLongChain,
                        double probChangeDir, lint numPartition,
                        bool doObfuscate, bool sameStartEnd,
                        lint partitionBias = -1) {
    if (K == -1) K = 5 * N;
    if (partitionBias == -1) partitionBias = numPartition;
    partitionBias = min(partitionBias, numPartition);
    assert(doObfuscate != sameStartEnd);
    N -= 2;
    assert(numPartition >= 2);
    vector<lint> coordsK;
    vector<lint> partitions(numPartition);
    vector<lint> betweenPartitions(numPartition - 1);
    lint slots = rnd.nextLongLong(
        numPartition - 1, max(numPartition - 1, min(3 * numPartition, N / 4)));
    assert(N >= slots);
    for (lint i = 0; i < N - slots; i++) {
      if (i < 3 * numPartition) {
        partitions[i % numPartition]++;
      } else if (rnd.nextDouble(1) < 0.5) {
        partitions[rnd.nextLongLong(0, numPartition - 1)]++;
      } else {
        partitions[rnd.nextLongLong(0, partitionBias - 1)]++;
      }
    }
    for (lint i = 0; i < slots; i++) {
      if (i < betweenPartitions.size()) {
        betweenPartitions[i]++;
      } else {
        betweenPartitions[rnd.nextLongLong(0, numPartition - 2)]++;
      }
    }
    // {
    //   cerr << "Top 5 Partition Sizes: ";
    //   vector<lint> top5 = partitions;
    //   sort(begin(top5), end(top5), greater<>());
    //   for (lint i = 0; i < min(lint(top5.size()), 5ll); i++) {
    //     cerr << top5[i] << " ";
    //   }
    //   cerr << '\n';
    // }
    vector<lint> ret;
    const lint rangeC = hiC - loC;
    lint maxCartesianDepth = 0;
    for (lint i = 0; i < numPartition; i++) {
      vector<lint> arr = customCartesianTree(
          partitions[i], K, antiNum, antiC, antiSizeL, antiSizeR, loC,
          hiC - rangeC / 2, longChain, numLongChain, probChangeDir, coordsK);
      maxCartesianDepth = max(maxCartesianDepth, cartesianDepth(arr));
      for (lint j = 0; j < lint(arr.size()); j++) {
        ret.emplace_back(arr[j]);
      }
      if (i + 1 < numPartition) {
        for (lint j = 0; j < betweenPartitions[i]; j++) {
          ret.emplace_back(0);
        }
      }
    }
    // cerr << "maxCartesianDepth = " << maxCartesianDepth << '\n';
    assert(ret.size() == N);
    lint r = rnd.nextLongLong(1, rangeC / 2);
    for (lint i = 0; i < N; i++) {
      ret[i] += r;
    }
    N += 2;
    ret.insert(begin(ret), r);
    ret.emplace_back(r);
    assert(*min_element(begin(ret), end(ret)) == r);
    if (doObfuscate) {
      assert(!sameStartEnd);
      for (lint i = 0; i < N; i++) {
        assert(loC <= ret[i] && ret[i] <= hiC);
      }
      ret = obfuscate(ret, hiC);
    }
    return ret;
  }

  vector<lint> obfuscate(vector<lint> A, lint max_value) {
    auto oldA = A;
    assert(N == lint(A.size()));
    assert(max_value >= *max_element(begin(A), end(A)));
    vector<lint> min_indices;
    const lint minimum = *min_element(begin(A), end(A));
    const lint gap = max_value - lint(*max_element(begin(A), end(A)));
    for (lint i = 0; i < lint(A.size()); i++) {
      if (A[i] == minimum) {
        min_indices.emplace_back(i);
      }
    }
    assert(min_indices[0] == 0 && min_indices.back() == N - 1);
    // cerr << "min_indices.size() = " << min_indices.size() << '\n';
    lint pL = rnd.nextLongLong(0, lint(min_indices.size()) - 1);
    lint pR = rnd.nextLongLong(0, lint(min_indices.size()) - 1);
    if (pL > pR) {
      swap(pL, pR);
    }
    vector<lint> adderLft = randomArray(pL, 0, gap);
    sort(begin(adderLft), end(adderLft), greater<>());
    for (lint i = pL - 1; i >= 0; i--) {
      for (lint j = min_indices[i]; j < min_indices[i + 1]; j++) {
        A[j] += adderLft[i];
      }
    }
    vector<lint> adderRgt =
        randomArray(lint(min_indices.size()) - pR - 1, 0, gap);
    sort(begin(adderRgt), end(adderRgt));
    for (lint i = pR + 1; i < lint(min_indices.size()); i++) {
      for (lint j = min_indices[i - 1] + 1; j <= min_indices[i]; j++) {
        A[j] += adderRgt[i - pR - 1];
      }
    }
    assert(*min_element(begin(A), end(A)) == minimum);
    assert(oldA == runPhase1(A));
    return A;
  }

  template <typename T, typename U>
  vector<lint> repeatUntil(const T& test, const U& condition,
                           const double MAX_TIME = 10.0) {
    const double start_time = clock();
    vector<lint> ret = test();
    lint count_iters = 0;
    while ((double(clock() - start_time) / CLOCKS_PER_SEC < MAX_TIME) &&
           !condition(ret)) {
      ret = test();
      count_iters++;
      // if (count_iters % 1000 == 0) cerr << count_iters << '\n';
    }
    if (!condition(ret)) {
      // cerr << "fail to generate counter\n";
    }
    return ret;
  }

  template <typename T, typename U, typename S>
  vector<lint> repeatUntil(const T& test, const U& wa, const S& ac,
                           const double MAX_TIME = 10.0) {
    return repeatUntil(test, [&](auto x) { return wa(x) != ac(x); }, MAX_TIME);
  }

  template <typename T>
  bool isBitonic(const vector<T>& A) {
    lint k = 1;
    while (k < lint(A.size()) && A[k - 1] <= A[k]) {
      k += 1;
    }
    return is_sorted(begin(A) + k, end(A), greater<>());
  }

  vector<lint> runPhase1(vector<lint> A) {
    int N = A.size();
    SegTree<0> segLft(N);
    SegTree<1> segRgt(N);
    segLft.Build(A);
    segRgt.Build(A);
    auto Update = [&](int l, int r, data_t x) {
      segLft.RangeAdd(l, r, x);
      segRgt.RangeAdd(l, r, x);
    };

    while (true) {
      auto [val, idx] = segLft.Query(0, N - 1);
      if (idx == 0) break;
      auto secondMin = segLft.Query(0, idx - 1).first;
      Update(0, idx - 1, val - secondMin);
    }

    while (true) {
      auto [val, idx] = segRgt.Query(0, N - 1);
      if (idx == N - 1) break;
      auto secondMin = segRgt.Query(idx + 1, N - 1).first;
      Update(idx + 1, N - 1, val - secondMin);
    }

    assert(segLft.Query(0, N - 1).second == 0);
    assert(segRgt.Query(0, N - 1).second == N - 1);

    vector<lint> ret(N);
    for (int i = 0; i < N; i++) {
      ret[i] = segLft.Query(i, i).first;
    }
    return ret;
  }

  lint cartesianDepth(const vector<lint>& A) {
    int N = A.size();
    SegTree<0> segLft(N);
    segLft.Build(A);
    auto Dfs = [&](auto self, int l, int r) -> int {
      if (l > r) return 0;
      auto [val, m] = segLft.Query(l, r);
      return max(self(self, l, m - 1), self(self, m + 1, r)) + 1;
    };
    return Dfs(Dfs, 0, N - 1);
  }
};
