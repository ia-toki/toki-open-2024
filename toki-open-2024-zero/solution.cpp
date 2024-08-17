#include "buffet.h"

#if not LOCAL
#define NDEBUG 1
#endif
#include <bits/stdc++.h>
using namespace std;

#define rep(i, a, b) for (auto i = a; i < (b); ++i)
#define down(x, a) for (auto x = a; x--;)
#define all(x) begin(x), end(x)
#define sz(x) int(size(x))
#define let auto const

using ll = long long;
using lint = ll;
using pii = pair<int, int>;
using vi = vector<int>;

template <class T>
struct YComb_ {
  T f;
  template <class... A>
  auto operator()(A&&... a) const {
    return f(ref(*this), forward<A>(a)...);
  }
};
template <class T>
YComb_<T> YComb(T f) {
  return {move(f)};
}

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

long long min_leftover(int N, std::vector<int> A) {
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

  auto M = segLft.Query(0, N - 1).first;
  Update(0, N - 1, -M);

  priority_queue<pair<int, data_t>> pq;
  YComb([&](auto self, int l, int r) -> void {
    if (l > r) return;
    auto [val, idx] = segLft.Query(l, r);
    pq.push({r - l + 1, val});
    Update(l, r, -val);
    self(l, idx - 1);
    self(idx + 1, r);
  })(0, N - 1);

  while (M > 0 && !pq.empty()) {
    auto [val, cnt] = pq.top();
    pq.pop();
    if (cnt > M) {
      pq.push({val, cnt - M});
      M = 0;
    } else {
      M -= cnt;
    }
  }

  data_t ans = 0;
  while (!pq.empty()) {
    auto [val, cnt] = pq.top();
    pq.pop();
    ans += static_cast<data_t>(val) * cnt;
  }

  return ans;
}
