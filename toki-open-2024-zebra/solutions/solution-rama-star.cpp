#include "zebra.h"

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

int find_root(int N, std::vector<int> U, std::vector<int> V) {
  vector<vector<pair<int, int>>> adj(N);
  rep(i, 0, N - 1) {
    adj[U[i]].push_back({V[i], i});
    adj[V[i]].push_back({U[i], i});
  }
  auto IsMonochrome = [&](vector<int> edges) {
    sort(all(edges));
    edges.erase(unique(all(edges)), end(edges));
    if (sz(edges) <= 1) return true;
    return !is_zebra(edges);
  };
  auto IsRoot = [&](int u) {  // Check if u is a root in 2 queries
    vector<int> que = {u};
    vector<int> depth(N, -1);
    vector<int> parentEdge(N, -1);
    depth[u] = 0;
    rep(q, 0, sz(que)) {
      int u = que[q];
      for (auto [v, i] : adj[u])
        if (depth[v] == -1) {
          depth[v] = depth[u] + 1;
          parentEdge[v] = i;
          que.push_back(v);
        }
    }
    vector<int> odd, even;
    for (int i = 1; i < sz(que); i++) {
      (depth[que[i]] & 1 ? odd : even).push_back(parentEdge[que[i]]);
    }
    assert(!odd.empty());
    if (odd.empty() || even.empty()) {
      return IsMonochrome(odd) && IsMonochrome(even);
    } else {
      return IsMonochrome(odd) && IsMonochrome(even) &&
             !IsMonochrome({odd[0], even[0]});
    }
  };
  auto FindFurthest = YComb([&](auto self, int u, int p) -> pii {
    pair<int, int> res = {0, u};
    for (auto [v, i] : adj[u])
      if (v != p) {
        auto [d, x] = self(v, u);
        res = max(res, {d + 1, x});
      }
    return res;
  });
  auto GetOtherEnd = [&](int u, int e) { return U[e] ^ V[e] ^ u; };
  auto diamUp = FindFurthest(FindFurthest(0, -1).second, -1).second;
  auto [diamLength, diamDn] = FindFurthest(diamUp, -1);
  assert(diamLength >= 2);
  vector<int> depth(N, -1);
  vector<int> parentEdge(N, -1);
  YComb([&](auto self, int u, int p) -> void {
    depth[u] = p != -1 ? depth[p] + 1 : 0;
    for (auto [v, i] : adj[u])
      if (v != p) {
        parentEdge[v] = i;
        self(v, u);
      }
  })(diamUp, -1);
  // Special Case: Star Graph
  if (diamLength == 2) {
    int center = GetOtherEnd(diamDn, parentEdge[diamDn]);
    if (IsRoot(center)) return center;
    // There is a single edge with a different color
    int lo = 0, hi = sz(adj[center]);
    while (lo < hi) {
      int mid = (lo + hi) / 2;
      vector<int> edges;
      rep(i, 0, mid + 1) edges.push_back(adj[center][i].second);
      if (IsMonochrome(edges)) {
        lo = mid + 1;
      } else {
        hi = mid;
      }
    }
    assert(lo < sz(adj[center]));
    if (lo == 1 &&
        IsMonochrome({adj[center][lo].second, adj[center].back().second})) {
      return GetOtherEnd(center, adj[center][0].second);
    } else {
      return GetOtherEnd(center, adj[center][lo].second);
    }
  }
  return -1;
}
